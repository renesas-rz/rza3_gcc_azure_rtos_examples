/* This is a small demo of the high-performance GUIX graphics framework. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "gx_api.h"
#include "SampleGrape.h"
#include "Sample_Grape_resources.h"
#include "Sample_Grape_specifications.h"

#include    "rz_a3_display_driver.h"

extern INT ActiveBtn;

#define VARIABLE_TEMP   (200.00)
#define VARIABLE_ANGLE  (270.00)
#define MIN_TEMP        (100)
#define MAX_ANGLE       (225)

VOID thermo_write(GX_ICON* icon, INT num);

#if MPU_OPERATION_CHECK
extern TX_EVENT_FLAGS_GROUP    btn_event_flags;
extern INT TEST_TOUCH;
#endif
UINT oven_event(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    //PENDOWNイベントのポインタを格納
    static GX_EVENT SlidePendownEvent;

    //DRAGイベントを複数回読み込まないようにする為に使用
    static GX_BOOL  SlideMove = GX_FALSE;

    //DRAGMOVEの戻り値を格納する変数
    INT             Result;

    //自身が今どのボタンを押下したのかを格納する変数
    static INT      ButtonPush = 0;

    //時刻格納用変数
    static INT      Min1 = 3;
    static INT      Min2 = 0;

    //RadialSliderで使用
    static INT      AngleNum;

    //現在の角度を格納する変数
    static INT      AngleNow;

    //スライダーのポインタを格納
    static GX_RADIAL_SLIDER* Slider = &Screen3_Oven.Screen3_Oven_radial_slider_1;
    GX_RADIAL_SLIDER_INFO* info = &Slider->gx_radial_slider_info;

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
            button_position(window, BUTTONWINDOW_1, &Screen3_Oven.Screen3_Oven_Button_window_1);
        }
        else 
        {
            button_position(window, BUTTONWINDOW_2, &Screen3_Oven.Screen3_Oven_Button_window_2);
        }
    
        //どのボタンが押されたかを初期化
        ButtonPush = NONE_PUSH;

        //今まで格納されていた角度を格納
        AngleNow = info->gx_radial_slider_info_current_angle;

        //まず、角度を一番左に設定
        gx_radial_slider_angle_set(Slider, info->gx_radial_slider_info_max_angle);

        //現在の角度までスライダーを動かす
        gx_radial_slider_animation_start(Slider, (GX_VALUE)AngleNow);
        break;

    case GX_EVENT_PEN_DOWN:
        //pendown時の座標へspriteを移動させ、spriteを実行する関数
        sprite_position(&Screen3_Oven.Screen3_Oven_click_sprite, event_ptr);

        //spriteをボタンの前面に出し、見栄えを良くする
        gx_widget_front_move(&Screen3_Oven.Screen3_Oven_click_sprite, 0);

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

    case GX_SIGNAL(Wash_B, GX_EVENT_CLICKED):
        ButtonPush = WASH_B_PUSH;
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
        gx_widget_back_move(&Screen3_Oven.Screen3_Oven_click_sprite, 0);
        break;
    
    //ボタン操作により時間を増やす
    case GX_SIGNAL(BU_3, GX_EVENT_CLICKED):
        //数値を増加
        Min1++;

        //5以上の数値になったら0に戻す
        if (Min1 > 5) 
        {
            Min1 = 0;
        }

        //現在の変数の数値をNumericPromptに格納
        gx_numeric_prompt_value_set(&Screen3_Oven.Screen3_Oven_Min_Prompt_1, Min1);
        break;

    case GX_SIGNAL(BU_4, GX_EVENT_CLICKED):
        //数値を増加
        Min2++;
        //9以上の数値になったら0に戻す
        if (Min2 > 9) 
        {
            Min2 = 0;
        }

        //現在の変数の数値をNumericPromptに格納
        gx_numeric_prompt_value_set(&Screen3_Oven.Screen3_Oven_Min_Prompt_2, Min2);
        break;

    //ボタン操作により時間を減らす
    case GX_SIGNAL(BD_3, GX_EVENT_CLICKED):
        //数値を減少
        Min1--;

        //0未満になったら5に戻す
        if (Min1 < 0) 
        {
            Min1 = 5;
        }

        //現在の変数の数値をNumericPromptに格納
        gx_numeric_prompt_value_set(&Screen3_Oven.Screen3_Oven_Min_Prompt_1, Min1);
        break;

    case GX_SIGNAL(BD_4, GX_EVENT_CLICKED):
        //数値を減少
        Min2--;

        //0未満になったら9に戻す
        if (Min2 < 0) 
        {
            Min2 = 9;
        }

        //現在の変数の数値をNumericPromptに格納
        gx_numeric_prompt_value_set(&Screen3_Oven.Screen3_Oven_Min_Prompt_2, Min2);
        break;

    //スライダーの値が変化するたびに下部の温度表記を計算
    case GX_SIGNAL(Thermo_Slider, GX_EVENT_SLIDER_VALUE):
        //角度を0～270にし、右に行くほど値を大きくする
        AngleNum = ((event_ptr->gx_event_payload.gx_event_intdata[0]) - MAX_ANGLE) * -1;

        //現在の角度に応じた温度の値を計算
        AngleNum = ((VARIABLE_TEMP / VARIABLE_ANGLE) * (double)AngleNum) + MIN_TEMP;

        //アイコンに適切な数値を記述
        thermo_write(&Screen3_Oven.Screen3_Oven_icon_1, (AngleNum / 100));
        thermo_write(&Screen3_Oven.Screen3_Oven_icon_2, ((AngleNum / 10) % 10));
        thermo_write(&Screen3_Oven.Screen3_Oven_icon_3, (AngleNum % 10));
        break;

#if MPU_OPERATION_CHECK
    case GX_EVENT_TIMER:
        //MPUテスト用に追加
        if (event_ptr->gx_event_payload.gx_event_timer_id == MPUTEST_TIMER)
        {
            mputest++;
            if (mputest == 5)
            {//スライダーをクリックしスライダーの動作とスライダー下部の温度が変化するかをテスト
                TEST_TOUCH = 12;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 10)
            {//+-ボタンを押して時間の増減をテスト
                TEST_TOUCH = 13;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 15)
            {//Movie画面へ遷移
                TEST_TOUCH = 14;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
                gx_system_timer_stop(&Screen3_Oven, MPUTEST_TIMER);
                mputest=0;
                movie_start_set();
            }
        }
        break;
#endif

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

//渡された数値を渡されたwidgetに格納する
VOID thermo_write(GX_ICON* icon, int num)
{

    switch (num)
    {
    case 0:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_0, GX_NULL);
        break;

    case 1:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_1, GX_NULL);
        break;

    case 2:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_2, GX_NULL);
        break;

    case 3:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_3, GX_NULL);
        break;

    case 4:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_4, GX_NULL);
        break;

    case 5:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_5, GX_NULL);
        break;

    case 6:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_6, GX_NULL);
        break;

    case 7:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_7, GX_NULL);
        break;

    case 8:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_8, GX_NULL);
        break;

    case 9:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_O_9, GX_NULL);
        break;

    default:
        break;
    }
}
