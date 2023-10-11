/* This is a small demo of the high-performance GUIX graphics framework. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "gx_api.h"
#include "SampleGrape.h"
#include "Sample_Grape_resources.h"
#include "Sample_Grape_specifications.h"
#include "rz_a3_display_driver.h"
#include "bsp_api.h"


#define MEMORY_POOL_BUFFER_SIZE  (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION * 8)

/* Used for initialization */
GX_WINDOW* InitScreen;
GX_WINDOW_ROOT* Root;

/* Used to memory allocate */
static TX_BYTE_POOL memory_pool;
static GX_UBYTE memory_pool_buffer[MEMORY_POOL_BUFFER_SIZE];

/* Create stack and control block for demo thread. */
static TX_THREAD demo_thread;
//static ULONG demo_thread_stack[1024];
static ULONG demo_thread_stack[2048];

//extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY* display);
extern UINT rz_a3_graphics_driver_setup_32argb(GX_DISPLAY* display);

/* Define prototypes.   */
VOID demo_thread_entry(ULONG thread_input);

//時計関連で使用
RTCDATE_TIME RtcDate;
//どちらのボタングループが表示されているのかについて格納
INT ActiveBtn;
//アニメーション終了までDRAG_MOVEを許可しない為の変数を用意
INT ChangeAnimation;

//マウスの定義を行う上で必要になる処理

#if MOUSE_OPERATION_CHECK
//threadの定義で使用
extern void mouse_thread_entry();
TX_THREAD mouse_thread;
CHAR mouse_thread_stack[2048];
//GUIX上でマウスの定義を行うのに必要
GX_MOUSE_CURSOR_INFO mouse_cursor;

#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void* _memory_allocate(ULONG size);
static void _memory_free(VOID* mem);

static void* _memory_allocate(ULONG size)
{
    VOID* memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

void _memory_free(VOID* mem)
{
    tx_byte_release(mem);

}

VOID init_SampleGrape(void)
{
#if MOUSE_OPERATION_CHECK
    //GUIXのthread作成
    tx_thread_create(&demo_thread, "Demo Thread", demo_thread_entry, 0,
        demo_thread_stack, sizeof(demo_thread_stack),
        GX_SYSTEM_THREAD_PRIORITY + 1, GX_SYSTEM_THREAD_PRIORITY + 1,
        TX_NO_TIME_SLICE, TX_AUTO_START);

    //マウス機能を使用するためのthreadを作成
    tx_thread_create(&mouse_thread, "mouse thread", mouse_thread_entry, 0,
            mouse_thread_stack, sizeof(mouse_thread_stack),
            20, 20, TX_NO_TIME_SLICE, TX_AUTO_START);
#endif
}

VOID demo_thread_entry(ULONG thread_input)
{
	FSP_PARAMETER_NOT_USED(thread_input);
    /* Create byte pool*/
    tx_byte_pool_create(&memory_pool, "memory_pol", memory_pool_buffer, MEMORY_POOL_BUFFER_SIZE);

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Assign memory alloc/free functions. */
    gx_system_memory_allocator_set(_memory_allocate, _memory_free);

    /* Used for display configuration. */
    gx_studio_display_configure(DISPLAY_1, rz_a3_graphics_driver_setup_32argb, LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &Root);

    /* create the hello world screen */
    gx_studio_named_widget_create("Screen_Title", (GX_WIDGET*)Root, (GX_WIDGET**)&InitScreen);

    /* Other screen settings to be used */
    gx_studio_named_widget_create("Screen0_Home", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("Screen1_Refrigerator", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("Screen2_Washer", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("Screen3_Oven", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("Screen4_Movie", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("Screen5_Information", GX_NULL, GX_NULL);

#if MOUSE_OPERATION_CHECK
    //マウスの定義を行う
    mouse_cursor.gx_mouse_cursor_image_id = GX_PIXELMAP_ID_MOUSE_ICON;
    mouse_cursor.gx_mouse_cursor_hotspot_x = 0;
    mouse_cursor.gx_mouse_cursor_hotspot_y = 0;

    //指定するキャンバスはRootwindowのキャンバス
    gx_canvas_mouse_define(Root->gx_window_root_canvas, &mouse_cursor);
#endif
    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(Root);

    /* start GUIX thread */
    gx_system_start();

    //初期に表示を行うボタングループはボタングループ1となるため、 BUTTONWINDOW_1を格納
    ActiveBtn = BUTTONWINDOW_1;
}

//タイトル画面でムービー再生を待機するEventFunction
UINT title_event(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    static INT TitleTimeCount = 0;
    GX_EVENT  MyEvent;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        //デフォルトのwindow処理を行う
        gx_window_event_process(window, event_ptr);

        //タイトル遷移用タイマを起動
        gx_system_timer_start((GX_WIDGET*)window, TITLE_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);
        break;
    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == TITLE_TIMER) 
        {
            //タイトル用タイマのカウント
            TitleTimeCount++;

            //TitleTimeCountの値がTITLE_TOGGLETIMEより大きくなったらtoggle
            //if (TitleTimeCount > TITLE_TOGGLETIME)
            if (TitleTimeCount > 0)
            {
                //イベントを発行するためにMyEventにメモリをセット
                memset(&MyEvent, 0, sizeof(GX_EVENT));

                //現在のwindowでのユーザーイベントを起こす
                MyEvent.gx_event_target = (GX_WIDGET *)window;

                //定義したイベントの格納
                MyEvent.gx_event_type = TOGGLE_EVENT;

                //イベントを発行
                gx_system_event_send(&MyEvent);

                //使用していたタイマを停止
                gx_system_timer_stop(window, TITLE_TIMER);
            }
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
        break;
    }

    return 0;
}

//画面遷移に使用
VOID toggle_screen(GX_WINDOW* new_win, GX_WINDOW* old_win)
{
    //遷移前の画面への処理
    gx_widget_hide(old_win);
    gx_widget_detach(old_win);

    //遷移後の画面への処理
    gx_widget_attach(Root, new_win);
    gx_widget_show(new_win);
}

//引数のイベントポインタからクリック時の座標を取得し、そこへspritewidgetを移動させる
VOID sprite_position(GX_SPRITE* sprite, GX_EVENT* event_ptr)
{
    INT x;
    INT y;
    GX_RECTANGLE NewSize;

    //クリック時の座標を格納
    x = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x - 20;
    y = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y - 20;

    //先ほど求めた座標を基にspriteを配置
    gx_utility_rectangle_define(&NewSize, (GX_VALUE)x, (GX_VALUE)y, (GX_VALUE)(x + 40), (GX_VALUE)(y + 40));
    gx_widget_resize(sprite, &NewSize);

    //spriteを再生
    gx_sprite_start(sprite, 0);
}

//画面遷移時にボタンを正しい位置へ配置
VOID button_position(GX_WINDOW* now_win, INT active_button, GX_WINDOW* button_group)
{
    GX_EVENT  MyEvent;
    GX_RECTANGLE NewSize;

    //ShowHide前にボタンを正しい位置に配置
    //座標をnew_sizeに格納し、これをresizeで使用している
    gx_utility_rectangle_define(&NewSize, 0, 630, 600, 800);
    gx_widget_resize(button_group, &NewSize);

    //イベントを発行するためにMyEventにメモリをセット
    memset(&MyEvent, 0, sizeof(GX_EVENT));
   
    //現在表示されている画面でどちらをshow/hideするかを決める
    if (active_button == 1)
    {
        MyEvent.gx_event_target = (GX_WIDGET *)now_win;
        MyEvent.gx_event_type = B1_SHOW;
    }
    if (active_button == 2)
    {
        MyEvent.gx_event_target = (GX_WIDGET *)now_win;
        MyEvent.gx_event_type = B2_SHOW;
    }

    //イベントを発行
    gx_system_event_send(&MyEvent);
}

//画面下部にあるボタングループ移動用関数
INT drag_move(GX_EVENT pendown, GX_EVENT* event_ptr, GX_WINDOW* window)
{
    INT HorizontalDist;
    GX_EVENT  MyEvent;

    //横方向のドラッグ距離を取得(PENDOWNした座標と現在ドラッグを行っている座標間の距離を計算している)
    HorizontalDist = GX_ABS(pendown.gx_event_payload.gx_event_pointdata.gx_point_x - event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x);

    //横方向へのドラッグ・ボタン群の範囲内でのドラッグ・ボタン群の遷移が複数回行われていない場合に入る
//    if (HorizontalDist > 100 && pendown.gx_event_payload.gx_event_pointdata.gx_point_y > 595)
      if (HorizontalDist > 100 && event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y > 595)
    {
        //イベントを発行するためにMyEventにメモリをセット
        memset(&MyEvent, 0, sizeof(GX_EVENT));

        //現在のwindowでのユーザーイベントを起こす
        MyEvent.gx_event_target = (GX_WIDGET *)window;

        //左方向へのドラッグの場合
        if (pendown.gx_event_payload.gx_event_pointdata.gx_point_x > event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x) 
        {
            //ボタン1が既に出ている場合はボタン2を出し、ボタン2が出ている場合は1が出る
            if (ActiveBtn == BUTTONWINDOW_1)
            {
                MyEvent.gx_event_type = B2_LEFT_MOVE;
                ActiveBtn = BUTTONWINDOW_2;
            }
            else 
            {
                MyEvent.gx_event_type = B1_LEFT_MOVE;
                ActiveBtn = BUTTONWINDOW_1;
            }
        }

        //右方向へのドラッグの場合
        if (pendown.gx_event_payload.gx_event_pointdata.gx_point_x < event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x) 
        {
            //こちらも先ほどと同様の処理
            if (ActiveBtn == BUTTONWINDOW_1)
            {
                MyEvent.gx_event_type = B2_RIGHT_MOVE;
                ActiveBtn = BUTTONWINDOW_2;
            }
            else 
            {
                MyEvent.gx_event_type = B1_RIGHT_MOVE;
                ActiveBtn = BUTTONWINDOW_1;
            }
        }

        //イベントを発行
        gx_system_event_send(&MyEvent);

        return DARG_MOVE_SUCCESS;
    }

    return DARG_MOVE_FAILED;
}

/* Call for screen transition after sprite animation */
VOID sprite_comp_func(GX_WINDOW* window, INT push_button_num)
{
    switch (push_button_num)
    {
    case HOME_B_PUSH:
        toggle_screen((GX_WINDOW *)&Screen0_Home, window);
//        movie_stop_set();
        break;

    case REFRI_B_PUSH:
        toggle_screen((GX_WINDOW *)&Screen1_Refrigerator, window);
//        movie_stop_set();
        break;

    case WASH_B_PUSH:
        toggle_screen((GX_WINDOW *)&Screen2_Washer, window);
//        movie_stop_set();
        break;

    case MICROWAVE_B_PUSH:
        toggle_screen((GX_WINDOW *)&Screen3_Oven, window);
//        movie_stop_set();
        break;

    case MOVIE_B_PUSH:
        toggle_screen((GX_WINDOW *)&Screen4_Movie, window);
//        movie_start_set();
        break;

    case INFO_B_PUSH:
        toggle_screen((GX_WINDOW *)&Screen5_Information, window);
//        movie_stop_set();
        break;

    default:
        break;
    }
}

//スワイプイン・スワイプアウト用のイベント発行用関数
VOID swipe_event(GX_WINDOW* window, INT event_num)
{
    //イベント用の構造体
    GX_EVENT  MyEvent;

    //イベントを発行するためにMyEventにメモリをセット
    memset(&MyEvent, 0, sizeof(GX_EVENT));

    //現在のwindowでのユーザーイベントを起こす
    MyEvent.gx_event_target = (GX_WIDGET *)window;

    //スワイプアウトさせる場合
    if (event_num == SWIPEOUT_EVENT)
    {
        //時刻画面のスワイプアウトのアニメーションイベントを発行
        MyEvent.gx_event_type = SWIPE_OUT_TIME;
        gx_system_event_send(&MyEvent);

        //ActiveBtnが格納する数値で格納するユーザーイベントを変更
        if (ActiveBtn == BUTTONWINDOW_1)
        {
            MyEvent.gx_event_type = SWIPE_OUT_BUTTON_1;
        }
        else if (ActiveBtn == BUTTONWINDOW_2)
        {
            MyEvent.gx_event_type = SWIPE_OUT_BUTTON_2;
        }
    }
    //スワイプインさせる場合
    else if (event_num == SWIPEIN_EVENT)
    {
        //時刻画面のスワイプインのアニメーションイベントを発行
        MyEvent.gx_event_type = SWIPE_IN_TIME;
        gx_system_event_send(&MyEvent);

        //ActiveBtnが格納する数値で格納するユーザーイベントを変更
        if (ActiveBtn == BUTTONWINDOW_1)
        {
            MyEvent.gx_event_type = SWIPE_IN_BUTTON_1;
        }
        else if (ActiveBtn == BUTTONWINDOW_2)
        {
            MyEvent.gx_event_type = SWIPE_IN_BUTTON_2;
        }
    }

    //アニメーション中にDRAG_MOVEを呼ばないようにする
    ChangeAnimation = DRAG_UNENABLE;

    //イベントを発行
    gx_system_event_send(&MyEvent);
}
