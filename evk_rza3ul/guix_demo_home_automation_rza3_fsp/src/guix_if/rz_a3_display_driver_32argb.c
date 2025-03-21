
#include    <stdio.h>
#include    <string.h>

#include    "bsp_api.h"
#include    "hal_data.h"
#include    "adv7513.h"
#include    "adv7513_data.h"

#include    "gx_api.h"

#include    "rz_a3_display_driver.h"

#define LCD_GUI_ON

#if (COLOR_FORMAT == GX_COLOR_FORMAT_32ARGB)

static void _CopyCanvasToBackBuffer32argb(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;
    ULONG *pPutRow;
    ULONG *pGetRow;
    ULONG *pGet;
    ULONG *pPut;
    int row;
    int col;
    int copy_width;
    int copy_height;
/*test*/
    ULONG wk;
/*test*/
    gx_utility_rectangle_define(&display_size, 0, 0, DISPLAY_HSIZE_INPUT1 - 1, DISPLAY_VSIZE_INPUT1 - 1);
    copy_clip = *copy;

    gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);
    gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width = (copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left + 1);
    copy_height = copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top + 1;

    if (copy_width <= 0 ||
        copy_height <= 0)
    {
        return;
    }
    
    pGetRow = (ULONG *) canvas -> gx_canvas_memory;
    pGetRow += copy->gx_rectangle_top * canvas->gx_canvas_x_resolution;
    pGetRow += copy->gx_rectangle_left;

    pPutRow = &layer2_buffer[0][0];
    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * DISPLAY_HSIZE_INPUT1;
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left);

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;
        
        for (col = 0; col < copy_width; col++)
        {
/*test*/
#if 0
            wk = *pGet++;
            wk &= 0x00FFFFFF;
            wk |= 0x80000000;
            *pPut++ = wk;
#else
            *pPut++ = *pGet++;
#endif
/*test*/
        }
        pGetRow += canvas->gx_canvas_x_resolution;
        pPutRow += DISPLAY_HSIZE_INPUT1;
    }
    R_BSP_CACHE_CleanInvalidateRange(layer2_buffer[0], sizeof(layer2_buffer[0]));
}

void rz_a3_32argb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_RECTANGLE Limit;
    GX_RECTANGLE Copy;
        
    gx_utility_rectangle_define(&Limit, 0, 0,
        canvas->gx_canvas_x_resolution -1,
        canvas->gx_canvas_y_resolution -1);
    
    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        /* copy our canvas to the back buffer */
        _CopyCanvasToBackBuffer32argb(canvas, &Copy);
    }

}

int32_t rz_a3_32argb_lcd_init(void)
{
#ifdef LCD_GUI_ON
    /* Initialize asv7513 for display  */
    adv7513_init();
#if 1
    {
        /* 72MHz 1280x800 */
        uint32_t reg;
        uint32_t fracin  = 800;
        uint32_t intin   = 213;
        uint32_t refdiv  = 2;
        uint32_t posdiv1 = 6;
        uint32_t posdiv2 = 6;

        /* PLL5 Pixel Clock */
        R_CPG->CPG_SIPLL5_CLK3_b.FRACIN = fracin;
        R_CPG->CPG_SIPLL5_CLK4_b.INTIN = intin;
        reg = R_CPG->CPG_SIPLL5_CLK1;
        reg &= 0xFFFF0000;
        reg |= ((1 << 24) | (refdiv << 8) | (posdiv2 << 4) | posdiv1 );
        R_CPG->CPG_SIPLL5_CLK1 = reg;

        R_CPG->CPG_PL5_SDIV  = 0x01010000;
        R_CPG->CPG_SIPLL5_STBY = 0x00150001;
//        R_CPG->CPG_RST_DDR   = 0x007F007F;
    }
#else
    /* PLL5 Pixel Clock */
    R_CPG->CPG_SIPLL5_CLK3 = 0x40000006;
    R_CPG->CPG_SIPLL5_CLK4 = 0x004a0000;
    R_CPG->CPG_PL5_SDIV  = 0x01010202;
    R_CPG->CPG_SIPLL5_STBY = 0x00150001;
//    R_CPG->CPG_RST_DDR   = 0x007F007F;
#endif

    /* Initialize buffer */
    memset(layer2_buffer[0], 0x00, sizeof(layer2_buffer[0]));

    /* Init LCDC */
    g_display_on_lcdc.open(g_display0.p_ctrl, g_display0.p_cfg);
    g_display_on_lcdc.start(g_display0.p_ctrl);
#endif // LCD_GUI_ON
    return 0;
}

UINT rz_a3_graphics_driver_setup_32argb(GX_DISPLAY *display)
{
    _gx_display_driver_32argb_setup(display, RZ_SCREEN_HANDLE, rz_a3_32argb_buffer_toggle);
    display->gx_display_handle = RZ_SCREEN_HANDLE;
    
    rz_a3_32argb_lcd_init();

    return GX_SUCCESS;
}

#endif /* (COLOR_FORMAT == GX_COLOR_FORMAT_32ARGB) */
