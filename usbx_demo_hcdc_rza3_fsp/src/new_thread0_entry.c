#include "new_thread0.h"
#include "ux_api.h"
#include "ux_host_startup.h"

#include "bsp_api.h"

#include "ux_host_class_cdc_acm.h"

#define OHCI_ADDRESS	(R_USB10_BASE+0x000)
#define EHCI_ADDRESS	(R_USB10_BASE+0x100)

#define EVENT_USB_PLUG_IN          (1UL << 0)
#define EVENT_USB_PLUG_OUT         (1UL << 1)
#define UX_FSP_DEVICE_INSERTION    (0x01U)
#define UX_FSP_DEVICE_REMOVAL      (0x02U)


ULONG       actual_flags    = 0;

UX_HOST_CLASS_CDC_ACM* p_cdc_acm = UX_NULL;

/* CDC-ACM reception data buffer. */
UCHAR buffer[128];

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

    /* Check the class container if it is for a USBX CDC/ACM class. */
    if (UX_SUCCESS ==
        _ux_utility_memory_compare(_ux_system_host_class_cdc_acm_name, host_class,
                                   _ux_utility_string_length_get(_ux_system_host_class_cdc_acm_name)))
    {
        /* Check if there is a device insertion. */
        if (UX_FSP_DEVICE_INSERTION == event)
        {
            p_cdc_acm = (UX_HOST_CLASS_CDC_ACM*)instance;
            if(p_cdc_acm->ux_host_class_cdc_acm_interface->ux_interface_descriptor.bInterfaceClass!=
                    UX_HOST_CLASS_CDC_DATA_CLASS)
            {
                if(p_cdc_acm->ux_host_class_cdc_acm_next_instance!=UX_NULL)
                {

                    /* It seems the DATA class is on the second interface. Or we hope !  */
                    p_cdc_acm = p_cdc_acm->ux_host_class_cdc_acm_next_instance;

                    /* Check again this interface, if this is not the data interface, we give up.  */
                    if(p_cdc_acm->ux_host_class_cdc_acm_interface->ux_interface_descriptor.bInterfaceClass!=
                            UX_HOST_CLASS_CDC_DATA_CLASS)
                    {
                        /* We did not find a proper data interface. */
                        p_cdc_acm = UX_NULL;
                    }
                }
            }

            if(p_cdc_acm!=UX_NULL)
            {
                if(p_cdc_acm->ux_host_class_cdc_acm_interface->ux_interface_descriptor.bInterfaceClass==
                        UX_HOST_CLASS_CDC_DATA_CLASS)
                {
                    tx_event_flags_set(&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_OR);
                }
            }
        }
        else if (UX_FSP_DEVICE_REMOVAL == event) /* Check if there is a device removal. */
        {
            p_cdc_acm = UX_NULL;
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
    ULONG actual_length;

    status = ux_host_startup(&g_basic0_ctrl, &g_basic0_cfg, usb_host_initialization);
    if (status != UX_SUCCESS)
    {
        printf("ux_host_startup API FAILED..\r\n");
        return;
    }

    printf("Init completed!\r\n\r\n");

    printf("Please insert the USB CDC ACM device.\r\n");

    /*  Wait until device inserted.*/
    tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
    if(EVENT_USB_PLUG_IN == actual_flags)
    {
        printf("USB CDC ACM device is inserted\r\n");
    }

    /* Reset the event flag */
    actual_flags = 0;
    loop_count = 0;

    while (true)
    {
        loop_count++;

        /*  Check if USB is plugged out.*/
        tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_OUT, TX_AND_CLEAR, &actual_flags, TX_NO_WAIT);
        if(EVENT_USB_PLUG_OUT == (actual_flags&EVENT_USB_PLUG_OUT))
        {
            /* clear current flags */
            actual_flags = 0;
            tx_event_flags_set(&g_usb_plug_events, 0, TX_AND);

            printf("\r\nPlease connect USB CDC ACM device.\r\n\r\n");

            /*  Wait until device inserted.*/
            while(true)
            {
                tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
                if(EVENT_USB_PLUG_IN == (actual_flags&EVENT_USB_PLUG_IN))
                {
                    break;
                }
            }

            printf("USB CDC ACM device is inserted\r\n");
            loop_count = 0;
            actual_flags = 0;
        }
        else
        {
            /* Send the AT command.  */
            buffer[0] = 'A';
            buffer[1] = 'T';
            buffer[2] = '\r';
            buffer[3] = '\n';

            status = ux_host_class_cdc_acm_write(p_cdc_acm,  buffer, 4, &actual_length);
            if (status != UX_SUCCESS)
            {
                printf("write error %d\r\n", status);
                tx_thread_sleep(500);
                continue;
            }
            else
            {
                printf("write length %d\r\n", actual_length);
            }

            /* Receive for the echo-back. */
            status = ux_host_class_cdc_acm_read(p_cdc_acm,  buffer, sizeof(buffer)-1, &actual_length);
            if (status != UX_SUCCESS)
            {
                printf("read error %d\r\n", status);
                tx_thread_sleep(500);
                continue;
            }
            else
            {
                buffer[actual_length] = '\0';
                printf("received(%d) : %s\r\n", actual_length, buffer);
            }


            /* Receive for ZLP. */
            status = ux_host_class_cdc_acm_read(p_cdc_acm,  buffer, sizeof(buffer)-1, &actual_length);
            if (status != UX_SUCCESS)
            {
                printf("read error %d\r\n", status);
                tx_thread_sleep(500);
                continue;
            }
            else
            {
                buffer[actual_length] = '\0';
                printf("received(%d) : %s\r\n", actual_length, buffer);
            }

            tx_thread_sleep(500);
        }
    }
}

