/* This is a small demo of the high-performance GUIX graphics framework. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "gx_api.h"
#include "SampleGrape.h"
#include "Sample_Grape_resources.h"
#include "Sample_Grape_specifications.h"

#include "rz_a3_display_driver.h"
#include "gx_slider.h"
#include "gx_widget.h"

extern INT ActiveBtn;
INT Slider2Pendown;
#if MPU_OPERATION_CHECK
extern TX_EVENT_FLAGS_GROUP    btn_event_flags;
extern INT TEST_TOUCH;
#endif

#define NOT_PRESSED             (0)
#define PRESSED                 (1)

VOID slider_action(GX_PIXELMAP_SLIDER* slider, GX_SLIDER_INFO* info);
UINT digitalic_function(GX_PIXELMAP_SLIDER* slider, GX_EVENT* event_ptr);

UINT refrigerator_event(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    //PENDOWNイベントのポインタを格納
    static GX_EVENT SlidePendownEvent;

    //DRAGイベントを複数回読み込まないようにする為に使用
    static GX_BOOL SlideMove = GX_FALSE;

    //DRAGMOVEの戻り値を格納する変数
    INT Result;

    //自身が今どのボタンを押下したのかを格納する変数
    static INT ButtonPush;

    //Slider1の現在値を格納する変数
    INT Value;

    //バグ防止のPEN_DOWN用変数
    static INT WindowPendown = NOT_PRESSED;

    //スライダーのinfoを格納するのに使用
    GX_SLIDER_INFO* info;

#if MPU_OPERATION_CHECK
    //MPUTEST用に追加
    static int mputest;
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
            button_position(window, BUTTONWINDOW_1, &Screen1_Refrigerator.Screen1_Refrigerator_Button_window_1);
        }
        else 
        {
            button_position(window, BUTTONWINDOW_2, &Screen1_Refrigerator.Screen1_Refrigerator_Button_window_2);
        }

        //どのボタンが押されたかを初期化
        ButtonPush = NONE_PUSH;
        
        //SHOW時にこの変数を初期化
        Slider2Pendown = NOT_PRESSED;
        break;
    
    case GX_EVENT_PEN_DOWN:
        //pendown時の座標へspriteを移動させ、spriteを実行する関数
        sprite_position(&Screen1_Refrigerator.Screen1_Refrigerator_click_sprite, event_ptr);
    
        //PENDOWN時のxy座標を求めるためにポインタを格納、ボタン群アニメーション描画時に使用
        SlidePendownEvent = *event_ptr;
        
        //別のwidgetからドラッグを行った際にDRAG関数が暴発しないように使用
        //このwidget内でPEN_DOWNが行われたので、DRAG関数のコールを許可
        WindowPendown = PRESSED;
        break;
    
    case GX_EVENT_PEN_DRAG:
        //ボタングループのアニメ―ションが許可されている場合、DRAGMOVEをコール
        if (SlideMove == GX_FALSE && WindowPendown == PRESSED) 
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
        
        //Window_PENDOWNを初期化する
        WindowPendown = NOT_PRESSED;
        
        if (Slider2Pendown == PRESSED)
        {
            //2つ目のスライダーから手を離した際の動きを記述
            info = &Screen1_Refrigerator.Screen1_Refrigerator_pixelmap_slider_1.gx_slider_info;
            slider_action(&Screen1_Refrigerator.Screen1_Refrigerator_pixelmap_slider_1, info);

            //変数を使用したので、初期値に戻す
            Slider2Pendown = NOT_PRESSED;
        }
        break;

    //各ボタンが押されると、押したボタンの番号を格納
    case GX_SIGNAL(Home_B, GX_EVENT_CLICKED):
        ButtonPush = HOME_B_PUSH;
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
            if (mputest == 5)
            {//通常のスライダーのテスト
                TEST_TOUCH = 4;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 12)
            {//デジタリックなスライダーのテスト
                TEST_TOUCH = 5;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 20)
            {//スライダー風ラジオボタンのテスト
                TEST_TOUCH = 6;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 25)
            {//Washer画面へ遷移
                TEST_TOUCH = 7;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
                gx_system_timer_stop(&Screen1_Refrigerator, MPUTEST_TIMER);
                mputest=0;
            }
        }
        break;
#endif
    //1つ目のスライダーの温度表記変更時に使用
    case GX_SIGNAL(Slider1, GX_EVENT_SLIDER_VALUE):
        Value = Screen1_Refrigerator.Screen1_Refrigerator_pixelmap_slider.gx_slider_info.gx_slider_info_current_val;
    
        if (Value < 20) 
        {
            gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider1_icon, GX_PIXELMAP_ID_R_1, GX_NULL);
        }
        else if (20 <= Value && Value < 40) 
        {
            gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider1_icon, GX_PIXELMAP_ID_R_2, GX_NULL);
        }
        else if (40 <= Value && Value < 60) 
        {
            gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider1_icon, GX_PIXELMAP_ID_R_3, GX_NULL);
        }
        else if (60 <= Value && Value < 80) 
        {
            gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider1_icon, GX_PIXELMAP_ID_R_4, GX_NULL);
        }
        else 
        {
            gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider1_icon, GX_PIXELMAP_ID_R_5, GX_NULL);
        }
        break;
    
    //3つ目のスライダー風ラジオボタンの温度表記変更時に使用
    case GX_SIGNAL(RB_1,GX_EVENT_RADIO_SELECT):
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider3_icon, GX_PIXELMAP_ID_R_3, GX_NULL);
        break;
    
    case GX_SIGNAL(RB_2, GX_EVENT_RADIO_SELECT):
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider3_icon, GX_PIXELMAP_ID_R_4, GX_NULL);
        break;
    
    case GX_SIGNAL(RB_3, GX_EVENT_RADIO_SELECT):
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider3_icon, GX_PIXELMAP_ID_R_5, GX_NULL);
        break;
    
    case GX_SIGNAL(RB_4, GX_EVENT_RADIO_SELECT):
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider3_icon, GX_PIXELMAP_ID_R_6, GX_NULL);
        break;
    
    case GX_SIGNAL(RB_5, GX_EVENT_RADIO_SELECT):
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider3_icon, GX_PIXELMAP_ID_R_7, GX_NULL);
        break;
    
    default:
        return gx_window_event_process(window, event_ptr);
    }
    
    return 0;
}


//2つ目のスライダーの温度表記変更+デジタリック化処理
VOID slider_action(GX_PIXELMAP_SLIDER* slider, GX_SLIDER_INFO* info)
{
    INT value;

    //現在の角度を変数valueに格納
    value = slider->gx_slider_info.gx_slider_info_current_val;

    //現在の角度に適した範囲内だった場合、既定の値をスライダーへ格納し、適切な温度表記を行う
    if (value < 20)
    {
        gx_slider_value_set((GX_SLIDER *)slider, info, 0);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_2, GX_PIXELMAP_ID_R_2, GX_NULL);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_3, GX_PIXELMAP_ID_R_0, GX_NULL);
    }
    else if (20 <= value && value < 60)
    {
        gx_slider_value_set((GX_SLIDER *)slider, info, 40);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_2, GX_PIXELMAP_ID_R_1, GX_NULL);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_3, GX_PIXELMAP_ID_R_9, GX_NULL);
    }
    else if (60 <= value && value < 100)
    {
        gx_slider_value_set((GX_SLIDER *)slider, info, 80);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_2, GX_PIXELMAP_ID_R_1, GX_NULL);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_3, GX_PIXELMAP_ID_R_8, GX_NULL);
    }
    else if (100 <= value && value < 140)
    {
        gx_slider_value_set((GX_SLIDER *)slider, info, 120);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_2, GX_PIXELMAP_ID_R_1, GX_NULL);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_3, GX_PIXELMAP_ID_R_7, GX_NULL);
    }
    else
    {
        gx_slider_value_set((GX_SLIDER *)slider, info, 160);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_2, GX_PIXELMAP_ID_R_1, GX_NULL);
        gx_icon_pixelmap_set(&Screen1_Refrigerator.Screen1_Refrigerator_Slider2_icon_3, GX_PIXELMAP_ID_R_6, GX_NULL);
    }
}

//2つ目のスライダーのEventFunction
UINT digitalic_function(GX_PIXELMAP_SLIDER* slider, GX_EVENT* event_ptr)
{
    UINT status;
    INT  pos;

    //スライダーのinfoを格納するのに使用
    GX_SLIDER_INFO* info;

    //一度でもPEN_DOWNされた場合に格納される変数を用意
    //static INT down = 0;

    /* Default status to success.  */
    status = GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:

        //Slider2でPEN_DOWNされた事を通知する為の変数
        Slider2Pendown = PRESSED;

        /* Set style for pen down.  */
        slider->gx_widget_style |= GX_STYLE_BUTTON_PUSHED;

        if (slider->gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
        {
            pos = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y;
        }
        else
        {
            pos = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
        }

        _gx_slider_value_calculate((GX_SLIDER*)slider, &slider->gx_slider_info, pos);
        sprite_position(&Screen1_Refrigerator.Screen1_Refrigerator_click_sprite, event_ptr);
        gx_widget_back_move(&Screen1_Refrigerator.Screen1_Refrigerator_click_sprite, 0);

        break;

    case GX_EVENT_PEN_UP:

        /* Set style for pen down.  */
        slider->gx_widget_style &= ~((ULONG)GX_STYLE_BUTTON_PUSHED);

        //widgetから手を離した際の機能をコール
        info = &Screen1_Refrigerator.Screen1_Refrigerator_pixelmap_slider_1.gx_slider_info;
        slider_action(&Screen1_Refrigerator.Screen1_Refrigerator_pixelmap_slider_1, info);
        break;

    case GX_EVENT_PEN_DRAG:
        /* move my needle */
        if (slider->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            if (slider->gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
            {
                pos = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y;
            }
            else
            {
                pos = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
            }

            _gx_slider_value_calculate((GX_SLIDER*)slider, &slider->gx_slider_info, pos);
        }
        break;

    case GX_EVENT_SHOW:
        status = _gx_widget_event_process((GX_WIDGET*)slider, event_ptr);
        _gx_pixelmap_slider_pixelmap_update(slider);
        break;

    default:

        /* Call the widget default processing.  */
        status = _gx_widget_event_process((GX_WIDGET*)slider, event_ptr);
    }

    /* Return completion status.  */
    return(status);
}
