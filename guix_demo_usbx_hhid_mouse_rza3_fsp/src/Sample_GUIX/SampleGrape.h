#pragma once
#include "gx_api.h"
#include "Sample_Grape_resources.h"
#include "Sample_Grape_specifications.h"

//タイマーのラベル
#define TITLE_TIMER         (1)
#define CLOCK_TIMER         (2)
#define GRAPH_TIMER         (3)
#define POPUP_TIMER         (4)

#define MPUTEST_TIMER       (50)

//タイトル画面からHome画面への遷移時間
//#define TITLE_TOGGLETIME    (2)
#define TITLE_TOGGLETIME    (0)


//DRAGアニメーションの許可と禁止
#define DRAG_ENABLE         (0)
#define DRAG_UNENABLE       (1)

//どちらのボタングループがアクティブか
#define BUTTONWINDOW_1      (1)
#define BUTTONWINDOW_2      (2)

//呼び出すイベントがスワイプアウトかインか
#define SWIPEOUT_EVENT      (1)
#define SWIPEIN_EVENT       (2)

//どのボタンが押されたかで使用
#define NONE_PUSH           (100)
#define HOME_B_PUSH         (0)
#define REFRI_B_PUSH        (1)
#define WASH_B_PUSH         (2)
#define MICROWAVE_B_PUSH    (3)
#define MOVIE_B_PUSH        (4)
#define INFO_B_PUSH         (5)

//DRAG_MOVE関数が成功したかどうかで使用
#define DARG_MOVE_SUCCESS   (0)
#define DARG_MOVE_FAILED    (1)

//MPU実行時に事前に用意した動作を実行するかどうか
//0→行わない　1→行う
#define MPU_OPERATION_CHECK (0)
//マウス機能を有効にするかどうか
//0→行わない　1→行う
#define MOUSE_OPERATION_CHECK (1)

//日付管理を行う構造体
typedef struct 
{
    INT Year;
    INT Month;
    INT DayOfWeek;
    INT Day;
    INT Hour;
    INT Minute;
    INT Second;
    INT Milliseconds;
} RTCDATE_TIME;

//SampleGrape.cで定義した関数
VOID init_SampleGrape(void);
VOID toggle_screen(GX_WINDOW* new_win, GX_WINDOW* old_win);
VOID button_position(GX_WINDOW* now_win, INT active_button, GX_WINDOW* button_group);
VOID sprite_position(GX_SPRITE* sprite, GX_EVENT* event_ptr);
INT  drag_move(GX_EVENT pendown, GX_EVENT* event_ptr, GX_WINDOW* window);
VOID sprite_comp_func(GX_WINDOW* window, INT push_button_num);
VOID swipe_event(GX_WINDOW* window, INT event_num);

#if MOUSE_OPERATION_CHECK

#include "tx_api.h"

//#include "fx_api.h"

#include "ux_api.h"
#include "ux_host_class_hub.h"
//#include "ux_host_class_storage.h"
#include "ux_host_stack.h"
#include "ux_system.h"

void tx_startup_err_callback(void *p_instance, void *p_data);

//マウス機能を稼働させるためのthread用に定義
//マウス機能のthreadに充てる関数
//void mouse_thread_entry(ULONG thread_input);
#endif
