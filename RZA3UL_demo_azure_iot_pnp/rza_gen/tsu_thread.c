/* generated thread source file - do not edit */
#include "tsu_thread.h"

TX_THREAD tsu_thread;
void tsu_thread_create(void);
static void tsu_thread_func(ULONG thread_input);
static uint8_t tsu_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.tsu_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
gtm_instance_ctrl_t g_timer1_ctrl;
const gtm_extended_cfg_t g_timer1_extend =
{ .generate_interrupt_when_starts = GTM_GIWS_TYPE_DISABLED, .gtm_mode = GTM_TIMER_MODE_INTERVAL, };
const timer_cfg_t g_timer1_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period_counts = 9999999 /* Actual period: 0.1 seconds. */, .channel = 1, .p_callback =
          NULL,
  .p_context = NULL, .cycle_end_ipl = 24, .p_extend = &g_timer1_extend,
#if defined(VECTOR_NUMBER_GTM1_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GTM1_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = GTM1_OSTM1INT_IRQn,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer1 =
{ .p_ctrl = &g_timer1_ctrl, .p_cfg = &g_timer1_cfg, .p_api = &g_timer_on_gtm };
extern bool g_fsp_common_initialized;
extern uint32_t g_fsp_common_thread_count;
extern TX_SEMAPHORE g_fsp_common_initialized_semaphore;

void tsu_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_fsp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&tsu_thread, (CHAR*) "TSU Thread", tsu_thread_func, (ULONG) NULL, &tsu_thread_stack, 1024,
                            1, 1, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&tsu_thread, 0);
    }
}

static void tsu_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    FSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    tsu_thread_entry ();
}
