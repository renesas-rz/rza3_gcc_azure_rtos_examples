/* This is a small demo of the high-performance GUIX graphics framework. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "gx_api.h"
#include "SampleGrape.h"
#include "Sample_Grape_resources.h"
#include "Sample_Grape_specifications.h"

#include    "rz_a3_display_driver.h"

#define SCREEN_TAP      (0)
#define SCREEN_NONTAP   (1)

extern INT ActiveBtn;
extern INT ChangeAnimation;

#if MPU_OPERATION_CHECK
extern TX_EVENT_FLAGS_GROUP    btn_event_flags;
extern INT TEST_TOUCH;
#endif

VOID movie_draw(GX_WINDOW* window);

UINT movie_event(GX_WINDOW* window, GX_EVENT* event_ptr) 
{
    //動画開始
    //movie_start_set();

    //PENDOWNイベントのポインタを格納
    static GX_EVENT SlidePendownEvent;
    
    //DRAGイベントを複数回読み込まないようにする為に使用
    static GX_BOOL SlideMove = GX_FALSE;
    
    //DRAGMOVEの戻り値を格納する変数
    INT Result;
    
    //自身が今どのボタンを押下したのかを格納する変数
    static INT ButtonPush;
    
    //タップ時の動作制御に使用
    static INT Tap;

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
    
        //デフォルトの描画関数を設定して裏のレイヤのみを表示
        gx_widget_draw_set(window, _gx_window_draw);
        
        //現在のウインドウを再描画
        gx_system_dirty_mark(window);
        
        //描画を行うボタングループを判定
        if (ActiveBtn == BUTTONWINDOW_1)
        {
            button_position(window, BUTTONWINDOW_1, &Screen4_Movie.Screen4_Movie_Button_window_1);
        }
        else 
        {
            button_position(window, BUTTONWINDOW_2, &Screen4_Movie.Screen4_Movie_Button_window_2);
        }
        
        //どのボタンが押されたかを初期化
        ButtonPush = NONE_PUSH;
        
        //最初に画面内のwidgetをスワイプアウト
        swipe_event(window, SWIPEOUT_EVENT);
        
        //TapにSCREEN_NONTAPを格納し、次回画面タッチ時にスワイプインできるようにする
        Tap = SCREEN_NONTAP;
        break;
    
    case GX_EVENT_PEN_DOWN:
        //pendown時の座標へspriteを移動させ、spriteを実行する関数
        sprite_position(&Screen4_Movie.Screen4_Movie_click_sprite, event_ptr);
        
        //PENDOWN時のxy座標を求めるためにポインタを格納、ボタン群アニメーション描画時に使用
        SlidePendownEvent = *event_ptr;
        
        //TapがSCREEN_NONTAPでかつ、アニメーションが許可されている場合
        if (Tap == SCREEN_NONTAP && ChangeAnimation == DRAG_ENABLE)
        {
            //画面内にwidgetが見えている
            swipe_event(window, SWIPEIN_EVENT);

            //タップされた状態になることで、次回タップ時は本イベントには入らないようになる
            Tap = SCREEN_TAP;
            
            //一度タップをされたら、半透明の背景を描画
            gx_widget_draw_set(window, movie_draw);
        }
        
        //現在のウインドウを再描画
        gx_system_dirty_mark(window);
        break;

    case GX_EVENT_PEN_DRAG:
        //ボタングループのアニメ―ションが許可されている場合、DRAGMOVEをコール
        if (SlideMove == GX_FALSE && ChangeAnimation == DRAG_ENABLE) 
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
    
    case GX_SIGNAL(Microwave_B, GX_EVENT_CLICKED):
        ButtonPush = MICROWAVE_B_PUSH;
        break;
    
    case GX_SIGNAL(Info_B, GX_EVENT_CLICKED):
        ButtonPush = INFO_B_PUSH;
        break;

    //spriteアニメーション終了時の動作
    case GX_SIGNAL(CS_0, GX_EVENT_SPRITE_COMPLETE):
        //アニメーション描画後に適切な画面へ遷移させる
        sprite_comp_func(window, ButtonPush);

        //spriteがwidgetの前に残らないようにback_moveをコール
        gx_widget_back_move(&Screen4_Movie.Screen4_Movie_click_sprite, 0);
        break;
    
    //特定のアニメーション終了後に画面下部のボタングループ切り替えアニメーションを許可する
    case GX_EVENT_ANIMATION_COMPLETE:
        switch (event_ptr->gx_event_sender)
        {
        case TIME_COMP_OUT:
            ChangeAnimation = DRAG_ENABLE;
            break;
    
        case TIME_COMP_IN:
            ChangeAnimation = DRAG_ENABLE;
            break;
        }
        break;

#if MPU_OPERATION_CHECK
        case GX_EVENT_TIMER:
        //MPUテスト用に追加
        if (event_ptr->gx_event_payload.gx_event_timer_id == MPUTEST_TIMER)
        {
            mputest++;
            if (mputest == 5)
            {//画面中心をタッチしてスワイプインイベントを起こす
                TEST_TOUCH = 15;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 10)
            {//右側へドラッグしてボタングループを切り替え
                TEST_TOUCH = 1;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 15)
            {//information画面へ遷移
                TEST_TOUCH = 3;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
                gx_system_timer_stop(&Screen4_Movie, MPUTEST_TIMER);
                mputest=0;
                movie_stop_set();
            }
        }
        break;
#endif

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

//本画面の描画関数
VOID movie_draw(GX_WINDOW* window)
{
    GX_PIXELMAP* map = GX_NULL;

    //windowを描画
    gx_window_draw(window);

    //画像とウインドウのポインタ取得
    gx_widget_pixelmap_get(window, GX_PIXELMAP_ID_BACKGROUND, &map);
    
    //描画
    gx_canvas_pixelmap_blend(0, 0, map, 128);
    
    //子widgetを描画
    gx_widget_children_draw(window);
}

