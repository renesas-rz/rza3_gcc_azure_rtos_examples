
#include    <stdio.h>
#include    <string.h>

#include    "bsp_api.h"
#include    "hal_data.h"

#include    "gx_api.h"

#include    "rz_a3_display_driver.h"

#define LCD_GUI_ON



//Without 90°rotation (CW) :Rotate_Screen_V (0)
//With    90°rotation (CW) :Rotate_Screen_V (1)
#define Rotate_Screen_V (0)


//Without Double Buffering :Double_Buffering  (0)
//With    Double Buffering :Double_Buffering  (1)
#define Double_Buffering (0)

   uint8_t layer3_buffer[2][DISPLAY_BUFFER_STRIDE_BYTES_INPUT1 * DISPLAY_VSIZE_INPUT1];



#if (COLOR_FORMAT == GX_COLOR_FORMAT_32ARGB)

static void _rz_a3_32argb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty);


static void _CopyCanvasToBackBuffer32argb(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;
    ULONG *pPutRow;
    ULONG *pGetRow;
#if  Double_Buffering
    ULONG *pPutRow_pre;
    ULONG *pPut_pre;
#endif
    ULONG *pGet;
    ULONG *pPut;
    int row;
    int col;
    int copy_width;
    int copy_height;

#if  Double_Buffering
    ULONG *start_put;
#endif
    ULONG *start_get;

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
    pPutRow = (ULONG* )&layer2_buffer[0][0];


#if Rotate_Screen_V

       pGetRow += copy->gx_rectangle_top * canvas->gx_canvas_x_resolution;
       pGetRow += copy->gx_rectangle_left;
       pGetRow += canvas->gx_canvas_x_resolution * copy_height;
       start_get = pGetRow;


       pPutRow += DISPLAY_1_Y_RESOLUTION - (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_bottom);
       pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left) * DISPLAY_HSIZE_INPUT1;


   #if  Double_Buffering

       pPutRow_pre = &layer3_buffer[0][0];

       pPutRow_pre += DISPLAY_1_Y_RESOLUTION - (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_bottom) - 1 ;
       pPutRow_pre += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left) * DISPLAY_HSIZE_INPUT1;

       start_put = pPutRow_pre;

       for (row = 0; row < copy_width; row++)
             {
                 pPut_pre = pPutRow_pre;

                 pGetRow = start_get;
                 pGetRow += row;

                 for (col = 0; col < copy_height; col++)
                 {
                     pGet = pGetRow;
                     *pPut_pre++ = *pGet;

                     pGetRow -= canvas->gx_canvas_x_resolution;

                 }

                 pPutRow_pre +=   DISPLAY_HSIZE_INPUT1;

             }

       pPutRow_pre = start_put;

        for (row = 0; row < copy_width; row++)
          {

            pPut = pPutRow;
            pPut_pre = pPutRow_pre;


             for (col = 0; col < copy_height; col++)
              {

                *pPut++ = *pPut_pre++;

              }

              pPutRow += DISPLAY_HSIZE_INPUT1;
              pPutRow_pre += DISPLAY_HSIZE_INPUT1;
          }

        R_BSP_CACHE_CleanInvalidateRange(layer3_buffer[0], sizeof(layer3_buffer[0]));

   #else


       for (row = 0; row < copy_width; row++)
       {
           pPut = pPutRow;

           pGetRow = start_get;
           pGetRow += row;


           for (col = 0; col < copy_height; col++)
           {
               pGetRow -= canvas->gx_canvas_x_resolution;

               pGet = pGetRow;
               *pPut++ = *pGet;
           }

           pPutRow +=   DISPLAY_HSIZE_INPUT1;

       }

  #endif


#else

    pGetRow += copy->gx_rectangle_top * canvas->gx_canvas_x_resolution;
    pGetRow += copy->gx_rectangle_left;

    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * DISPLAY_HSIZE_INPUT1;
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left);

#if  Double_Buffering

    pPutRow_pre = &layer3_buffer[0][0];

    pPutRow_pre += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * DISPLAY_HSIZE_INPUT1;
    pPutRow_pre += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left);
    start_put = pPutRow_pre;


    for (row = 0; row < copy_height; row++)
    {
       pGet = pGetRow;
       pPut_pre = pPutRow_pre;

        for (col = 0; col < copy_width; col++)
        {

            *pPut_pre++ = *pGet++;

        }

        pGetRow += canvas->gx_canvas_x_resolution;
        pPutRow_pre += DISPLAY_HSIZE_INPUT1;

    }


    pPutRow_pre = start_put;


    for (row = 0; row < copy_height; row++)
       {

         pPut = pPutRow;
         pPut_pre = pPutRow_pre;

        for (col = 0; col < copy_width; col++)
           {

             *pPut++ = *pPut_pre++;

           }

           pPutRow += DISPLAY_HSIZE_INPUT1;
           pPutRow_pre += DISPLAY_HSIZE_INPUT1;
       }


    R_BSP_CACHE_CleanInvalidateRange(layer3_buffer[0], sizeof(layer3_buffer[0]));

  #else

    for (row = 0; row < copy_height; row++)
    {
       pGet = pGetRow;
       pPut = pPutRow;

        for (col = 0; col < copy_width; col++)
        {

            *pPut++ = *pGet++;

        }

        pGetRow += canvas->gx_canvas_x_resolution;
        pPutRow += DISPLAY_HSIZE_INPUT1;

    }

  #endif

#endif
    R_BSP_CACHE_CleanInvalidateRange((uint64_t)layer2_buffer[0], sizeof(layer2_buffer[0]));

}

static void _rz_a3_32argb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
	FSP_PARAMETER_NOT_USED(dirty);
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
    /* For fps. 2022/07/12 */
    //iFps++;
}
UINT rz_a3_graphics_driver_setup_32argb(GX_DISPLAY *display)
{
    _gx_display_driver_32argb_setup(display, (void *)RZ_SCREEN_HANDLE, _rz_a3_32argb_buffer_toggle);
    display->gx_display_handle = RZ_SCREEN_HANDLE;
    
    memset(layer2_buffer[0], 0x00, sizeof(layer2_buffer[0]));
    memset(layer3_buffer[0], 0x00, sizeof(layer3_buffer[0]));

    return GX_SUCCESS;
}

#endif /* (COLOR_FORMAT == GX_COLOR_FORMAT_32ARGB) */
