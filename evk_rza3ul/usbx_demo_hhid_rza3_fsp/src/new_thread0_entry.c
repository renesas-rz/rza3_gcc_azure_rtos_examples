#include "new_thread0.h"
#include "ux_api.h"
#include "ux_host_class_hid.h"
#include "ux_host_class_hid_mouse.h"

#include "bsp_api.h"

#define OHCI_ADDRESS	(R_USB10_BASE+0x000)
#define EHCI_ADDRESS	(R_USB10_BASE+0x100)

#define EVENT_USB_PLUG_IN          (1UL << 0)
#define EVENT_USB_PLUG_OUT         (1UL << 1)
#define UX_FSP_DEVICE_INSERTION    (0x01U)
#define UX_FSP_DEVICE_REMOVAL      (0x02U)

#define MOUSE_DEVICE    (2)

ULONG       actual_flags    = 0;

UX_HOST_CLASS_HID* g_mouse = UX_NULL;
UX_HOST_CLASS_HID_CLIENT* g_hid_client;

ULONG   error_counter = 0;

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

    /* Check the class container if it is for a USBX HID(Mouse) class. */

    /* Check if there is a device insertion. */
    if (UX_FSP_DEVICE_INSERTION == event)
    {
        status = ux_host_stack_class_get(_ux_system_host_class_hid_name, &class);
        if (UX_SUCCESS != status)
        {
            return status;
        }

        if(((UX_HOST_CLASS_HID *)instance)->ux_host_class_hid_interface->ux_interface_descriptor.bInterfaceProtocol == MOUSE_DEVICE)
        {
            g_mouse = (UX_HOST_CLASS_HID*)instance;
            g_hid_client = g_mouse->ux_host_class_hid_client;

            tx_event_flags_set(&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_OR);
        }
    }
    else if (UX_FSP_DEVICE_REMOVAL == event) /* Check if there is a device removal. */
    {
        g_mouse = UX_NULL;
        g_hid_client = UX_NULL;

        tx_event_flags_set(&g_usb_plug_events, EVENT_USB_PLUG_OUT, TX_OR);
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
    UINT       ux_return = UX_SUCCESS;
    UINT       status    = 0;
    fsp_err_t  err       = FSP_SUCCESS;
    UINT my_old_posture;

    UINT mouse_event_count;
    ULONG mouse_buttons;
    SLONG mouse_x;
    SLONG mouse_y;

    status = ux_host_startup(&g_basic0_ctrl, &g_basic0_cfg, usb_host_initialization);
    if (status != UX_SUCCESS)
    {
        printf("ux_host_startup API FAILED..\r\n");
        return;
    }

    printf("Init completed!\r\n\r\n");

    printf("Please insert the USB Mouse\r\n");

    /*  Wait until device inserted.*/
    tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
    if(EVENT_USB_PLUG_IN == actual_flags)
    {
        printf("USB Mouse is inserted\r\n");
    }

    /* Reset the event flag */
    actual_flags = 0;
    mouse_event_count = 0;

    while (true)
    {
        /*  Check if USB is plugged out.*/
        tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_OUT, TX_AND_CLEAR, &actual_flags, TX_NO_WAIT);
        if(EVENT_USB_PLUG_OUT == actual_flags)
        {
            printf("\r\nPlease connect USB Mouse for work.\r\n\r\n");
            actual_flags = 0;

            /*  Wait until device inserted.*/
            while(EVENT_USB_PLUG_IN != actual_flags)
            {
                tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
                if(EVENT_USB_PLUG_IN == actual_flags)
                {
                    printf("USB Mouse is inserted\r\n");
                    mouse_event_count = 0;
                }
            }
        }
        else
        {
            status = ux_host_class_hid_mouse_buttons_get(
                            (UX_HOST_CLASS_HID_MOUSE*)(g_hid_client->ux_host_class_hid_client_local_instance),
                            &mouse_buttons);
            if(status==UX_SUCCESS)
            {
                status = ux_host_class_hid_mouse_position_get (
                            (UX_HOST_CLASS_HID_MOUSE*)(g_hid_client->ux_host_class_hid_client_local_instance),
                            &mouse_x, &mouse_y);

                if(status==UX_SUCCESS)
                {
                    /* mouse event received */
                    mouse_event_count++;

                    printf("    Mouse Position<%d/%d>\r\n", (int)mouse_x, (int)mouse_y);
                }
            }

            tx_thread_sleep(100);
        }
    }
}

