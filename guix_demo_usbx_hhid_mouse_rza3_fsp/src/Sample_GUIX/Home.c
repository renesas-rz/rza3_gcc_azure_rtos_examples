/* This is a small demo of the high-performance GUIX graphics framework. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "gx_api.h"
#include "SampleGrape.h"
#include "Sample_Grape_resources.h"
#include "Sample_Grape_specifications.h"

#include    "rz_a3_display_driver.h"

VOID clock_update(RTCDATE_TIME* rtc);
VOID rtc_update(RTCDATE_TIME* rtc);
VOID date_write(RTCDATE_TIME* rtc);
VOID week_set(GX_ICON* icon, INT num);
VOID date_set(GX_ICON* icon, INT num);
VOID date_set_b(GX_ICON* icon, int num);
VOID popup_move(INT move);

#define RTC_UNENABLE    (1)
#define RTC_ENABLE      (2)
#define POPUP_OFF       (1)
#define POPUP_ON        (2)
#define POPUP_TIME      (10)

extern INT ActiveBtn;
extern INT ChangeAnimation;
extern RTCDATE_TIME RtcDate;
#if MPU_OPERATION_CHECK
extern TX_EVENT_FLAGS_GROUP    btn_event_flags;
extern INT TEST_TOUCH;
#endif
UINT home_event(GX_WINDOW* window, GX_EVENT* event_ptr)
{

    //PENDOWNイベントのポインタを格納
    static GX_EVENT SlidePendownEvent;

    //DRAGイベントを複数回読み込まないようにする為に使用
    static GX_BOOL SlideMove = GX_FALSE;

    //DRAGMOVEの戻り値を格納する変数
    INT Result;

    //自身が今どのボタンを押下したのかを格納する変数
    static INT ButtonPush = 0;

    //ポップアップ画面の出力に使用
    static INT PopupCount = 0;
    static GX_BOOL OperatingFlag;
    static GX_BOOL PenupFlag;

#if MPU_OPERATION_CHECK
    /* Used in MPU operation check */
    static INT mputest = 0;
#endif


    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        //デフォルトのwindow処理を行う
        gx_window_event_process(window, event_ptr);

        //時刻情報の初期化
        clock_update(&RtcDate);

        //時計用のタイマを動かすのに使用
        gx_system_timer_start((GX_WIDGET*)window, CLOCK_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);

        //ポップアップ画面出力用タイマを動かすのに使用
        gx_system_timer_start((GX_WIDGET*)window, POPUP_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);

        //画面が操作されているか等を管理するフラグの初期化
        OperatingFlag = GX_FALSE;
        PenupFlag = GX_FALSE;

#if MPU_OPERATION_CHECK
        /* Start of timer for MPU operation check */
        gx_system_timer_start((GX_WIDGET*)window, MPUTEST_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);

        /* Initialization of variable mputest for timer count */
        mputest = 0;
#endif

        //描画を行うボタングループを判定
        if (ActiveBtn == BUTTONWINDOW_1)
        {
            button_position(window, BUTTONWINDOW_1, &Screen0_Home.Screen0_Home_Button_window_1);
        }
        else 
        {
            button_position(window, BUTTONWINDOW_2, &Screen0_Home.Screen0_Home_Button_window_2);
        }

        //どのボタンが押されたかを初期化
        ButtonPush = NONE_PUSH;

        //画面遷移時に画面をスワイプイン
        swipe_event(window, SWIPEIN_EVENT);
        break;

    case GX_EVENT_PEN_DOWN:
        //pendown時の座標へspriteを移動させ、spriteを実行する関数
        sprite_position(&Screen0_Home.Screen0_Home_click_sprite, event_ptr);

        //PENDOWN時のxy座標を求めるためにポインタを格納、ボタン群アニメーション描画時に使用
        SlidePendownEvent = *event_ptr;

        //ポップアップ画面を出すカウントを禁止にする
        OperatingFlag = GX_TRUE;
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

        //PENUPされたことを伝達
        if(OperatingFlag == GX_TRUE)
        {
            PenupFlag = GX_TRUE;
        }
        break;

    //各ボタンが押されると、押したボタンの番号をButtonPushに格納
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

    case GX_SIGNAL(Info_B, GX_EVENT_CLICKED):
        ButtonPush = INFO_B_PUSH;
        break;

    //spriteアニメーション終了時の動作
    case GX_SIGNAL(CS_0, GX_EVENT_SPRITE_COMPLETE):
        if (ButtonPush != NONE_PUSH)
        {
            //タイマを停止
            gx_system_timer_stop(window, POPUP_TIMER);

            //ポップアップ画面を画面外へ移動
            popup_move(POPUP_OFF);

            //カウントの初期化
            PopupCount = 0;

            //アニメーション描画後に適切な画面へ遷移させる
            sprite_comp_func(window, ButtonPush);
        }

        //spriteがwidgetの前に残らないようにback_moveをコール
        gx_widget_back_move(&Screen2_Washer.Screen2_Washer_click_sprite, 0);
        break;

    //時刻のスワイプインアニメーションが終了したらボタングループ切り替えアニメーションを許可
    case GX_EVENT_ANIMATION_COMPLETE:
        switch (event_ptr->gx_event_sender) 
        {
        case TIME_COMP:
            ChangeAnimation = DRAG_ENABLE;
            break;
        }
        break;

    case GX_EVENT_TIMER:
        //1秒ごとに時刻更新関数をコールする
        if (event_ptr->gx_event_payload.gx_event_timer_id == CLOCK_TIMER)
        {
            clock_update(&RtcDate);
        }

        //ポップアップ画面を出すカウントを行う
        if (event_ptr->gx_event_payload.gx_event_timer_id == POPUP_TIMER)
        {
            //カウントが許可されている場合はインクリメント
            if (OperatingFlag == GX_FALSE)
            {
                PopupCount++;
            }
            //されていない場合はカウントを0に
            else
            {
                //PEN_UPイベント後の場合、フラグをリセットし操作状態を解除
                if (PenupFlag == GX_TRUE)
                {
                    OperatingFlag = GX_FALSE;
                    PenupFlag = GX_FALSE;
                }

                PopupCount = 0;
            }

            //カウント用変数が定義POPUP_TIMEよりも大きい値になったらポップアップ画面を出力
            if (PopupCount > POPUP_TIME)
            {
                //カウントの初期値へ
                PopupCount = 0;

                //タイマを停止
                gx_system_timer_stop(window, POPUP_TIMER);

                //ポップアップ画面を最上面へ
                gx_widget_front_move(&Screen0_Home.Screen0_Home_popup_window, 0);

                //ポップアップ画面を画面内に出す
                popup_move(POPUP_ON);
            }
        }
#if MPU_OPERATION_CHECK
        /* Used in MPU operation check */
        if (event_ptr->gx_event_payload.gx_event_timer_id == MPUTEST_TIMER)
        {
            mputest++;
            if (mputest == 3)
            {//右側へのボタングループ切り替え処理をテスト
                TEST_TOUCH = 1;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 8)
            {//左側へのボタングループ切り替え処理をテスト
                TEST_TOUCH = 2;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
            }
            if (mputest == 13)
            {//Refrigerator画面へ遷移
                TEST_TOUCH = 3;
                tx_event_flags_set(&btn_event_flags, 0x1, TX_OR);
                //使用していたタイマを停止
                gx_system_timer_stop(&Screen0_Home, MPUTEST_TIMER);
                mputest=0;
                movie_stop_set();
            }
        }
#endif
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

//ポップアップ画面のwindowに紐づけたEventFunction
UINT popup_function(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        break;

    case GX_EVENT_PEN_DOWN:
        //ポップアップ画面を画面外へ
        popup_move(POPUP_OFF);

        //ポップアップ用のタイマを開始
        gx_system_timer_start(&Screen0_Home, POPUP_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

//ポップアップ画面を移動させる関数
VOID popup_move(INT move)
{
    GX_RECTANGLE NewSize;

    //ポップアップ画面を画面内へ表示させるか否かを決める
    switch (move)
    {
    case POPUP_OFF:
        gx_utility_rectangle_define(&NewSize, 600, 600, 1100, 800);
        break;

    case POPUP_ON:
        gx_utility_rectangle_define(&NewSize, 100, 600, 600, 800);
        break;

    default:
        break;
    }
    gx_widget_resize(&Screen0_Home.Screen0_Home_popup_window, &NewSize);

}

//時刻の初期化と時刻更新関数・表示更新関数をコール
VOID clock_update(RTCDATE_TIME* rtc)
{
    //時刻の取得方法を格納
    //RTC_unenable→自身で時刻を設定
    //RTC_enable→RTCから時刻を取得(未実装)
    UINT TimeGet = RTC_UNENABLE;

    //タイマーの初期値を格納する際に使用
    static INT InitComp = 0;

    //時刻を取得し、構造体を初期化
    switch (TimeGet)
    {
    case RTC_UNENABLE:
        if (InitComp == 0)
        {
            //RTC無しの状態で動かす場合
            rtc->Year = 2022;
            rtc->Month = 4;
            rtc->Day = 30;
            rtc->DayOfWeek = 6;
            rtc->Hour = 23;
            rtc->Minute = 59;
            rtc->Second = 40;
            rtc->Milliseconds = 0;
            InitComp = 1;
        }

        //時刻の更新
        rtc_update(rtc);

        break;

    case RTC_ENABLE:
        //RTC有りの状態で動かす場合
        break;

    default:
        break;
    }

    //時刻を書き込む用の関数をコール
    date_write(rtc);
}

//時刻更新用関数
VOID rtc_update(RTCDATE_TIME* rtc)
{
    //秒数をカウント
    rtc->Second += 1;

    //秒数が60以上なら分を進めて秒数をリセット
    if (rtc->Second > 59) 
    {
        rtc->Minute += 1;
        rtc->Second = 0;
    }

    //分数が60以上なら時を進めて分数をリセット
    if (rtc->Minute > 59) 
    {
        rtc->Hour += 1;
        rtc->Minute = 0;
    }

    //時が24なら日と曜日を進めて時をリセット　土曜日の場合は曜日の数値をリセットする
    if (rtc->Hour > 23) 
    {
        rtc->Day += 1;
        rtc->DayOfWeek += 1;
        if (rtc->DayOfWeek > 6) 
        {
            rtc->DayOfWeek = 0;
        }
        rtc->Hour = 0;
    }

    //年と月を見て、それぞれ対応
    //31日まである月の場合
    if ((rtc->Month == 1 || rtc->Month == 3 || rtc->Month == 5 || rtc->Month == 7 ||
        rtc->Month == 8 || rtc->Month == 10 || rtc->Month == 12) && rtc->Day > 31) 
    {
        rtc->Month += 1;
        if (rtc->Month > 12) 
        {
            rtc->Year += 1;
            rtc->Month = 1;
        }
        rtc->Day = 1;
    }

    //30日まである2月以外の月の場合
    if ((rtc->Month == 4 || rtc->Month == 6 || rtc->Month == 9 || rtc->Month == 11) && rtc->Day > 30) 
    {
        rtc->Month += 1;
        rtc->Day = 1;
    }

    //2月で28日または30日の場合
    if (rtc->Month == 2) 
    {
        if (rtc->Day > 28 && rtc->Year % 4 == 0) 
        {
            rtc->Month += 1;
            rtc->Day = 1;
        }
        else if (rtc->Day > 30) 
        {
            rtc->Month += 1;
            rtc->Day = 1;
        }
    }
}

//時刻表示更新関数
VOID date_write(RTCDATE_TIME* rtc) 
{
    //年月日曜日時分秒の順番で更新を行う
    //Home画面
    date_set(&Screen0_Home.Screen0_Home_Year_3, ((rtc->Year) % 100) / 10);
    date_set(&Screen0_Home.Screen0_Home_Year_4, ((rtc->Year) % 100) % 10);
    date_set(&Screen0_Home.Screen0_Home_Month_1, ((rtc->Month) / 10));
    date_set(&Screen0_Home.Screen0_Home_Month_2, ((rtc->Month) % 10));
    date_set(&Screen0_Home.Screen0_Home_Day_1, ((rtc->Day) / 10));
    date_set(&Screen0_Home.Screen0_Home_Day_2, ((rtc->Day) % 10));
    date_set_b(&Screen0_Home.Screen0_Home_Hour_1, ((rtc->Hour) / 10));
    date_set_b(&Screen0_Home.Screen0_Home_Hour_2, ((rtc->Hour) % 10));
    date_set_b(&Screen0_Home.Screen0_Home_Min_1, ((rtc->Minute) / 10));
    date_set_b(&Screen0_Home.Screen0_Home_Min_2, ((rtc->Minute) % 10));
    date_set_b(&Screen0_Home.Screen0_Home_Sec_1, ((rtc->Second) / 10));
    date_set_b(&Screen0_Home.Screen0_Home_Sec_2, ((rtc->Second) % 10));
    week_set(&Screen0_Home.Screen0_Home_Week, rtc->DayOfWeek);
    
    //Refrigerator画面
    //date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Year_3, ((rtc->Year) % 100) / 10);
    //date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Year_4, ((rtc->Year) % 100) % 10);
    //date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Month_1, ((rtc->Month) / 10));
    //date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Month_2, ((rtc->Month) % 10));
    //date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Day_1, ((rtc->Day) / 10));
    //date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Day_2, ((rtc->Day) % 10));
    date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Hour_1, ((rtc->Hour) / 10));
    date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Hour_2, ((rtc->Hour) % 10));
    date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Min_1, ((rtc->Minute) / 10));
    date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Min_2, ((rtc->Minute) % 10));
    date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Sec_1, ((rtc->Second) / 10));
    date_set(&Screen1_Refrigerator.Screen1_Refrigerator_Sec_2, ((rtc->Second) % 10));
    //week_set(&Screen1_Refrigerator.Screen1_Refrigerator_Week, rtc->DayOfWeek);
    
    //Washer画面
    //date_set(&Screen2_Washer.Screen2_Washer_Year_3, ((rtc->Year) % 100) / 10);
    //date_set(&Screen2_Washer.Screen2_Washer_Year_4, ((rtc->Year) % 100) % 10);
    //date_set(&Screen2_Washer.Screen2_Washer_Month_1, ((rtc->Month) / 10));
    //date_set(&Screen2_Washer.Screen2_Washer_Month_2, ((rtc->Month) % 10));
    //date_set(&Screen2_Washer.Screen2_Washer_Day_1, ((rtc->Day) / 10));
    //date_set(&Screen2_Washer.Screen2_Washer_Day_2, ((rtc->Day) % 10));
    date_set(&Screen2_Washer.Screen2_Washer_Hour_1, ((rtc->Hour) / 10));
    date_set(&Screen2_Washer.Screen2_Washer_Hour_2, ((rtc->Hour) % 10));
    date_set(&Screen2_Washer.Screen2_Washer_Min_1, ((rtc->Minute) / 10));
    date_set(&Screen2_Washer.Screen2_Washer_Min_2, ((rtc->Minute) % 10));
    date_set(&Screen2_Washer.Screen2_Washer_Sec_1, ((rtc->Second) / 10));
    date_set(&Screen2_Washer.Screen2_Washer_Sec_2, ((rtc->Second) % 10));
    //week_set(&Screen2_Washer.Screen2_Washer_Week, rtc->DayOfWeek);
    
    //Microwave画面
    //date_set(&Screen3_Oven.Screen3_Oven_Year_3, ((rtc->Year) % 100) / 10);
    //date_set(&Screen3_Oven.Screen3_Oven_Year_4, ((rtc->Year) % 100) % 10);
    //date_set(&Screen3_Oven.Screen3_Oven_Month_1, ((rtc->Month) / 10));
    //date_set(&Screen3_Oven.Screen3_Oven_Month_2, ((rtc->Month) % 10));
    //date_set(&Screen3_Oven.Screen3_Oven_Day_1, ((rtc->Day) / 10));
    //date_set(&Screen3_Oven.Screen3_Oven_Day_2, ((rtc->Day) % 10));
    date_set(&Screen3_Oven.Screen3_Oven_Hour_1, ((rtc->Hour) / 10));
    date_set(&Screen3_Oven.Screen3_Oven_Hour_2, ((rtc->Hour) % 10));
    date_set(&Screen3_Oven.Screen3_Oven_Min_1, ((rtc->Minute) / 10));
    date_set(&Screen3_Oven.Screen3_Oven_Min_2, ((rtc->Minute) % 10));
    date_set(&Screen3_Oven.Screen3_Oven_Sec_1, ((rtc->Second) / 10));
    date_set(&Screen3_Oven.Screen3_Oven_Sec_2, ((rtc->Second) % 10));
    //week_set(&Screen3_Oven.Screen3_Oven_Week, rtc->DayOfWeek);
    
    //Movie画面
    //date_set(&Screen4_Movie.Screen4_Movie_Year_3, ((rtc->Year) % 100) / 10);
    //date_set(&Screen4_Movie.Screen4_Movie_Year_4, ((rtc->Year) % 100) % 10);
    //date_set(&Screen4_Movie.Screen4_Movie_Month_1, ((rtc->Month) / 10));
    //date_set(&Screen4_Movie.Screen4_Movie_Month_2, ((rtc->Month) % 10));
    //date_set(&Screen4_Movie.Screen4_Movie_Day_1, ((rtc->Day) / 10));
    //date_set(&Screen4_Movie.Screen4_Movie_Day_2, ((rtc->Day) % 10));
    date_set(&Screen4_Movie.Screen4_Movie_Hour_1, ((rtc->Hour) / 10));
    date_set(&Screen4_Movie.Screen4_Movie_Hour_2, ((rtc->Hour) % 10));
    date_set(&Screen4_Movie.Screen4_Movie_Min_1, ((rtc->Minute) / 10));
    date_set(&Screen4_Movie.Screen4_Movie_Min_2, ((rtc->Minute) % 10));
    date_set(&Screen4_Movie.Screen4_Movie_Sec_1, ((rtc->Second) / 10));
    date_set(&Screen4_Movie.Screen4_Movie_Sec_2, ((rtc->Second) % 10));
    //week_set(&Screen4_Movie.Screen4_Movie_Week, rtc->DayOfWeek);
    
    //Information画面
    //date_set(&Screen5_Information.Screen5_Information_Year_3, ((rtc->Year) % 100) / 10);
    //date_set(&Screen5_Information.Screen5_Information_Year_4, ((rtc->Year) % 100) % 10);
    //date_set(&Screen5_Information.Screen5_Information_Month_1, ((rtc->Month) / 10));
    //date_set(&Screen5_Information.Screen5_Information_Month_2, ((rtc->Month) % 10));
    //date_set(&Screen5_Information.Screen5_Information_Day_1, ((rtc->Day) / 10));
    //date_set(&Screen5_Information.Screen5_Information_Day_2, ((rtc->Day) % 10));
    date_set(&Screen5_Information.Screen5_Information_Hour_1, ((rtc->Hour) / 10));
    date_set(&Screen5_Information.Screen5_Information_Hour_2, ((rtc->Hour) % 10));
    date_set(&Screen5_Information.Screen5_Information_Min_1, ((rtc->Minute) / 10));
    date_set(&Screen5_Information.Screen5_Information_Min_2, ((rtc->Minute) % 10));
    date_set(&Screen5_Information.Screen5_Information_Sec_1, ((rtc->Second) / 10));
    date_set(&Screen5_Information.Screen5_Information_Sec_2, ((rtc->Second) % 10));
    //week_set(&Screen5_Information.Screen5_Information_Week, rtc->DayOfWeek);
}

//曜日のみ書き込む画像の値が異なるため、別関数を用いた
VOID week_set(GX_ICON* icon, INT num) 
{

    switch (num)
    {
    case 0:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_SUN, GX_NULL);
        break;

    case 1:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_MON, GX_NULL);
        break;

    case 2:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_THU, GX_NULL);
        break;

    case 3:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_WED, GX_NULL);
        break;

    case 4:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_THU, GX_NULL);
        break;

    case 5:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_FRI, GX_NULL);
        break;

    case 6:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_SAT, GX_NULL);
        break;

    default:
        break;
    }
}

//渡された数値を渡されたwidgetに格納する
VOID date_set(GX_ICON* icon, INT num)
{

    switch (num)
    {
    case 0:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_0, GX_NULL);
        break;

    case 1:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_1, GX_NULL);
        break;

    case 2:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_2, GX_NULL);
        break;

    case 3:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_3, GX_NULL);
        break;

    case 4:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_4, GX_NULL);
        break;

    case 5:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_5, GX_NULL);
        break;

    case 6:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_6, GX_NULL);
        break;

    case 7:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_7, GX_NULL);
        break;

    case 8:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_8, GX_NULL);
        break;

    case 9:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_N_9, GX_NULL);
        break;

    default:
        break;
    }
}

VOID date_set_b(GX_ICON* icon, int num)
{

    switch (num)
    {
    case 0:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_0, GX_NULL);
        break;

    case 1:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_1, GX_NULL);
        break;

    case 2:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_2, GX_NULL);
        break;

    case 3:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_3, GX_NULL);
        break;

    case 4:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_4, GX_NULL);
        break;

    case 5:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_5, GX_NULL);
        break;

    case 6:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_6, GX_NULL);
        break;

    case 7:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_7, GX_NULL);
        break;

    case 8:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_8, GX_NULL);
        break;

    case 9:
        gx_icon_pixelmap_set(icon, GX_PIXELMAP_ID_NB_9, GX_NULL);
        break;

    default:
        break;
    }
}
