/*******************************************************************************************************************//**
 * Touch panel driver for common
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup TOUCH_PANEL
 * @{
 **********************************************************************************************************************/
#ifndef _TOUCH_PANEL_H_
#define _TOUCH_PANEL_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "bsp_api.h"
#include "hal_data.h"
#include "r_external_irq_api.h"
#include "r_i2c_master_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** touch panel device select */
//#define TP_USE_EXC80H60			/* for EXC80H60 */
#define TP_USE_GT911			/* for GT911 */
#define TP_USE_VERTICAL         /* Vertical or Horizontal */

#ifdef TP_USE_EXC80H60
#ifdef TP_USE_VERTICAL
#define PANEL_MAX_X             (600)       /* Panel Resolution X */
#define PANEL_MAX_Y             (800)       /* Panel Resolution Y */
#else
#define PANEL_MAX_X             (800)       /* Panel Resolution X */
#define PANEL_MAX_Y             (600)       /* Panel Resolution Y */
#endif
#else
#ifdef TP_USE_VERTICAL
#define PANEL_MAX_X             (720)       /* Panel Resolution X */
#define PANEL_MAX_Y             (1280)       /* Panel Resolution Y */
#else
#define PANEL_MAX_X             (1280)       /* Panel Resolution X */
#define PANEL_MAX_Y             (720)       /* Panel Resolution Y */
#endif
#endif

/** Touch Panel Interupt port */
#define TP_INT_PORT             (BSP_IO_PORT_08_PIN_01)

/** event flag pattern define */
#define TP_EVENT_FLAG_IRQ		(0x00000001UL)          /* from IRQ Handler */
#define TP_EVENT_FLAG_START		(0x00000002UL)          /* from API */
#define TP_EVENT_FLAG_STOP		(0x00000004UL)          /* from API */
#define TP_EVENT_FLAG_MASK		(TP_EVENT_FLAG_IRQ|TP_EVENT_FLAG_START|TP_EVENT_FLAG_STOP)
#define TP_EVENT_FLAG_I2C_ABT   (0x00000008UL)          /* from I2C Callback */
#define TP_EVENT_FLAG_I2C_RXC   (0x00000010UL)          /* from I2C Callback */
#define TP_EVENT_FLAG_I2C_TXC   (0x00000020UL)          /* from I2C Callback */
#define TP_EVENT_FLAG_I2C_ERR   (0x00000040UL)          /* from I2C Callback */
#define TP_EVENT_FLAG_I2C_MSK   (TP_EVENT_FLAG_I2C_ABT|TP_EVENT_FLAG_I2C_RXC|TP_EVENT_FLAG_I2C_TXC|TP_EVENT_FLAG_I2C_ERR)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** touch panel initialize parameters */
typedef  struct st_tp_init {
	TX_QUEUE *p_queue;
	TX_EVENT_FLAGS_GROUP *p_flag;
	TX_BLOCK_POOL *p_pool;
	external_irq_instance_t *p_irq_instance;
} tp_device_init_t;

/** touch panel state */
typedef enum e_tp_state {
	TP_STATE_PUSH       = 0x00000001UL,
	TP_STATE_RELEASE    = 0x00000002UL,
	TP_STATE_DRAG       = 0x00000004UL,
} tp_state_t;

/** touch panel message define */
typedef  struct st_tp_message {
	tp_state_t state;
	uint16_t pos_x;
	uint16_t pos_y;
} tp_messege_info_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/


/** @cond INC_HEADER_DEFS_SEC */

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
void touch_panel_init( TX_QUEUE *p_que, external_irq_instance_t *p_irq);
void touch_panel_deinit( void );
void touch_panel_message_release( void *p_msg );

/*******************************************************************************************************************//**
 * @} (end defgroup TOUCH_PANEL)
 **********************************************************************************************************************/

#endif /* _TOUCH_PANEL_H_ */
