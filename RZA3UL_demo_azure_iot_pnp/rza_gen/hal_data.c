/* generated HAL source file - do not edit */
#include "hal_data.h"
#define FSP_NOT_DEFINED (UINT32_MAX)
#if (FSP_NOT_DEFINED) != (FSP_NOT_DEFINED)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void scif_uart_tx_dmac_callback(scif_uart_instance_ctrl_t const * const p_ctrl);

void g_uart0_tx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    scif_uart_tx_dmac_callback(&g_uart0_ctrl);
}
#endif

#if (FSP_NOT_DEFINED) != (FSP_NOT_DEFINED)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void scif_uart_rx_dmac_callback(scif_uart_instance_ctrl_t const * const p_ctrl);

void g_uart0_rx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    scif_uart_rx_dmac_callback(&g_uart0_ctrl);
}
#endif
#undef FSP_NOT_DEFINED

scif_uart_instance_ctrl_t g_uart0_ctrl;

scif_baud_setting_t g_uart0_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcs = 0,
          .semr_baudrate_bits_b.bgdm = 1, .semr_baudrate_bits_b.cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCIF HAL driver */
const scif_uart_extended_cfg_t g_uart0_cfg_extend =
{ .bri_ipl = 24, .bri_irq = SCIF0_BRK_IRQn, .clock = SCIF_UART_CLOCK_INT, .noise_cancel =
          SCIF_UART_NOISE_CANCELLATION_DISABLE,
  .p_baud_setting = &g_uart0_baud_setting, .rx_fifo_trigger = SCIF_UART_RECEIVE_TRIGGER_MAX, .rts_fifo_trigger =
          SCIF_UART_RTS_TRIGGER_14,
  .uart_mode = SCIF_UART_MODE_RS232, .flow_control = SCIF_UART_FLOW_CONTROL_NONE, .rs485_setting =
  { .enable = (sci_uart_rs485_enable_t) NULL, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH, .de_control_pin =
            (bsp_io_port_pin_t) SCIF_UART_INVALID_16BIT_PARAM, }, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 0,
  .data_bits = UART_DATA_BITS_8,
  .parity = UART_PARITY_OFF,
  .stop_bits = UART_STOP_BITS_1,
  .p_callback = uart0_cb_stdio,
  .p_context = NULL,
  .p_extend = &g_uart0_cfg_extend,
  .p_transfer_tx = g_uart0_P_TRANSFER_TX,
  .p_transfer_rx = g_uart0_P_TRANSFER_RX,
  .rxi_ipl = 24,
  .txi_ipl = 24,
  .tei_ipl = 24,
  .eri_ipl = 24,
  .rxi_irq = SCIF0_RXI_IRQn,
  .txi_irq = SCIF0_TXI_IRQn,
  .tei_irq = SCIF0_TEI_DRI_IRQn,
  .eri_irq = SCIF0_RERR_IRQn, };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_scif };
gtm_instance_ctrl_t g_timer0_ctrl;
const gtm_extended_cfg_t g_timer0_extend =
{ .generate_interrupt_when_starts = GTM_GIWS_TYPE_DISABLED, .gtm_mode = GTM_TIMER_MODE_INTERVAL, };
const timer_cfg_t g_timer0_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period_counts = 99999 /* Actual period: 0.001 seconds. */, .channel = 0, .p_callback =
          NULL,
  .p_context = NULL, .cycle_end_ipl = 30, .p_extend = &g_timer0_extend,
#if defined(VECTOR_NUMBER_GTM0_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GTM0_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = GTM0_OSTM0INT_IRQn,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_gtm };
extern void _tx_timer_interrupt(void);

#define portDISABLE_INTERRUPTS()									\
	__asm volatile ( "MSR DAIFSET, #1" ::: "memory" );				\
	__asm volatile ( "DSB SY" );									\
	__asm volatile ( "ISB SY" );

#define portENABLE_INTERRUPTS()										\
	__asm volatile ( "MSR DAIFCLR, #1" ::: "memory" );				\
	__asm volatile ( "DSB SY" );									\
	__asm volatile ( "ISB SY" );

/* In all GICs 255 can be written to the priority mask register to unmask all
 (but the lowest) interrupt priority. */
#define portUNMASK_VALUE				( 0xFFUL )

/* Macro to unmask all interrupt priorities. */
/* s3_0_c4_c6_0 is ICC_PMR_EL1. */
#define portCLEAR_INTERRUPT_MASK()						\
{														\
	__asm volatile (	"MSR DAIFSET, #1 		\n"		\
						"DSB SY					\n"		\
						"ISB SY					\n"		\
						"MSR s3_0_c4_c6_0, %0 	\n"		\
						"DSB SY					\n"		\
						"ISB SY					\n"		\
						"MSR DAIFCLR, #1 		\n"		\
						"DSB SY					\n"		\
						"ISB SY					\n"		\
						::"r"( portUNMASK_VALUE ) );	\
}

void tx_tick_handler(__attribute__( ( unused ))  timer_callback_args_t *unused);

void tx_tick_handler(__attribute__( ( unused ))  timer_callback_args_t *unused)
{
    /* Must be the lowest possible priority. */
#if !defined( QEMU )
    {
        uint64_t ullRunningInterruptPriority;
        /* s3_0_c12_c11_3 is ICC_RPR_EL1. */
        __asm volatile ( "MRS %0, s3_0_c12_c11_3" : "=r" ( ullRunningInterruptPriority ) );
//		configASSERT( ullRunningInterruptPriority == ( portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT ) );
    }
#endif

    /* Set interrupt mask before altering scheduler structures.   The tick
     handler runs at the lowest priority, so interrupts cannot already be masked,
     so there is no need to save and restore the current mask value.  It is
     necessary to turn off interrupts in the CPU itself while the ICCPMR is being
     updated. */
    /* s3_0_c4_c6_0 is ICC_PMR_EL1. */
    __asm volatile ( "MSR s3_0_c4_c6_0, %0		\n"
            "DSB SY					\n"
            "ISB SY					\n"
            :: "r" ( (17UL) << (3) ) : "memory" );
//					 :: "r" ( configMAX_API_CALL_INTERRUPT_PRIORITY << portPRIORITY_SHIFT ) : "memory" );

    /* Ok to enable interrupts after the interrupt source has been cleared. */
    portENABLE_INTERRUPTS();

#ifndef TX_NO_TIMER
    _tx_timer_interrupt ();
#endif

    /* Ensure all interrupt priorities are active again. */
    portCLEAR_INTERRUPT_MASK();
}

__attribute__( ( weak ) ) void vPortSetupTimerInterrupt(void) /* PRIVILEGED_FUNCTION */
{
    /* Stop and reset the SysTick. */
    g_timer0.p_api->open (g_timer0.p_ctrl, g_timer0.p_cfg);
    /* Configure SysTick to interrupt at the requested rate. */
    g_timer0.p_api->callbackSet (g_timer0.p_ctrl, tx_tick_handler, NULL, NULL);
    /* Start the SysTick. */
    g_timer0.p_api->start (g_timer0.p_ctrl);

}
#if configGENERATE_RUN_TIME_STATS
__attribute__( ( weak ) ) void vPortConfigureTimerForRunTimeStatus( void )
{
	/* Stop and reset the Status Timer. */
	FSP_NOT_DEFINED.p_api->open(FSP_NOT_DEFINED.p_ctrl, FSP_NOT_DEFINED.p_cfg);
	/* Start the Status Timer. */
	FSP_NOT_DEFINED.p_api->start(FSP_NOT_DEFINED.p_ctrl);
}

/* Count clock = 100MHz
   10-bits-right-shift makes 10.24us counting interval */
#define runtimeCLOCK_SCALE_SHIFT    ( 10 )

/* GTM overflows 32bit */
#define runtimeOVERFLOW_SHIFT       ( 32 - runtimeCLOCK_SCALE_SHIFT )
__attribute__( ( weak ) ) unsigned long ulGetRunTimeCounterValue( void )
{
	static unsigned long ulLastCounterValue = 0UL, ulOverflows = 0;
	unsigned long ulValueNow = 0;
	timer_status_t timer_status;

	/* Get count value */
	if (FSP_SUCCESS == FSP_NOT_DEFINED.p_api->statusGet(FSP_NOT_DEFINED.p_ctrl, &timer_status))
	{
		ulValueNow = timer_status.counter;

		/* Has the value overflowed since it was last read. */
		if( ulValueNow < ulLastCounterValue )
		{
			ulOverflows++;
		}
		ulLastCounterValue = ulValueNow;

		/* move the count value to  */
		ulValueNow >>= runtimeCLOCK_SCALE_SHIFT;
		ulValueNow += ulOverflows << runtimeOVERFLOW_SHIFT;
	}

	return ulValueNow;
}
#endif
void g_hal_init(void)
{
    g_common_init ();
}
