//#include "SampleGrape.h"
#include "startup_thread.h"
#include "hal_data.h"
#include "rz_a3_display_driver.h"
//#include <r_usb_basic.h>
//#include "common_data.h"

#include "ux_api.h"
//#include "ux_host_class_storage.h"
#include "ux_host_class_hub.h"
#include "ux_host_class_hid.h"
#include "ux_host_class_hid_mouse.h"

#include "bsp_api.h"

#include "r_usb_extern.h"

#define OHCI_ADDRESS    (R_USB10_BASE+0x000)
#define EHCI_ADDRESS    (R_USB10_BASE+0x100)

#define EVENT_USB_PLUG_IN          (1UL << 0)
#define EVENT_USB_PLUG_OUT         (1UL << 1)
#define UX_FSP_DEVICE_INSERTION    (0x01U)
#define UX_FSP_DEVICE_REMOVAL      (0x02U)

#define MOUSE_DEVICE    (2)

//ルートウインドウ用の変数
extern GX_WINDOW_ROOT* root;
static GX_EVENT mouse_event;

/* Global variables */
uint8_t  g_ux_pool_memory_[64*1024];
BSP_ALIGN_VARIABLE(256) uint8_t  g_ux_pool_memory_uncached_[64*1024] USB_BUFFER_PLACE_IN_SECTION;

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

/* USB Thread entry function */
void mouse_thread_entry(ULONG thread_input)
{
    UINT       ux_return = UX_SUCCESS;
    UINT       status    = 0;
    fsp_err_t  err       = FSP_SUCCESS;
    UINT my_old_posture;
    UINT my_old_threshold;

    UINT mouse_event_count;
    ULONG mouse_buttons;
    SLONG mouse_x;
    SLONG mouse_y;

    //マウス動作用に使用する変数
    ULONG mouse_left = 0;
    ULONG mouse_right = 0;
    ULONG mouse_middle = 0;

    ULONG current_type;
    SLONG org_x;
    SLONG org_y;
    SLONG offset_x;
    SLONG offset_y;
    SLONG current_x;
    SLONG current_y;

    org_x = 0;
    org_y = 0;

    //マウスイベントの初期化
    mouse_event.gx_event_type = GX_EVENT_REDRAW;
    //ここの値はディスプレイに設定しているものと同じ定義を格納する必要がある。
    mouse_event.gx_event_display_handle = RZ_SCREEN_HANDLE;
    mouse_event.gx_event_target = 0;
    mouse_event.gx_event_sender = GX_ID_NONE;
    mouse_event.gx_event_payload.gx_event_pointdata.gx_point_x = DISPLAY_1_X_RESOLUTION/2;
    mouse_event.gx_event_payload.gx_event_pointdata.gx_point_y = DISPLAY_1_Y_RESOLUTION/2;

    
    int size_test = sizeof(TX_THREAD);

    ux_host_startup(&g_basic0.p_ctrl,&g_basic0_cfg,UX_SUCCESS);

    /* ux_system_initialization */
    ux_return = _ux_system_initialize((CHAR *)g_ux_pool_memory_, sizeof(g_ux_pool_memory_),
                                        g_ux_pool_memory_uncached_, sizeof(g_ux_pool_memory_uncached_));
    if (UX_SUCCESS != ux_return)
    {
        printf("_UX_SYSTEM_INITIALIZE API FAILED..\r\n");
        return;
    }

    printf("ux_system initialized successfully!\r\n");

#if 0
    /* Lockout interrupts */
    my_old_posture = tx_interrupt_control(TX_INT_DISABLE);
#endif

    /* Open usb driver */
    err = R_USB_Open(&g_basic0_ctrl, &g_basic0_cfg);
    if (FSP_SUCCESS != err)
    {
        tx_interrupt_control(my_old_posture);
        printf("R_USB_OPEN API FAILED..\r\n");
        return;
    }
    /* ux host stack initialization */
    ux_return =  ux_host_stack_initialize(apl_change_function);
    if (UX_SUCCESS != ux_return)
    {
        tx_interrupt_control(my_old_posture);
        printf("UX_HOST_STACK_INITIALIZE API FAILED..\r\n");
        return;
    }

    /* Register all the USB host controllers available in this system */
    status =  ux_host_stack_hcd_register(_ux_system_host_hcd_ohci_name, ux_hcd_ohci_initialize, OHCI_ADDRESS, 0x0);
    if (status != UX_SUCCESS)
    {
        tx_interrupt_control(my_old_posture);
        printf("OHCI regist FAILED..\r\n");
        return;
    }

    status =  ux_host_stack_hcd_register(_ux_system_host_hcd_ehci_name, ux_hcd_ehci_initialize, EHCI_ADDRESS, 0x0);
    if (status != UX_SUCCESS)
    {
        tx_interrupt_control(my_old_posture);
        printf("EHCI regist FAILED..\r\n");
        return;
    }

#if 1
    /* Register the hub class.  */
    status =  ux_host_stack_class_register(_ux_system_host_class_hub_name, _ux_host_class_hub_entry);
    if (status != UX_SUCCESS)
    {
        tx_interrupt_control(my_old_posture);
        printf("UX_HOST_STACK_INITIALIZE API FAILED..\r\n");
        return;
    }
#endif

    /* Register the HID class.  */
    status =  ux_host_stack_class_register(_ux_system_host_class_hid_name, _ux_host_class_hid_entry);
    if (status != UX_SUCCESS)
    {
        tx_interrupt_control(my_old_posture);
        printf("UX_HOST_STACK_CLASS_REGISTER API(HID) FAILED..\r\n");
        return;
    }

    /* Register the HID(Mouse) client.  */
    status =  ux_host_class_hid_client_register(_ux_system_host_class_hid_client_mouse_name, ux_host_class_hid_mouse_entry);
    if (status != UX_SUCCESS)
    {
        tx_interrupt_control(my_old_posture);
        printf("UX_HOST_HID_CLIENT_REGISTER API(Mouse) FAILED..\r\n");
        return;
    }

    /* Enable interrupt */
    usb_cpu_usbint_init(g_basic0_ctrl.module_number, &g_basic0_cfg);

#if 0
    /* Restore previous interrupt lockout posture. */
    tx_interrupt_control(my_old_posture);
#endif

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

            //すでに一度マウスの処理を行った場合、ここでReDrawイベントを挟み、初期化&マウスを隠す
            if(mouse_event.gx_event_type!=GX_EVENT_REDRAW)
            {
                /* hide mouse cursor */
                gx_canvas_mouse_hide(root->gx_window_root_canvas);

                mouse_event.gx_event_type = GX_EVENT_REDRAW;
                gx_system_event_send(&mouse_event);
            }

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
            tx_thread_sleep(100);
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
                    if(mouse_event.gx_event_type==GX_EVENT_REDRAW)
                    {
                        /* show mouse cursor */
                        gx_canvas_mouse_show(root->gx_window_root_canvas);

                        org_x = 0;
                        org_y = 0;
                        mouse_event.gx_event_type = GX_EVENT_PEN_MOVE;
                    }

                    mouse_left =  mouse_buttons & UX_HOST_CLASS_HID_MOUSE_BUTTON_1_PRESSED;
                    mouse_right = mouse_buttons & UX_HOST_CLASS_HID_MOUSE_BUTTON_2_PRESSED;
                    mouse_middle = mouse_buttons & UX_HOST_CLASS_HID_MOUSE_BUTTON_3_PRESSED;

                    mouse_x = -mouse_x;
                    mouse_y = -mouse_y; /* Calculate the next point and adjust the range */
                    offset_x = org_x - mouse_x;
                    offset_y = org_y - mouse_y;

                    current_x = mouse_event.gx_event_payload.gx_event_pointdata.gx_point_x + offset_x;
                    current_y = mouse_event.gx_event_payload.gx_event_pointdata.gx_point_y + offset_y;
                    //現在のx座標が0未満の場合は0に補正
                    if(current_x < 0)
                    {
                        current_x = 0;
                    }
                    //現在のx座標が最大のdisplayサイズ以上だったら最大displayサイズに設定
                    else if(DISPLAY_1_X_RESOLUTION < current_x)
                    {
                        current_x = DISPLAY_1_X_RESOLUTION;
                    }
                    //y座標も同様に設定
                    if(current_y < 0)
                    {
                        current_y = 0;
                    }
                    else if(DISPLAY_1_Y_RESOLUTION < current_y)
                    {
                        current_y = DISPLAY_1_Y_RESOLUTION;
                    }

                    /* Create type for the next point */
                    //ここからどのイベントをGUIXへ送信するか決める　デフォルトはマウスイベント
                    current_type = GX_EVENT_PEN_MOVE;
                    //クリックされている場合は以下のif文内へ入る
                    if(mouse_left!=0)
                    {
                        if(mouse_event.gx_event_type==GX_EVENT_PEN_MOVE)
                        {
                            current_type = GX_EVENT_PEN_DOWN;
                        }
                        else
                        {
                            current_type = GX_EVENT_PEN_DRAG;
                        }
                    }
                    else
                    {
                        if((mouse_event.gx_event_type==GX_EVENT_PEN_DOWN) ||
                            (mouse_event.gx_event_type==GX_EVENT_PEN_DRAG))
                        {
                            current_type = GX_EVENT_PEN_UP;
                        }
                    }

                    //現在取得している値が、先ほど格納したx,y座標と異なる、又はイベントタイプが異なる場合は以下の処理を行う。
                    if((mouse_event.gx_event_payload.gx_event_pointdata.gx_point_x!=current_x) ||
                        (mouse_event.gx_event_payload.gx_event_pointdata.gx_point_y!=current_y) ||
                        (mouse_event.gx_event_type!=current_type))
                    {
                        /* If the status has changed, send a event */
                        mouse_event.gx_event_type = current_type;
                        mouse_event.gx_event_payload.gx_event_pointdata.gx_point_x = current_x;
                        mouse_event.gx_event_payload.gx_event_pointdata.gx_point_y = current_y;

                        status = gx_system_event_send(&mouse_event);
                        if(status!=0){
                            printf("GUIX_event_err\n");
                        }
                    }

                    /* Save the current point for the next point calculation */
                    org_x = mouse_x;
                    org_y = mouse_y;
                }
            }
            tx_thread_sleep(5);
        }
    }
}

