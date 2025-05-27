/*******************************************************************************************************************//**
 * Touch panel driver for RZ/A3M Board
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "../inc/touch_panel.h"
#include "../inc/tp_gt911.h"
#include "tx_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define TP_THREAD_STACK_SZ		(2048)
#define TP_THREAD_PRI			(24)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#define TP_MSG_BLOCK_NUM		(10)
#define TP_MSG_POOL_SIZE		((sizeof(tp_messege_info_t)+sizeof(void*))*TP_MSG_BLOCK_NUM)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static TX_THREAD				tp_scan_thread;
static uint8_t					tp_scan_thread_stack[TP_THREAD_STACK_SZ] BSP_PLACE_IN_SECTION(".stack.tp_scan_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
static TX_EVENT_FLAGS_GROUP		tp_event_flags;
static VOID						(*entry_function)(ULONG) = NULL;		/* thread entry function pointer */
static tp_device_init_t			tp_param = {0};
static TX_BLOCK_POOL            tp_message_pool;
static uint8_t                  tp_block_pool_mem[TP_MSG_POOL_SIZE] = {0};


/*******************************************************************************************************************//**
 * @addtogroup TOUCH_PANEL
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

void touch_panel_init( TX_QUEUE *p_que, external_irq_instance_t *p_irq)
{
	UINT status;

	if( (p_que == NULL)||(p_irq == NULL))
	{
	    printf("[Touch] parameter is fail.\r\n");
	    return;
	}

	memset( (void*)&tp_param, 0x00, sizeof(tp_device_init_t));
#if defined TP_USE_EXC80H60
	entry_function = tp_exc80h60_thread_entry;
	tp_exc80h60_init( &tp_param );
#elif defined TP_USE_GT911
	entry_function = tp_gt911_thread_entry;
	tp_gt911_init( &tp_param );
#else
	printf("[Touch] device is not select.\r\n");
	return;
#endif

	/* event flag create */
	status = tx_event_flags_create( &tp_event_flags, (CHAR*)"tp event flag");
	if (TX_SUCCESS != status)
	{
		printf("[TP] tx_event_flags_create is failed(%d).\r\n", status);
		return;
	}

    /* block pool create */
    status = tx_block_pool_create( &tp_message_pool, (CHAR*)"tp block pool", (ULONG)sizeof(tp_messege_info_t), (void*)tp_block_pool_mem, TP_MSG_POOL_SIZE );
    if (TX_SUCCESS != status)
    {
        printf("[TP] tx_event_flags_create is failed(%d).\r\n", status);
        return;
    }

    /* save info */
	tp_param.p_queue		= p_que;
	tp_param.p_irq_instance = p_irq;
	tp_param.p_flag			= &tp_event_flags;
	tp_param.p_pool         = &tp_message_pool;

	/* thread create for tp */
	status = tx_thread_create(	&tp_scan_thread, "tp scan thread",
								entry_function,
								0,
								tp_scan_thread_stack, TP_THREAD_STACK_SZ,
								TP_THREAD_PRI, TP_THREAD_PRI,
								TX_NO_TIME_SLICE, TX_DONT_START);
	if( TX_SUCCESS != status )
	{
		tx_event_flags_delete(&tp_event_flags);
		printf("[TP] tx_thread_create is failed(%d).\r\n", status);
		return;
	}

	/* start */
	tx_event_flags_set( &tp_event_flags, (ULONG)TP_EVENT_FLAG_START, TX_OR );
	tx_thread_resume(&tp_scan_thread);

	return;
}

void touch_panel_deinit( void )
{
	UINT status;
	ULONG statusbit;

	/* set stop flag */
	tx_event_flags_set( &tp_event_flags, (ULONG)TP_EVENT_FLAG_STOP, TX_OR );

	/* wait clear stop flag */
	while(true)
	{
		status = tx_event_flags_get( &tp_event_flags, TP_EVENT_FLAG_STOP, TX_OR, &statusbit, TX_NO_WAIT );
		if( TX_NO_EVENTS == status )
		{
			break;
		}
		tx_thread_sleep(100);
	}

	/* resource close */
	tx_thread_suspend(&tp_scan_thread);
	tx_thread_delete(&tp_scan_thread);
	tx_event_flags_delete(&tp_event_flags);
}

void touch_panel_message_release( void *p_msg )
{
    tx_block_release(p_msg);
}

/*******************************************************************************************************************//**
 * @} (end addtogroup TOUCH_PANEL)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

