/* generated thread header file - do not edit */
#ifndef TSU_THREAD_H_
#define TSU_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void tsu_thread_entry(void);
                #else
extern void tsu_thread_entry(void);
#endif
#include "r_gtm.h"
FSP_HEADER
/** GTM Timer Instance */
extern const timer_instance_t g_timer1;

/** Access the GTM instance using these structures when calling API functions directly (::p_api is not used). */
extern gtm_instance_ctrl_t g_timer1_ctrl;
extern const timer_cfg_t g_timer1_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
FSP_FOOTER
#endif /* TSU_THREAD_H_ */
