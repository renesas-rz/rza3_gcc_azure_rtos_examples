#include <device/ili9881/ili9881.h>

#include "display_thread.h"
#include <stdio.h>

/**
 * defines
 */

/**
 * static variable declaration
 */

/**
 * global function prototype
 */
bool g_message_sent = false;

/**
 * static function prototype
 */
static void _display_panel_clock_setup( void );
static void _display_rza3m_lcdc_setup( void );

void mipi_dsi0_callback(mipi_dsi_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);

    if(p_args->event == MIPI_DSI_EVENT_POST_OPEN)
    {
        /*Interface has been opened. Perform post-open application processing*/
    }
    else if(p_args->event == MIPI_DSI_EVENT_PRE_START)
    {
        /*Video is about to start. Perform pre-video application processing*/
    }
    else if(p_args->event == MIPI_DSI_EVENT_SEQUENCE_0)
    {
         g_message_sent = ((p_args->tx_status & MIPI_DSI_SEQUENCE_STATUS_DESCRIPTORS_FINISHED) == MIPI_DSI_SEQUENCE_STATUS_DESCRIPTORS_FINISHED);
    }
    else
    {
        //do nothing
    }
}

/* Display Thread entry function */
void display_thread_entry(void)
{

	tx_thread_sleep (500);

    /* panel clock setup */
    _display_panel_clock_setup();

    /* lcdc setup */
    _display_rza3m_lcdc_setup();

    /* MIPI-DSI setup */
    R_MIPI_DSI_B_Open(g_mipi_dsi0.p_ctrl, g_mipi_dsi0.p_cfg );
    ili9881_init(g_mipi_dsi0.p_ctrl);
    R_MIPI_DSI_B_Start(g_mipi_dsi0.p_ctrl);

    /* TODO: add your own code here */
    while (1)
    {
        tx_thread_sleep (1);
    }
}

static void _display_panel_clock_setup( void )
{
    /* Setting for PIXEL Clock (PLL5 Pixel Clock) */
#if 0
    /* PLL: 792.0MHz, division ratio: 1/12  */
    //R_CPG->CPG_OTHERFUNC1_REG = 0x1;
    R_CPG->CPG_SIPLL5_CLK1 = 0x01110231;
    R_CPG->CPG_SIPLL5_CLK3 = 0x00000000;
    R_CPG->CPG_SIPLL5_CLK4 = 0x00C60000;
    R_CPG->CPG_PL5_SDIV    = 0x01010B00;
    R_CPG->CPG_SIPLL5_STBY = 0x00150001;
#else
    /* 66MHz 800x1280 */
    uint32_t reg;
    uint32_t fracin;
    uint32_t intin;
    uint32_t refdiv;
    uint32_t postdiv1;
    uint32_t postdiv2;
    uint32_t divdsia_set;
    uint32_t divdsib_set;

    fracin   = 0;
    intin    = 66;
    refdiv    = 1;
    postdiv1 = 2;
    postdiv2 = 1;
    divdsia_set = 0;
    divdsib_set = 11;

    /* Setting for PIXEL Clock (PLL5 Pixel Clock) */
    //R_CPG->CPG_OTHERFUNC1_REG = 0x00010001;//@@
    reg  = R_CPG->CPG_SIPLL5_CLK3;
    reg &= 0x0000FFFF;
    reg |= (fracin << 16);
    R_CPG->CPG_SIPLL5_CLK3 = reg;
    R_CPG->CPG_SIPLL5_CLK4 = (intin << 16);
    reg = R_CPG->CPG_SIPLL5_CLK1;
    reg &= 0xFFFF0000;
    reg |= ( (1 << 24) | (1 << 20) | (1 << 16) | (refdiv << 8) | (postdiv2 << 4) | postdiv1 );
    R_CPG->CPG_SIPLL5_CLK1 = reg;
    reg = 0x01010000;
    reg |= (divdsib_set << 8) | (divdsia_set);
    R_CPG->CPG_PL5_SDIV  = reg;
    R_CPG->CPG_SIPLL5_STBY = 0x00150001;
#endif
}

static void _display_rza3m_lcdc_setup( void )
{
    fsp_err_t err;

    /* Initialize LCDC */
    err = g_display0.p_api->open( g_display0.p_ctrl, g_display0.p_cfg );
    if( FSP_SUCCESS != err )
    {
        printf("g_display0 open failed(%d)\r\n", err);
    }

    err = g_display0.p_api->start( g_display0.p_ctrl );
    if( FSP_SUCCESS != err )
    {
        printf("g_display0 start failed(%d)\r\n", err);
    }
}

