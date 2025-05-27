#include "sd_card_detection_thread.h"
#include "filex_test_thread.h"
#include "stdio.h"

#define CD_PORT                                 (BSP_IO_PORT_10_PIN_03)
#define FILEX_TEST_THREAD_MEDIA_INSERT_EVENT    (uint32_t)(1 << 0)
#define FILEX_TEST_THREAD_MEDIA_REMOVE_EVENT    (uint32_t)(1 << 1)

/* Card Detection Thread entry function */
void sd_card_detection_thread_entry(void)
{
    bsp_io_level_t pin_value;
    bsp_io_level_t old_pin_value = -1;

    while (1)
    {
        g_ioport.p_api->pinRead( g_ioport.p_ctrl, CD_PORT, &pin_value );
        if( old_pin_value != pin_value )
        {
            if( pin_value == BSP_IO_LEVEL_LOW )
            {
                printf( "BSP_IO_LEVEL_LOW:INSERT\r\n" );
                tx_event_flags_set (&g_media_event_flags, FILEX_TEST_THREAD_MEDIA_INSERT_EVENT, TX_OR);
            }
            else
            {
                printf( "BSP_IO_LEVEL_HIGH:REMOVE\r\n" );
                tx_event_flags_set (&g_media_event_flags, FILEX_TEST_THREAD_MEDIA_REMOVE_EVENT, TX_OR);
            }
            old_pin_value = pin_value;
        }
        tx_thread_sleep (200);
    }
}
