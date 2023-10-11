#include "new_thread0.h"
#include "ux_api.h"
#include "ux_host_startup.h"

#include "bsp_api.h"

#include "ux_host_class_storage.h"

#define OHCI_ADDRESS	(R_USB10_BASE+0x000)
#define EHCI_ADDRESS	(R_USB10_BASE+0x100)

#define EVENT_USB_PLUG_IN          (1UL << 0)
#define EVENT_USB_PLUG_OUT         (1UL << 1)
#define UX_FSP_DEVICE_INSERTION    (0x01U)
#define UX_FSP_DEVICE_REMOVAL      (0x02U)


FX_FILE     g_file;
FX_MEDIA *  g_p_media       = UX_NULL;
ULONG       actual_flags    = 0;

ULONG   error_counter = 0;

CHAR    file_name[64];
UCHAR   read_buffer[128];

/*******************************************************************************************************************//**
 * @brief     In this function, checks the USB device status and notifies to perform operation
 * @param[IN] event       updates usb events on attaching and dettaching device.
 * @param[IN] *host_class  pointer to host class device structure
 * @param[IN] instance
 * @retval    none
 **********************************************************************************************************************/
static UINT apl_change_function (ULONG event, UX_HOST_CLASS * host_class, VOID * instance)
{
    UINT                          status = UX_SUCCESS;
    UX_HOST_CLASS               * class;
    UX_HOST_CLASS_STORAGE       * storage;
    UX_HOST_CLASS_STORAGE_MEDIA * storage_media;
    FSP_PARAMETER_NOT_USED      ((void) instance);

    /* Check the class container if it is for a USBX Host Mass Storage class. */
    if (UX_SUCCESS ==
        _ux_utility_memory_compare(_ux_system_host_class_storage_name, host_class,
                                   _ux_utility_string_length_get(_ux_system_host_class_storage_name)))
    {
        /* Check if there is a device insertion. */
        if (UX_FSP_DEVICE_INSERTION == event)
        {
            status = ux_host_stack_class_get(_ux_system_host_class_storage_name, &class);
            if (UX_SUCCESS != status)
            {
                return status;
            }
            status = ux_host_stack_class_instance_get(class, 0, (void **) &storage);
            if (UX_SUCCESS != status)
            {
                return status;
            }
            if (UX_HOST_CLASS_INSTANCE_LIVE != storage->ux_host_class_storage_state)
            {
                return UX_ERROR;
            }
            storage_media = class->ux_host_class_media;
            g_p_media     = &storage_media->ux_host_class_storage_media;
            tx_event_flags_set(&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_OR);
        }
        else if (UX_FSP_DEVICE_REMOVAL == event) /* Check if there is a device removal. */
        {
            g_p_media = UX_NULL;
            tx_event_flags_set(&g_usb_plug_events, EVENT_USB_PLUG_OUT, TX_OR);
        }
        else
        {
        }
    }
    return status;
}

static UINT usb_host_initialization(void)
{
    UINT status;

    /* ux host stack initialization */
    status =  ux_host_stack_initialize(apl_change_function);
    if (status != UX_SUCCESS)
    {
        printf("UX_HOST_STACK_INITIALIZE API FAILED..\r\n");
        return UX_ERROR;
    }

    /* Register all the USB host controllers available in this system */
    status =  ux_host_stack_hcd_register(_ux_system_host_hcd_ohci_name, ux_hcd_ohci_initialize, OHCI_ADDRESS, 0x0);
    if (status != UX_SUCCESS)
    {
        printf("OHCI regist FAILED..\r\n");
        return UX_ERROR;
    }

    status =  ux_host_stack_hcd_register(_ux_system_host_hcd_ehci_name, ux_hcd_ehci_initialize, EHCI_ADDRESS, 0x0);
    if (status != UX_SUCCESS)
    {
        printf("EHCI regist FAILED..\r\n");
        return UX_ERROR;
    }

    return UX_SUCCESS;
}

/* USB Thread entry function */
void new_thread0_entry(void)
{
    UINT status;

    int     loop_count;
    int     files_count;
    UINT    file_attribute;
    ULONG   requested_length;
    ULONG   read_count;

    status = ux_host_startup(&g_basic0_ctrl, &g_basic0_cfg, usb_host_initialization);
    if (status != UX_SUCCESS)
    {
        printf("ux_host_startup API FAILED..\r\n");
        return;
    }

    printf("Init completed!\r\n\r\n");

    printf("Please insert the USB Pendrive\r\n");

    /*  Wait until device inserted.*/
    tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
    if(EVENT_USB_PLUG_IN == actual_flags)
    {
        printf("USB Pendrive is inserted\r\n");
    }

    /* Reset the event flag */
    actual_flags = 0;
    loop_count = 0;

    while (true)
    {
        loop_count++;

        /*  Check if USB is plugged out.*/
        tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_OUT, TX_AND_CLEAR, &actual_flags, TX_NO_WAIT);
        if(EVENT_USB_PLUG_OUT == actual_flags)
        {
            printf("\r\nPlease connect USB Pen drive for work.\r\n\r\n");
            actual_flags = 0;

            /*  Wait until device inserted.*/
            while(EVENT_USB_PLUG_IN != actual_flags)
            {
                tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
                if(EVENT_USB_PLUG_IN == actual_flags)
                {
                    printf("USB Pendrive is inserted\r\n");
                    loop_count = 0;
                }
            }
        }
        else
        {
            /* Find first file on media.  */
            status =  fx_directory_first_entry_find(g_p_media, file_name);
            if (status != UX_SUCCESS)
            {
                tx_thread_sleep(20);
                continue;
            }

#if 1
            /* Seek, Write, Read test */
            printf("\"TEST.TXT\" file create.\r\n");

            /* Open the test file.  */
            fx_file_create(g_p_media, "TEST.TXT");

            status =  fx_file_open(g_p_media, &g_file, "TEST.TXT", FX_OPEN_FOR_WRITE);
            /* Check the file open status.  */
            if (status != FX_SUCCESS)
            {
                printf("File create FAILED..\r\n");
                /* Error opening file, break the loop.  */
                break;
            }

            /* Seek to the beginning of the test file.  */
            status =  fx_file_seek(&g_file, 0);
            /* Check the file seek status.  */
            if (status != FX_SUCCESS)
            {
                printf("File seek FAILED..\r\n");
                /* Error performing file seek, break the loop.  */
                break;
            }

            /* Write a string to the test file.  */
            status =  fx_file_write(&g_file, " ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", 28);
            /* Check the file write status.  */
            if (status != FX_SUCCESS)
            {
                printf("File write FAILED..\r\n");
                /* Error writing to a file, break the loop.  */
                break;
            }

            printf("    Data has been written to the file.\r\n");

            /* Seek to the beginning of the test file.  */
            status =  fx_file_seek(&g_file, 0);
            /* Check the file seek status.  */
            if (status != FX_SUCCESS)
            {
                printf("File seek FAILED..\r\n");
                /* Error performing file seek, break the loop.  */
                break;
            }

            /* Read the first 28 bytes of the test file.  */
            status =  fx_file_read(&g_file, read_buffer, 28, &requested_length);
            /* Check the file read status.  */
            if ((status != FX_SUCCESS) || (requested_length != 28))
            {
                printf("File read FAILED..\r\n");
                /* Error reading file, break the loop.  */
                break;
            }

            printf("    It checked the data in the file.\r\n");

            /* Close the test file.  */
            status =  fx_file_close(&g_file);
            /* Check the file close status.  */
            if (status != FX_SUCCESS)
            {
                printf("File close FAILED..\r\n");
                /* Error closing the file, break the loop.  */
                break;
            }

            /* Media flush  */
            status =  fx_media_flush(g_p_media);
            /* Check the media flush status.  */
            if (status != FX_SUCCESS)
            {
                printf("Media flush FAILED..\r\n");
                /* Error media flush, break the loop.  */
                break;
            }
#else
            /* We get here if there is at least a file in the directory.  */
            files_count = 0;
            do
            {
                /* Loop through the files in the root directory.  */

                printf("    File name:<%s>.\r\n", file_name);

                /* Increment file count.  */
                files_count++;

                /* Reset file attributes.  */
                file_attribute = 0;

                /* Try to read the file attributes.  */
                status =  fx_file_attributes_read(g_p_media, file_name, &file_attribute);

                /* If this is a directory, pass.  */
                if(!(file_attribute & 0x18) && (status == UX_SUCCESS))
                {
                    /* A file is present, attempt to read its contents.  */

                    /* Try to open the file.  */
                    status =  fx_file_open(g_p_media, &g_file, file_name, FX_OPEN_FOR_READ);
                    if (status != UX_SUCCESS)
                        break;

                    read_count = 0;

                    while(status == UX_SUCCESS)
                    {
                        /* Read the file in blocks */
                        requested_length = 0;
                        status = fx_file_read(&g_file, read_buffer, sizeof(read_buffer), &requested_length);

                        /* Check if status OK.  */
                        if (status != UX_SUCCESS)
                        {
                            error_counter++;
                            break;
                        }

                        read_count += requested_length;

                        /* Check if end of file.  */
                        if (requested_length != sizeof(read_buffer))
                            break;
                    }

                    /* Finished reading file either at the end or because of error. */
                    fx_file_close(&g_file);

                    printf("        File size=%d\r\n", read_count);
                }

                /* Find the next file.  */
                status =  fx_directory_next_entry_find(g_p_media, file_name);
            } while (status == UX_SUCCESS);

            printf("    Total file count:%d, Loop count:%d\r\n\r\n", files_count, loop_count);
#endif

            tx_thread_sleep(500);
        }
    }
}

