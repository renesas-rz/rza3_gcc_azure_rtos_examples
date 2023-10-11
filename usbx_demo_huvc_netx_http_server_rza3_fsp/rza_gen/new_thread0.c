/* generated thread source file - do not edit */
#include "new_thread0.h"

TX_THREAD new_thread0;
void new_thread0_create(void);
static void new_thread0_func(ULONG thread_input);
static uint8_t new_thread0_stack[4096] BSP_PLACE_IN_SECTION(".stack.new_thread0") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);

extern bool g_fsp_common_initialized;
extern uint32_t g_fsp_common_thread_count;
extern TX_SEMAPHORE g_fsp_common_initialized_semaphore;

void new_thread0_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_fsp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&new_thread0, (CHAR*) "New Thread", new_thread0_func, (ULONG) NULL, &new_thread0_stack,
                            4096, 3, 3, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&new_thread0, 0);
    }
}

static void new_thread0_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    FSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    new_thread0_entry ();
}
