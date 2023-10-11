/* This is a small demo of the high-performance GUIX graphics framework. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "gx_api.h"
#include "SampleGrape.h"
#include "Sample_Grape_resources.h"
#include "Sample_Grape_specifications.h"

#include    "rz_a3_display_driver.h"

extern INT ActiveBtn;

#if MPU_OPERATION_CHECK
extern TX_EVENT_FLAGS_GROUP    btn_event_flags;
extern INT TEST_TOUCH;
#endif
UINT information_event(GX_WINDOW* window, GX_EVENT* event_ptr) 
{

    //動画停止
    //movie_stop_set();

    //PENDOWNイベントのポインタを格納
    static GX_EVENT SlidePendownEvent;

    //DRAGイベントを複数回読み込まないようにする為に使用
    static GX_BOOL SlideMove = GX_FALSE;

    //DRAGMOVEの戻り値を格納する変数
    INT result;

    //自身が今どのボタンを押下したのかを格納する変数
    static INT ButtonPush = 0;

#if MPU_OPERATION_CHECK
    //MPUTEST用に追加
    static INT mputest;
#endif

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
#if MPU_OPERATION_CHECK
        //MPUTEST用に追加
        gx_system_timer_start((GX_WIDGET*)window, MPUTEST_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);
        mputest = 0;
#endif

        //デフォルトのwindow処理を行う
        gx_window_event_process(window, event_ptr);

        //描画を行うボタングループを判定
        if (ActiveBtn == BUTTONWINDOW_1)
        {
            button_position(window, BUTTONWINDOW_1, &Screen5_Information.Screen5_Information_Button_window_1);
        }
        else 
        {
            button_position(window, BUTTONWINDOW_2, &Screen5_Information.Screen5_Information_Button_window_2);
        }
        
        //どのボタンが押されたかを初期化
        ButtonPush = NONE_PUSH;
        break;

    case GX_EVENT_PEN_DOWN:
        //pendown時の座標へspriteを移動させ、spriteを実行する関数
        sprite_position(&Screen5_Information.Screen5_Information_click_sprite, event_ptr);

        //PENDOWN時のxy座標を求めるためにポインタを格納、ボタン群アニメーション描画時に使用
        SlidePendownEvent = *event_ptr;
        break;
    
    case GX_EVENT_PEN_DRAG:
        //ボタングループのアニメ―ションが許可されている場合、DRAGMOVEをコール
        if (SlideMove == GX_FALSE) 
        {
            result = drag_move(SlidePendownEvent, event_ptr, window);
            if (result == DARG_MOVE_SUCCESS) 
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

    case GX_SIGNAL(Microwave_B, GX_EVENT_CLICKED):
        ButtonPush = MICROWAVE_B_PUSH;
        break;

    case GX_SIGNAL(Movie_B, GX_EVENT_CLICKED):
        ButtonPush = MOVIE_B_PUSH;
        break;

    //spriteアニメーション終了時の動作
    case GX_SIGNAL(CS_0, GX_EVENT_SPRITE_COMPLETE):
        //ボタンが押されている場合、アニメーション描画後に適切な画面へ遷移させる
        sprite_comp_func(window, ButtonPush);

        //spriteがwidgetの前に残らないようにback_moveをコール
        gx_widget_back_move(&Screen5_Information.Screen5_Information_click_sprite, 0);
        break;

#if MPU_OPERATION_CHECK
    case GX_EVENT_TIMER:
        //MPUテスト用に追加
        if (event_ptr->gx_event_payload.gx_event_timer_id == MPUTEST_TIMER)
        {
            mputest++;
            if (mputest == 5)
            {//左側へドラッグしてボタングループを切り替え
                TEST_TOUCH = 2;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 10)
            {//ホームボタンを押してホームへ遷移
                TEST_TOUCH = 16;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
                gx_system_timer_stop(&Screen5_Information, MPUTEST_TIMER);
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
