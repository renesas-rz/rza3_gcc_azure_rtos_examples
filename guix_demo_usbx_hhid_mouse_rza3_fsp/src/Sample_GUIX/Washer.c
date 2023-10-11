/* This is a small demo of the high-performance GUIX graphics framework. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "gx_api.h"
#include "SampleGrape.h"
#include "Sample_Grape_resources.h"
#include "Sample_Grape_specifications.h"

//RadialSliderで使用
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_system.h"
#include "gx_radial_slider.h"

extern INT ActiveBtn;

//現在の角度を格納する変数
static INT AngleNum;

//読み取った角度から現在の背景の表示パターンを格納
INT BackPattern = 0;

#if MPU_OPERATION_CHECK
extern TX_EVENT_FLAGS_GROUP    btn_event_flags;
extern INT TEST_TOUCH;
#endif

#define DRAW_PATTERN_HARD (1)
#define DRAW_PATTERN_SOFT (2)
#define DRAW_PATTERN_AUTO (3)
#define DRAW_PATTERN_WOOL (4)
#define DRAW_PATTERN_ECO  (5)

static UINT  pen_up_event(GX_RADIAL_SLIDER* slider, GX_EVENT* event_ptr);
static UINT  pen_drag_event(GX_RADIAL_SLIDER* slider, GX_EVENT* event_ptr);
static UINT  pen_down_event(GX_RADIAL_SLIDER* slider, GX_EVENT* event_ptr);
VOID digitalic(VOID);

UINT washer_event(GX_WINDOW* window, GX_EVENT* event_ptr) 
{
    //PENDOWNイベントのポインタを格納
    static GX_EVENT SlidePendownEvent;

    //DRAGイベントを複数回読み込まないようにする為に使用
    static GX_BOOL SlideMove = GX_FALSE;

    //DRAGMOVEの戻り値を格納する変数
    INT Result;

    //自身が今どのボタンを押下したのかを格納する変数
    static INT ButtonPush;

#if MPU_OPERATION_CHECK
    //MPUTEST用に追加
    static INT mputest;
#endif

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        //デフォルトのwindow処理を行う
        gx_window_event_process(window, event_ptr);

#if MPU_OPERATION_CHECK
        //MPUTEST用に追加
        gx_system_timer_start((GX_WIDGET*)window, MPUTEST_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);
        mputest = 0;
#endif

        //描画を行うボタングループを判定
        if (ActiveBtn == BUTTONWINDOW_1)
        {
            button_position(window, BUTTONWINDOW_1, &Screen2_Washer.Screen2_Washer_Button_window_1);
        }
        else
        {
            button_position(window, BUTTONWINDOW_2, &Screen2_Washer.Screen2_Washer_Button_window_2);
        }
        
        //どのボタンが押されたかを初期化
        ButtonPush = NONE_PUSH;
        break;

    case GX_EVENT_PEN_DOWN:
        //pendown時の座標へspriteを移動させ、spriteを実行する関数
        sprite_position(&Screen2_Washer.Screen2_Washer_click_sprite, event_ptr);

        //spriteをボタンの前面に出す
        gx_widget_front_move(&Screen2_Washer.Screen2_Washer_click_sprite, 0);

        //PENDOWN時のxy座標を求めるためにポインタを格納、ボタン群アニメーション描画時に使用
        SlidePendownEvent = *event_ptr;

        break;

    case GX_EVENT_PEN_DRAG:
        //ボタングループのアニメ―ションが許可されている場合、DRAGMOVEをコール
        if (SlideMove == GX_FALSE)
        {
            Result = drag_move(SlidePendownEvent, event_ptr, window);
            if (Result == DARG_MOVE_SUCCESS)
            {
                //ボタン群のアニメーションを2回以上再生しないように禁止
                SlideMove = GX_TRUE;
            }
        }
        break;

    case GX_EVENT_PEN_UP:
        //ボタン群のアニメーションを許可
        SlideMove = GX_FALSE;
        break;

    //各ボタンが押されると、押したボタンの番号を格納
    case GX_SIGNAL(Home_B, GX_EVENT_CLICKED):
        ButtonPush = HOME_B_PUSH;
        break;

    case GX_SIGNAL(Refri_B, GX_EVENT_CLICKED):
        ButtonPush = REFRI_B_PUSH;
        break;

    case GX_SIGNAL(Microwave_B, GX_EVENT_CLICKED):
        ButtonPush = MICROWAVE_B_PUSH;
        break;

    case GX_SIGNAL(Movie_B, GX_EVENT_CLICKED):
        ButtonPush = MOVIE_B_PUSH;
        break;

    case GX_SIGNAL(Info_B, GX_EVENT_CLICKED):
        ButtonPush = INFO_B_PUSH;
        break;

    //spriteアニメーション終了時の動作
    case GX_SIGNAL(CS_0, GX_EVENT_SPRITE_COMPLETE):
        //アニメーション描画後に適切な画面へ遷移させる
        sprite_comp_func(window, ButtonPush);

        //spriteがwidgetの前に残らないようにback_moveをコール
        gx_widget_back_move(&Screen2_Washer.Screen2_Washer_click_sprite, 0);
        break;

#if MPU_OPERATION_CHECK
    case GX_EVENT_TIMER:
        //MPUテスト用に追加
        if (event_ptr->gx_event_payload.gx_event_timer_id == MPUTEST_TIMER)
        {
            mputest++;
            if (mputest == 3)
            {//デジタリックな円形スライダーが正常に動作するかを確認
                TEST_TOUCH = 8;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 10)
            {//ラジオボタンがグループごとに使用できるか確認
                TEST_TOUCH = 9;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 15)
            {//Microwave画面へ遷移
                TEST_TOUCH = 11;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
                gx_system_timer_stop(&Screen2_Washer, MPUTEST_TIMER);
                mputest=0;
            }
        }
        break;
#endif

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

//円形スライダーのEventFunction
UINT radial_slider_function(GX_RADIAL_SLIDER* slider, GX_EVENT* event_ptr) 
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        pen_down_event(slider, event_ptr);
        gx_widget_front_move(&Screen2_Washer.Screen2_Washer_click_sprite, 0);
        sprite_position(&Screen2_Washer.Screen2_Washer_click_sprite, event_ptr);
        break;

    case GX_EVENT_PEN_DRAG:
        pen_drag_event(slider, event_ptr);
        break;

    case GX_EVENT_PEN_UP:
        pen_up_event(slider, event_ptr);

        //背景画像変更とデジタリック化を行う関数のコール
        digitalic();
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == GX_RADIAL_SLIDER_TIMER)
        {
            /* Update needle position. */
            _gx_radial_slider_animation_update(slider);
        }
        else
        {
            /* Process default event process. */
            _gx_widget_event_process((GX_WIDGET*)slider, event_ptr);
        }
        break;

    default:
        /* Process default event process. */
        return _gx_widget_event_process((GX_WIDGET*)slider, event_ptr);
    }

    return GX_SUCCESS;
}

static UINT  pen_up_event(GX_RADIAL_SLIDER* slider, GX_EVENT* event_ptr)
{
    GX_RADIAL_SLIDER_INFO* info = &slider->gx_radial_slider_info;
    GX_VALUE               new_value = 0;
    GX_POINT* point;

    if (slider->gx_widget_status & GX_STATUS_OWNS_INPUT)
    {
        _gx_system_input_release((GX_WIDGET*)slider);

        point = &event_ptr->gx_event_payload.gx_event_pointdata;

        if ((slider->gx_widget_status & GX_STATUS_TRACKING_PEN) ||
            ((GX_ABS(point->gx_point_x - slider->gx_radial_slider_pendown_point.gx_point_x) < 5) &&
                (GX_ABS(point->gx_point_y - slider->gx_radial_slider_pendown_point.gx_point_y) < 5)))
        {
            if (slider->gx_widget_status & GX_STATUS_TRACKING_PEN)
            {
                new_value = info->gx_radial_slider_info_current_angle;
            }
            else
            {
                /* Calculate new angle value with current point. */
                _gx_radial_slider_angle_calculate(slider, *point, &new_value);
            }

            _gx_radial_slider_anchor_angle_calculate(slider, &new_value);
            _gx_radial_slider_animation_start(slider, new_value);
        }

        slider->gx_widget_status |= GX_STATUS_ANIMATION_NONE;
        slider->gx_widget_status &= ~((ULONG)GX_STATUS_TRACKING_PEN);
    }
    else
    {
        _gx_widget_event_to_parent((GX_WIDGET*)slider, event_ptr);
    }
    //現在の角度をスライダーに格納
    Screen2_Washer.Screen2_Washer_radial_slider.gx_radial_slider_info.gx_radial_slider_info_current_angle = new_value;
    return GX_SUCCESS;
}

static UINT  pen_drag_event(GX_RADIAL_SLIDER* slider, GX_EVENT* event_ptr)
{
    GX_VALUE               new_value;
    GX_VALUE               previous_value;
    GX_RADIAL_SLIDER_INFO* info = &slider->gx_radial_slider_info;
    GX_BOOL                update = GX_FALSE;

    /* move my needle */
    if (slider->gx_widget_status & GX_STATUS_TRACKING_PEN)
    {
        /* Calcualte new slider value. */
        _gx_radial_slider_angle_calculate(slider, event_ptr->gx_event_payload.gx_event_pointdata, &new_value);

        /* Check if the new value is inside valide range. */
        if ((new_value >= info->gx_radial_slider_info_min_angle) &&
            (new_value <= info->gx_radial_slider_info_max_angle))
        {
            /* Calculate previous value. */
            _gx_radial_slider_angle_calculate(slider, slider->gx_radial_slider_pendown_point, &previous_value);

            if ((previous_value < new_value) && ((new_value - previous_value) < 180))
            {
                //anti-clockwise
                if (info->gx_radial_slider_info_current_angle < new_value)
                {
                    update = GX_TRUE;
                }
            }
            else if (previous_value > new_value)
            {
                //clockwise
                if (info->gx_radial_slider_info_current_angle > new_value)
                {
                    update = GX_TRUE;
                }
            }

            if (update)
            {
                _gx_radial_slider_angle_set(slider, new_value);
            }
        }

        slider->gx_radial_slider_pendown_point = event_ptr->gx_event_payload.gx_event_pointdata;
    }
    else
    {
        _gx_widget_event_to_parent((GX_WIDGET*)slider, event_ptr);
    }

    return GX_SUCCESS;
}

static UINT  pen_down_event(GX_RADIAL_SLIDER* slider, GX_EVENT* event_ptr)
{
    INT                    xpos;
    INT                    ypos;
    INT                    dist;
    GX_RADIAL_SLIDER_INFO* info = &slider->gx_radial_slider_info;
    GX_POINT* point;
    GX_RECTANGLE           rectangle;

    if ((slider->gx_widget_style & GX_STYLE_ENABLED) &&
        (slider->gx_widget_status & GX_STATUS_ANIMATION_NONE))
    {
        point = &event_ptr->gx_event_payload.gx_event_pointdata;

        /* Record pen down point. */
        slider->gx_radial_slider_pendown_point = *point;

        /* Get current needle bounding rectangle. */
        _gx_radial_slider_needle_rectangle_calculate(slider, &rectangle);

        /* Check if click point is inside needle area. */
        if (_gx_utility_rectangle_point_detect(&rectangle, *point))
        {
            slider->gx_widget_status |= GX_STATUS_TRACKING_PEN;

            _gx_system_input_capture((GX_WIDGET*)slider);
        }
        else
        {
            /* Check if the click down point is inside tracking area. */
            xpos = point->gx_point_x - slider->gx_widget_size.gx_rectangle_left;
            ypos = point->gx_point_y - slider->gx_widget_size.gx_rectangle_top;


            /* Calculate the distacnce from click point to tracking center. */
            dist = (INT)_gx_utility_math_sqrt((UINT)((xpos - info->gx_radial_slider_info_xcenter) * (xpos - info->gx_radial_slider_info_xcenter) +
                (ypos - info->gx_radial_slider_info_ycenter) * (ypos - info->gx_radial_slider_info_ycenter)));

            if ((info->gx_radial_slider_info_radius - info->gx_radial_slider_info_track_width / 2 <= dist) &&
                (dist <= info->gx_radial_slider_info_radius + info->gx_radial_slider_info_track_width / 2))
            {
                /* Click point is inside tracking area.  */
                _gx_system_input_capture((GX_WIDGET*)slider);
            }
        }
    }
    else
    {
        _gx_widget_event_to_parent((GX_WIDGET*)slider, event_ptr);
    }

    return GX_SUCCESS;
}

//円形スライダーのデジタリック化＋角度に応じた背景変更処理
VOID digitalic(VOID) 
{
    //スライダーのポインタを格納
    static GX_RADIAL_SLIDER* slider_1 = &Screen2_Washer.Screen2_Washer_radial_slider;

    //スライダーの角度抽出時に使用
    GX_RADIAL_SLIDER_INFO* info_1 = &slider_1->gx_radial_slider_info;

    //操作終了時の角度をAngleNumへ格納
    AngleNum = info_1->gx_radial_slider_info_current_angle;

    //AngleNumの値で設定する角度と背景画像を決定
    if (AngleNum < 0) 
    {
        gx_radial_slider_animation_start(slider_1, -30);
        BackPattern = DRAW_PATTERN_HARD;
    }
    else if (AngleNum >= 0 && AngleNum < 60)
    {
        gx_radial_slider_animation_start(slider_1, 30);
        BackPattern = DRAW_PATTERN_SOFT;
    }
    else if (AngleNum >= 60 && AngleNum < 120)
    {
        gx_radial_slider_animation_start(slider_1, 90);
        BackPattern = DRAW_PATTERN_AUTO;
    }
    else if (AngleNum >= 120 && AngleNum < 180)
    {
        gx_radial_slider_animation_start(slider_1, 150);
        BackPattern = DRAW_PATTERN_WOOL;
    }
    else if (AngleNum >= 180)
    {
        gx_radial_slider_animation_start(slider_1, 210);
        BackPattern = DRAW_PATTERN_ECO;
    }

    //背景画像を再描画
    gx_system_dirty_mark(&Screen2_Washer.Screen2_Washer_circledraw_window);
}

//円形スライダーの背景変更用描画関数
VOID circle_draw(GX_WINDOW* window) 
{
    GX_PIXELMAP* map = GX_NULL;

    //windowを描画
    gx_window_draw(window);

    //digitalic関数で指定された背景を実際に描く
    switch (BackPattern)
    {
    case DRAW_PATTERN_HARD:
        gx_widget_pixelmap_get(window, GX_PIXELMAP_ID_CIRCLE_LABEL_HARD, &map);
        break;

    case DRAW_PATTERN_SOFT:
        gx_widget_pixelmap_get(window, GX_PIXELMAP_ID_CIRCLE_LABEL_SOFT, &map);
        break;

    case DRAW_PATTERN_AUTO:
        gx_widget_pixelmap_get(window, GX_PIXELMAP_ID_CIRCLE_LABEL_AUTO, &map);
        break;

    case DRAW_PATTERN_WOOL:
        gx_widget_pixelmap_get(window, GX_PIXELMAP_ID_CIRCLE_LABEL_WOOL, &map);
        break;

    case DRAW_PATTERN_ECO:
        gx_widget_pixelmap_get(window, GX_PIXELMAP_ID_CIRCLE_LABEL_ECO, &map);
        break;

    default:
        gx_widget_pixelmap_get(window, GX_PIXELMAP_ID_CIRCLE_LABEL_AUTO, &map);
        break;
    }

    //windowの指定された座標へ先ほど格納した画像を描画
    gx_canvas_pixelmap_draw(60, 105, map);

    //windowの子widgetを描画
    gx_widget_children_draw(window);
}
