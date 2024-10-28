/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "tx_api.h"
//#include "tx_execution_profile.h"

#include "hal_data.h"
#include "startup_thread.h"
#include "adv7513.h"

#define GUI_ON
//#define TOUCH_ON
//#define H264_ON

TX_THREAD      touch_thread;
TX_THREAD      h264_thread;
/* For fps. 2022/07/12 */
TX_THREAD      fps_thread;

/* test */
//extern long still_image_width;
//extern long still_image_height;
//extern unsigned short still_image_data[1024000];
/* test */

static uint8_t touch_thread_stack[4096] BSP_PLACE_IN_SECTION(".stack.touch_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
static uint8_t h264_thread_stack[1024*8] BSP_PLACE_IN_SECTION(".stack.h264_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
/* For fps. 2022/07/12 */
static uint8_t fps_thread_stack[512] BSP_PLACE_IN_SECTION(".stack.fps_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);

extern VOID init_demo_guix_widget_types(void);
extern VOID touch_thread_entry(ULONG thread_input);
extern VOID h264_thread_entry(ULONG thread_input);
/* For fps. 2022/07/12 */
extern VOID fps_thread_entry(ULONG thread_input);

volatile uint8_t flag_lcdc_frameend;
volatile uint32_t lcd_irq_cnt;


/* LCDC interrupt Callback function */
void _rm_guix_port_display_callback(display_callback_args_t *p_args)
{
    if(p_args->event == DISPLAY_EVENT_FRAME_END)
    {
        flag_lcdc_frameend = 1;
        lcd_irq_cnt++;
    }
}

/* Blinky Thread entry function */
void startup_thread_entry (void)
{
#if 0
    uint32_t   width;
    uint32_t   height;
    uint8_t   * pbuff;
    int i;

    /* Initialize buffer */
    for(i=0; i<16;i++){
        pbuff = (uint8_t *)&layer1_buffer[i][0];  /* Y */
        memset(pbuff, 0xff, DISPLAY_VSIZE_INPUT0 * DISPLAY_HSIZE_INPUT0);
        pbuff = (uint8_t *)&layer1_buffer_CB[i][0]; /* Cb */
        memset(pbuff, 0x80, DISPLAY_VSIZE_INPUT0 / 2 * DISPLAY_BUFFER_STRIDE_BYTES_INPUT0_CBCR);
        pbuff = (uint8_t *)&layer1_buffer_CR[i][0]; /* Cr */
        memset(pbuff, 0x80, DISPLAY_VSIZE_INPUT0 / 2 * DISPLAY_BUFFER_STRIDE_BYTES_INPUT0_CBCR);

    }
#endif

    /* initialize HomeAutomation demo */
    /* This initialization function is used to initialize the LCDC.
       must be called after the g_hal_init function in the tx_startup_common_init function. */
#ifdef GUI_ON
/* For execution profile. 2022/07/12 */
#ifdef TX_EXECUTION_PROFILE_ENABLE
    /* Stop and reset the execution profile timer. */
    g_timer1.p_api->open (g_timer1.p_ctrl, g_timer1.p_cfg);
    /* Start the execution profile timer. */
    g_timer1.p_api->start (g_timer1.p_ctrl);
#endif /* TX_EXECUTION_PROFILE_ENABLE */

    init_demo_guix_widget_types();

#ifdef TOUCH_ON
    tx_thread_create(&touch_thread, "Touch Thread",
                     touch_thread_entry, 0,
                     touch_thread_stack, sizeof(touch_thread_stack),
                     1, 1,
                     TX_NO_TIME_SLICE, TX_AUTO_START);
#endif

#ifdef TX_EXECUTION_PROFILE_ENABLE
    /* For fps. 2022/07/12 */
#if 0
    tx_thread_create(&fps_thread, "Fps Thread",
                     fps_thread_entry, 0,
                     fps_thread_stack, sizeof(fps_thread_stack),
                     1, 1,
                     TX_NO_TIME_SLICE, TX_AUTO_START);
#endif
#endif /* TX_EXECUTION_PROFILE_ENABLE */
#endif
#ifdef H264_ON
    tx_thread_create(&h264_thread, "h.264 Thread",
                     h264_thread_entry, 0,
                     h264_thread_stack, sizeof(h264_thread_stack),
                     1, 1,
                     TX_NO_TIME_SLICE, TX_AUTO_START);
#endif

    while (1)
    {
        tx_thread_sleep(1000);
    }
}
