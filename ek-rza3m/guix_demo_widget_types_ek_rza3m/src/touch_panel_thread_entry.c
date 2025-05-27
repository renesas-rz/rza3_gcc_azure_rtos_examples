#include "touch_panel_thread.h"
#include "device\touch_panel\inc\touch_panel.h"
//extern ULONG display_guix_get_handle( void );
#include "rz_a3_display_driver.h"

/* Touch Panel Thread entry function */
void touch_panel_thread_entry(void)
{
    UINT status;
    tp_messege_info_t *p_tp_info;
    GX_EVENT touch_event;

    //tx_thread_sleep(2000);

    printf( "\r\n[RZA3UL] touch_panel_thread_entry\r\n" );

    /* touch panel device init */
    touch_panel_init( &g_tp_queue, (external_irq_instance_t*)&g_external_irq0);

    while (1)
    {
        status = tx_queue_receive( &g_tp_queue, (void*)&p_tp_info, TX_WAIT_FOREVER );
        if( TX_SUCCESS == status )
        {
            switch(p_tp_info->state)
            {
            case TP_STATE_PUSH:
                touch_event.gx_event_type = GX_EVENT_PEN_DOWN;
                touch_event.gx_event_sender = GX_ID_NONE;
                touch_event.gx_event_target = 0;
                //touch_event.gx_event_display_handle = display_guix_get_handle();
                touch_event.gx_event_display_handle = RZ_SCREEN_HANDLE;
                touch_event.gx_event_payload.gx_event_pointdata.gx_point_x = (GX_VALUE)p_tp_info->pos_x;
                touch_event.gx_event_payload.gx_event_pointdata.gx_point_y = (GX_VALUE)p_tp_info->pos_y;
                break;
            case TP_STATE_RELEASE:
                touch_event.gx_event_type = GX_EVENT_PEN_UP;
                touch_event.gx_event_sender = GX_ID_NONE;
                touch_event.gx_event_target = 0;
                //touch_event.gx_event_display_handle = display_guix_get_handle();
                touch_event.gx_event_display_handle = RZ_SCREEN_HANDLE;
                touch_event.gx_event_payload.gx_event_pointdata.gx_point_x = (GX_VALUE)p_tp_info->pos_x;
                touch_event.gx_event_payload.gx_event_pointdata.gx_point_y = (GX_VALUE)p_tp_info->pos_y;
                break;
            case TP_STATE_DRAG:
                touch_event.gx_event_type = GX_EVENT_PEN_DRAG;
                touch_event.gx_event_sender = GX_ID_NONE;
                touch_event.gx_event_target = 0;
                //touch_event.gx_event_display_handle = display_guix_get_handle();
                touch_event.gx_event_display_handle = RZ_SCREEN_HANDLE;
                touch_event.gx_event_payload.gx_event_pointdata.gx_point_x = (GX_VALUE)p_tp_info->pos_x;
                touch_event.gx_event_payload.gx_event_pointdata.gx_point_y = (GX_VALUE)p_tp_info->pos_y;
                break;
            default:
                printf("[TEST] TP_STATE_XXX(%d):X(%d),Y(%d)\r\n", p_tp_info->state, p_tp_info->pos_x, p_tp_info->pos_y);
                continue;
            }

            status = gx_system_event_send(&touch_event);
            if( GX_SUCCESS != status )
            {
                printf("[TEST] gx_system_event_send failed(%d)\r\n", status );
            }

            /* release tp message */
            touch_panel_message_release(p_tp_info);
        }
        else
        {
            printf("[TEST] tx_queue_receive is failed(%d).\r\n", status);
        }
    }
}
