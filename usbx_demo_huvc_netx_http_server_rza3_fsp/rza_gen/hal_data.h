/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "nxd_http_server.h"
#include "r_uart_api.h"
#include "r_scif_uart.h"
#include "r_gtm.h"
#include "tx_api.h"
#include "r_timer_api.h"
FSP_HEADER
#define G_HTTP_SERVER0_STACK_SIZE (4096)

void g_http_server0_quick_setup();
/** UART on SCIF Instance. */
extern const uart_instance_t g_uart0;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern scif_uart_instance_ctrl_t g_uart0_ctrl;
extern const uart_cfg_t g_uart0_cfg;
extern const scif_uart_extended_cfg_t g_uart0_cfg_extend;

#ifndef uart0_cb_stdio
void uart0_cb_stdio(uart_callback_args_t *p_args);
#endif

#define FSP_NOT_DEFINED (1)
#if (FSP_NOT_DEFINED == FSP_NOT_DEFINED)
#define g_uart0_P_TRANSFER_TX (NULL)
#else
    #define g_uart0_P_TRANSFER_TX (&FSP_NOT_DEFINED)
#endif
#if (FSP_NOT_DEFINED == FSP_NOT_DEFINED)
#define g_uart0_P_TRANSFER_RX (NULL)
#else
    #define g_uart0_P_TRANSFER_RX (&FSP_NOT_DEFINED)
#endif
#undef FSP_NOT_DEFINED
/** GTM Timer Instance */
extern const timer_instance_t g_timer0;

/** Access the GTM instance using these structures when calling API functions directly (::p_api is not used). */
extern gtm_instance_ctrl_t g_timer0_ctrl;
extern const timer_cfg_t g_timer0_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
extern void vPortSetupTimerInterrupt(void); /* PRIVILEGED_FUNCTION */
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
