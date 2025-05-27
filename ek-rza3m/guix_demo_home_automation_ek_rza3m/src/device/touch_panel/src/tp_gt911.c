/*******************************************************************************************************************//**
 * Touch panel driver for EXC80H60
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdlib.h>
#include "bsp_api.h"
#include "tx_api.h"
#include "../inc/tp_gt911.h"
#include "../../i2c/inc/i2c_ctrl.h"
#ifdef TP_USE_GT911
/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define GT911_I2C_SLV_ADDR     (0x14)      /* gt911 slave address */
//#define GT911_I2C_SLV_ADDR     (0x5D)      /* gt911 slave address */
#define GT911_I2C_WAIT         (500)       /* I2C Callback wait time */

#define WRITE_DEBUG

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
static uint8_t tmp_arr[186][3] = {
		{0x80, 0x47, 0x48},
		{0x80, 0x48, 0x0},
		{0x80, 0x49, 0x5},
		{0x80, 0x4A, 0x1F},
		{0x80, 0x4B, 0x3},
		{0x80, 0x4C, 0x5},
		{0x80, 0x4D, 0xBC},
		{0x80, 0x4E, 0x10},
		{0x80, 0x4F, 0x1},
		{0x80, 0x50, 0x8},
		{0x80, 0x51, 0x1E},
		{0x80, 0x52, 0x5},
		{0x80, 0x53, 0x46},
		{0x80, 0x54, 0x32},
		{0x80, 0x55, 0x3},
		{0x80, 0x56, 0x5},
		{0x80, 0x57, 0x0},
		{0x80, 0x58, 0x0},
		{0x80, 0x59, 0x0},
		{0x80, 0x5A, 0x0},
		{0x80, 0x5B, 0x0},
		{0x80, 0x5C, 0x0},
		{0x80, 0x5D, 0x6},
		{0x80, 0x5E, 0x18},
		{0x80, 0x5F, 0x1A},
		{0x80, 0x60, 0x1E},
		{0x80, 0x61, 0x14},
		{0x80, 0x62, 0x8B},
		{0x80, 0x63, 0x2B},
		{0x80, 0x64, 0xE},
		{0x80, 0x65, 0x20},
		{0x80, 0x66, 0x22},
		{0x80, 0x67, 0x33},
		{0x80, 0x68, 0xF},
		{0x80, 0x69, 0x0},
		{0x80, 0x6A, 0x0},
		{0x80, 0x6B, 0x0},
		{0x80, 0x6C, 0xC2},
		{0x80, 0x6D, 0x33},
		{0x80, 0x6E, 0x1C},
		{0x80, 0x6F, 0x0},
		{0x80, 0x70, 0x0},
		{0x80, 0x71, 0x55},
		{0x80, 0x72, 0xA},
		{0x80, 0x73, 0x0},
		{0x80, 0x74, 0x2B},
		{0x80, 0x75, 0xE0},
		{0x80, 0x76, 0x3},
		{0x80, 0x77, 0x32},
		{0x80, 0x78, 0x46},
		{0x80, 0x79, 0x32},
		{0x80, 0x7A, 0x1C},
		{0x80, 0x7B, 0x30},
		{0x80, 0x7C, 0x2},
		{0x80, 0x7D, 0xC2},
		{0x80, 0x7E, 0xA},
		{0x80, 0x7F, 0x5},
		{0x80, 0x80, 0x5},
		{0x80, 0x81, 0x0},
		{0x80, 0x82, 0x4},
		{0x80, 0x83, 0x96},
		{0x80, 0x84, 0x1D},
		{0x80, 0x85, 0x0},
		{0x80, 0x86, 0x8D},
		{0x80, 0x87, 0x20},
		{0x80, 0x88, 0x0},
		{0x80, 0x89, 0x83},
		{0x80, 0x8A, 0x24},
		{0x80, 0x8B, 0x0},
		{0x80, 0x8C, 0x7C},
		{0x80, 0x8D, 0x28},
		{0x80, 0x8E, 0x0},
		{0x80, 0x8F, 0x75},
		{0x80, 0x90, 0x2D},
		{0x80, 0x91, 0x0},
		{0x80, 0x92, 0x75},
		{0x80, 0x93, 0x0},
		{0x80, 0x94, 0x0},
		{0x80, 0x95, 0x0},
		{0x80, 0x96, 0x0},
		{0x80, 0x97, 0x0},
		{0x80, 0x98, 0x0},
		{0x80, 0x99, 0x0},
		{0x80, 0x9A, 0x0},
		{0x80, 0x9B, 0x0},
		{0x80, 0x9C, 0x0},
		{0x80, 0x9D, 0x0},
		{0x80, 0x9E, 0x1E},
		{0x80, 0x9F, 0x0},
		{0x80, 0xA0, 0x0},
		{0x80, 0xA1, 0x0},
		{0x80, 0xA2, 0x0},
		{0x80, 0xA3, 0x0},
		{0x80, 0xA4, 0x0},
		{0x80, 0xA5, 0x0},
		{0x80, 0xA6, 0x0},
		{0x80, 0xA7, 0x0},
		{0x80, 0xA8, 0x0},
		{0x80, 0xA9, 0x0},
		{0x80, 0xAA, 0x0},
		{0x80, 0xAB, 0x0},
		{0x80, 0xAC, 0x0},
		{0x80, 0xAD, 0x0},
		{0x80, 0xAE, 0x0},
		{0x80, 0xAF, 0x0},
		{0x80, 0xB0, 0x0},
		{0x80, 0xB1, 0x0},
		{0x80, 0xB2, 0x0},
		{0x80, 0xB3, 0x0},
		{0x80, 0xB4, 0x0},
		{0x80, 0xB5, 0x0},
		{0x80, 0xB6, 0x0},
		{0x80, 0xB7, 0x2},
		{0x80, 0xB8, 0x4},
		{0x80, 0xB9, 0x6},
		{0x80, 0xBA, 0x8},
		{0x80, 0xBB, 0xA},
		{0x80, 0xBC, 0xC},
		{0x80, 0xBD, 0xE},
		{0x80, 0xBE, 0x10},
		{0x80, 0xBF, 0x12},
		{0x80, 0xC0, 0x14},
		{0x80, 0xC1, 0x16},
		{0x80, 0xC2, 0x18},
		{0x80, 0xC3, 0x1A},
		{0x80, 0xC4, 0x1C},
		{0x80, 0xC5, 0xFF},
		{0x80, 0xC6, 0xFF},
		{0x80, 0xC7, 0xFF},
		{0x80, 0xC8, 0xFF},
		{0x80, 0xC9, 0xFF},
		{0x80, 0xCA, 0xFF},
		{0x80, 0xCB, 0xFF},
		{0x80, 0xCC, 0xFF},
		{0x80, 0xCD, 0xFF},
		{0x80, 0xCE, 0xFF},
		{0x80, 0xCF, 0xFF},
		{0x80, 0xD0, 0xFF},
		{0x80, 0xD1, 0xFF},
		{0x80, 0xD2, 0xFF},
		{0x80, 0xD3, 0xFF},
		{0x80, 0xD4, 0xFF},
		{0x80, 0xD5, 0x0},
		{0x80, 0xD6, 0x2},
		{0x80, 0xD7, 0x4},
		{0x80, 0xD8, 0x6},
		{0x80, 0xD9, 0x8},
		{0x80, 0xDA, 0xA},
		{0x80, 0xDB, 0xC},
		{0x80, 0xDC, 0xF},
		{0x80, 0xDD, 0x10},
		{0x80, 0xDE, 0x12},
		{0x80, 0xDF, 0x13},
		{0x80, 0xE0, 0x16},
		{0x80, 0xE1, 0x18},
		{0x80, 0xE2, 0x1C},
		{0x80, 0xE3, 0x1D},
		{0x80, 0xE4, 0x1E},
		{0x80, 0xE5, 0x1F},
		{0x80, 0xE6, 0x20},
		{0x80, 0xE7, 0x21},
		{0x80, 0xE8, 0x22},
		{0x80, 0xE9, 0x24},
		{0x80, 0xEA, 0x26},
		{0x80, 0xEB, 0xFF},
		{0x80, 0xEC, 0xFF},
		{0x80, 0xED, 0xFF},
		{0x80, 0xEE, 0xFF},
		{0x80, 0xEF, 0xFF},
		{0x80, 0xF0, 0xFF},
		{0x80, 0xF1, 0xFF},
		{0x80, 0xF2, 0xFF},
		{0x80, 0xF3, 0xFF},
		{0x80, 0xF4, 0xFF},
		{0x80, 0xF5, 0xFF},
		{0x80, 0xF6, 0xFF},
		{0x80, 0xF7, 0xFF},
		{0x80, 0xF8, 0xFF},
		{0x80, 0xF9, 0xFF},
		{0x80, 0xFA, 0xFF},
		{0x80, 0xFB, 0xFF},
		{0x80, 0xFC, 0xFF},
		{0x80, 0xFD, 0xFF},
		{0x80, 0xFE, 0xFF},
		{0x80, 0xFF, 0xC3},
		{0x81, 0x00, 0x1}
};


/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void _gt911_i2c_start( void );
static void _gt911_irq_start( void );
static void _gt911_i2c_stop( void );
static void _gt911_irq_stop( void );
static void _gt911_irq( void );
static void _gt911_i2c_callback( i2c_master_callback_args_t *p_arg );
static void _gt911_irq_callback( external_irq_callback_args_t *p_arg );
static UINT _gt911_i2c_wait_cb( i2c_master_event_t *p_event );
static void _gt911_setup( void );
static bool _gt911_i2c_reg_read( uint16_t reg, uint8_t *p_data );
static bool _gt911_i2c_reg_write( uint16_t reg, uint8_t data );

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static tp_device_init_t	*p_param = NULL;
static i2c_master_instance_t *p_i2c;

/*******************************************************************************************************************//**
 * @addtogroup TP_EXC80H60
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
void tp_gt911_thread_entry(ULONG thread_input)
{
	FSP_PARAMETER_NOT_USED(thread_input);
	UINT status;
	ULONG statusbit;
	bsp_io_level_t pin_value;

	printf("[GT911] tp_gt911_thread_entry start.\r\n");

	while(true)
	{
		status = tx_event_flags_get( p_param->p_flag, TP_EVENT_FLAG_MASK, TX_OR, &statusbit, TX_WAIT_FOREVER );
		if( TX_SUCCESS == status )
		{
			if( (statusbit & TP_EVENT_FLAG_START) == TP_EVENT_FLAG_START )
			{
			    g_ioport.p_api->pinRead( g_ioport.p_ctrl, TP_INT_PORT, &pin_value );
				printf("[GT911] TP_EVENT_FLAG_START(%d)\r\n", pin_value);
				_gt911_irq_start();
				/* setup gt911 */
				_gt911_setup();
				tx_event_flags_set( p_param->p_flag, (ULONG)~TP_EVENT_FLAG_START, TX_AND );
			}
			if( (statusbit & TP_EVENT_FLAG_STOP) == TP_EVENT_FLAG_STOP )
			{
				printf("[GT911] TP_EVENT_FLAG_STOP\r\n");
				_gt911_irq_stop();
				tx_event_flags_set( p_param->p_flag, (ULONG)~TP_EVENT_FLAG_STOP, TX_AND );
				break;
			}
			if( (statusbit & TP_EVENT_FLAG_IRQ) == TP_EVENT_FLAG_IRQ )
			{
				printf("[GT911] TP_EVENT_FLAG_IRQ\r\n");
				_gt911_irq();
				tx_event_flags_set( p_param->p_flag, (ULONG)~TP_EVENT_FLAG_IRQ, TX_AND );
			}
		}
		else
		{
			printf("[GT911] tx_event_flags_get failed.(%d)\r\n", status);
			tx_thread_sleep(1000);
		}
	}

}

/**
 * @brief tp_gt911_init_init
 * @param p_init
 */
void tp_gt911_init( tp_device_init_t *p_init )
{
	p_param = p_init;
}


/*******************************************************************************************************************//**
 * @} (end addtogroup TP_EXC80H60)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
static void _gt911_i2c_start( void )
{
	fsp_err_t err;

	// Get i2c instance
	p_i2c = i2c_get_ch1_func();

	/* i2C HAL Driver init */
	err = p_i2c->p_api->open( p_i2c->p_ctrl, p_i2c->p_cfg );
	if(FSP_SUCCESS != err)
	{
		printf("[GT911] I2C open failed.(%d)\r\n", err);
	}

	err = p_i2c->p_api->slaveAddressSet( p_i2c->p_ctrl, GT911_I2C_SLV_ADDR, I2C_MASTER_ADDR_MODE_7BIT );
	if(FSP_SUCCESS != err)
	{
		printf("[GT911] I2C slaveAddressSet failed.(%d)\r\n", err);
	}

	err = p_i2c->p_api->callbackSet( p_i2c->p_ctrl, _gt911_i2c_callback, NULL ,NULL );
	if(FSP_SUCCESS != err)
	{
		printf("[GT911] I2C callbackSet failed.(%d)\r\n", err);
	}
}

static void _gt911_irq_start( void )
{
	fsp_err_t err;

	/* IRQ HAL Driver init */
	err = p_param->p_irq_instance->p_api->open( p_param->p_irq_instance->p_ctrl, p_param->p_irq_instance->p_cfg );
	if(FSP_SUCCESS != err)
	{
		printf("[GT911] IRQ open failed.(%d)\r\n", err);
	}
	err = p_param->p_irq_instance->p_api->callbackSet( p_param->p_irq_instance->p_ctrl, _gt911_irq_callback, NULL, NULL );
	if(FSP_SUCCESS != err)
	{
		printf("[GT911] IRQ callbackSet failed.(%d)\r\n", err);
	}
	err = p_param->p_irq_instance->p_api->enable( p_param->p_irq_instance->p_ctrl );
	if(FSP_SUCCESS != err)
	{
		printf("[GT911] IRQ enable failed.(%d)\r\n", err);
	}
}

static void _gt911_i2c_stop( void )
{
	/* I2C Close */
	p_i2c->p_api->close( p_i2c->p_ctrl );
}

static void _gt911_irq_stop( void )
{
	/* IRQ Close */
	p_param->p_irq_instance->p_api->disable( p_param->p_irq_instance->p_ctrl );
	p_param->p_irq_instance->p_api->close( p_param->p_irq_instance->p_ctrl );

}

static void _gt911_irq( void )
{
	bool stat;
	UINT status;
	tp_messege_info_t *p_message;
	static tp_state_t old_state = TP_STATE_RELEASE;
	uint8_t buffer_data;
	uint8_t data[2];
	uint8_t trackid;
	uint16_t x_point = 0;
	uint16_t y_point = 0;
	uint16_t size;

	_gt911_i2c_start();

	do{
		/* get buffer status */
		stat = _gt911_i2c_reg_read( 0x814E, &(buffer_data) );
		if( !stat )
		{
			printf("i2c read data failed\r\n");
			return;
		}
		//printf("BufferStatus first :%#x\r\n", buffer_data);

		// Push touch panel
		if (((buffer_data & 0x80) && (buffer_data & 0x07)))
		{
			/* Track id */
			stat = _gt911_i2c_reg_read( 0x814F, &(data[0]) );
			if( !stat )
			{
			    printf("i2c read data failed\r\n");
			    return;
			}
			trackid = data[0];
			printf("Track id:%d\r\n", trackid);

			/* point 1 x coordinate (low byte) */
			stat = _gt911_i2c_reg_read( 0x8150, &(data[0]) );
			if( !stat )
			{
			    printf("i2c read data failed\r\n");
			    return;
			}
			/* point 1 x coordinate (high byte) */
			stat = _gt911_i2c_reg_read( 0x8151, &(data[1]) );
			if( !stat )
			{
			    printf("i2c read data failed\r\n");
			    return;
			}
			x_point = (uint16_t)((data[1]<<8) | data[0]);
			printf("x_point:%d\r\n", x_point);

			/* point 1 y coordinate (low byte) */
			stat = _gt911_i2c_reg_read( 0x8152, &(data[0]) );
			if( !stat )
			{
			    printf("i2c read data failed\r\n");
			    return;
			}
			/* point 1 y coordinate (high byte) */
			stat = _gt911_i2c_reg_read( 0x8153, &(data[1]) );
			if( !stat )
			{
			    printf("i2c read data failed\r\n");
			    return;
			}
			y_point = (uint16_t)((data[1]<<8) | data[0]);
			printf("y_point:%d\r\n", y_point);

			/* point 1 size (low byte) */
			stat = _gt911_i2c_reg_read( 0x8154, &(data[0]) );
			if( !stat )
			{
			    printf("i2c read data failed\r\n");
			    return;
			}
			/* point 1 size (high byte) */
			stat = _gt911_i2c_reg_read( 0x8155, &(data[1]) );
			if( !stat )
			{
			    printf("i2c read data failed\r\n");
			    return;
			}
			size = (uint16_t)((data[1]<<8) | data[0]);
			printf("size:%d\r\n", size);
		}
#if 1
		/* message block pool allocate */
		status = tx_block_allocate( p_param->p_pool, (void**)&p_message, TX_NO_WAIT );
		if( TX_SUCCESS != status )
		{
			printf("[GT911] tx_block_allocate failed(%d)\r\n",status);
			continue;
		}

#ifdef TP_USE_VERTICAL
		p_message->pos_x = (uint16_t)abs(720 - (int)x_point);
		p_message->pos_y = (uint16_t)abs(1280 - (int)y_point);
#else
		p_message->pos_x = (uint16_t)abs((int)y_point);
		p_message->pos_y = (uint16_t)abs(1280 - (int)x_point);
#endif

        /* touch state update */
        switch(old_state)
        {
            case TP_STATE_PUSH:
                if( buffer_data & 0x07 )
                {
                	p_message->state = TP_STATE_DRAG;
                }
                else
                {
                	p_message->state = TP_STATE_RELEASE;
                }
                old_state = p_message->state;
                break;
            case TP_STATE_DRAG:
                if( buffer_data & 0x07 )
                {
                	p_message->state = TP_STATE_DRAG;
                }
                else
                {
                	p_message->state = TP_STATE_RELEASE;
                }
                old_state = p_message->state;
                break;
            case TP_STATE_RELEASE:
            default:
                if( buffer_data & 0x07 )
                {
                	p_message->state = TP_STATE_PUSH;
                }
                else
                {
                	p_message->state = TP_STATE_RELEASE;
                }
                old_state = p_message->state;
                break;
        }
#endif
        printf("@msg:%d,%d,%d\r\n", p_message->state,  p_message->pos_x, p_message->pos_y );

        status = tx_queue_send( p_param->p_queue, (void*)&p_message, TX_WAIT_FOREVER );
        if( TX_SUCCESS != status )
        {
            printf("[EXC80] tx_queue_send failed(%d)\r\n",status);
        }

		// Clear register 0x814E
		stat = _gt911_i2c_reg_write( 0x814E, 0x00 );
		if( !stat )
		{
			printf("i2c read data failed\r\n");
			return;
		}

		tx_thread_sleep(50);
		// Check register 0x814E
		stat = _gt911_i2c_reg_read( 0x814E, &(data[0]) );
		if( !stat )
		{
			printf("i2c read data failed\r\n");
			return;
		}
		//printf("BufferStatus end :%#x\r\n", data[0]);
	}while((buffer_data & 0x80));

	_gt911_i2c_stop();
}

static void _gt911_i2c_callback( i2c_master_callback_args_t *p_arg )
{
    switch(p_arg->event)
    {
    case I2C_MASTER_EVENT_ABORTED:
        tx_event_flags_set( p_param->p_flag, (ULONG)TP_EVENT_FLAG_I2C_ABT, TX_OR );
        break;
    case I2C_MASTER_EVENT_RX_COMPLETE:
        tx_event_flags_set( p_param->p_flag, (ULONG)TP_EVENT_FLAG_I2C_RXC, TX_OR );
        break;
    case I2C_MASTER_EVENT_TX_COMPLETE:
        tx_event_flags_set( p_param->p_flag, (ULONG)TP_EVENT_FLAG_I2C_TXC, TX_OR );
        break;
    default:
        tx_event_flags_set( p_param->p_flag, (ULONG)TP_EVENT_FLAG_I2C_ERR, TX_OR );
        break;
    }
}

static void _gt911_irq_callback( external_irq_callback_args_t *p_arg )
{
	FSP_PARAMETER_NOT_USED(p_arg);
	tx_event_flags_set( p_param->p_flag, (ULONG)TP_EVENT_FLAG_IRQ, TX_OR );
}

static UINT _gt911_i2c_wait_cb( i2c_master_event_t *p_event )
{
    UINT status;
    ULONG statusbit;

    status = tx_event_flags_get( p_param->p_flag, TP_EVENT_FLAG_I2C_MSK, TX_OR_CLEAR, &statusbit, GT911_I2C_WAIT );
    if( TX_SUCCESS == status )
    {
        if( (statusbit & TP_EVENT_FLAG_I2C_ABT) == TP_EVENT_FLAG_I2C_ABT )
        {
            *p_event = I2C_MASTER_EVENT_ABORTED;
        }
        else if( (statusbit & TP_EVENT_FLAG_I2C_RXC) == TP_EVENT_FLAG_I2C_RXC )
        {
            *p_event = I2C_MASTER_EVENT_RX_COMPLETE;
        }
        else if( (statusbit & TP_EVENT_FLAG_I2C_TXC) == TP_EVENT_FLAG_I2C_TXC )
        {
            *p_event = I2C_MASTER_EVENT_TX_COMPLETE;
        }
        else if( (statusbit & TP_EVENT_FLAG_I2C_ERR) == TP_EVENT_FLAG_I2C_ERR )
        {
            status = TX_NOT_AVAILABLE;
        }
        else
        {
            status = TX_NOT_AVAILABLE;
        }
        return status;
    }
    else
    {
        printf("[EXC80] tx_event_flags_get failed(%X)\r\n", status);
        return TX_WAIT_ERROR;
    }
}

static void _gt911_setup( void )
{
    bool stat;
    uint8_t data[4] = {0};
    uint16_t addr = 0x8140;
    //uint16_t addr = 0x8047;
    uint16_t i;
    uint16_t x_reso;
    uint16_t y_reso;
    uint16_t x_point;
    uint16_t y_point;
    uint16_t sum = 0;
#if 0
    // Read BufferStatus
    stat = _gt911_i2c_reg_read( 0x814E, &(data[0]) );
    if( !stat )
    {
        printf("i2c read data failed\r\n");
        return;
    }
    printf("BufferStatus_1:%#x\r\n", data[0]);
    do{
        if(data[0] & 0x80)
        {
            stat = _gt911_i2c_reg_write( 0x814E, 0x00 );
            if( !stat )
            {
                printf("i2c read data failed\r\n");
                return;
            }
        }
        tx_thread_sleep(10);
        stat = _gt911_i2c_reg_read( 0x814E, &(data[0]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        printf("BufferStatus_2:%#x\r\n", data[0]);
    }while(data[0] & 0x80);
#endif

    _gt911_i2c_start();

    /* get product id */
    for(i=0;i<4;i++)
    {
        stat = _gt911_i2c_reg_read( addr+i, &(data[i]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
    }
    printf("Product ID:%s\r\n", data);

#if 0
    for(i=0;i<186;i++)
    {
    	addr = (uint16_t)(((uint16_t)(tmp_arr[i][0])<<8) | (uint16_t)(tmp_arr[i][1]));
    	data[0] = tmp_arr[i][2];
        stat = _gt911_i2c_reg_write( addr, data[0] );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        printf("addr:(%#x),val:(%#x)\r\n", addr, data[0]);
        sum = sum + data[0];
    }
    sum -= 0xC4;
    printf("sum:(%#X)\r\n", sum);
#endif
    /* x coordinate resolution */
    stat = _gt911_i2c_reg_read( 0x8146, &(data[0]) );
    //stat = _gt911_i2c_reg_read( 0x8048, &(data[0]) );
    if( !stat )
    {
        printf("i2c read data failed\r\n");
        return;
    }
    stat = _gt911_i2c_reg_read( 0x8147, &(data[1]) );
    //stat = _gt911_i2c_reg_read( 0x8049, &(data[1]) );
    if( !stat )
    {
        printf("i2c read data failed\r\n");
        return;
    }
    x_reso = (uint16_t)((data[1]<<8) | data[0]);
    printf("x coordinate resolution:%d\r\n", x_reso);

    /* y coordinate resolution */
    stat = _gt911_i2c_reg_read( 0x8148, &(data[0]) );
    //stat = _gt911_i2c_reg_read( 0x804A, &(data[0]) );
    if( !stat )
    {
        printf("i2c read data failed\r\n");
        return;
    }
    stat = _gt911_i2c_reg_read( 0x8149, &(data[1]) );
    //stat = _gt911_i2c_reg_read( 0x804B, &(data[1]) );
    if( !stat )
    {
        printf("i2c read data failed\r\n");
        return;
    }
    y_reso = (uint16_t)((data[1]<<8) | data[0]);
    printf("y coordinate resolution:%d\r\n", y_reso);

#if 0
    sum = 0;
    printf("sum:(%#X)\r\n", sum);
    for(i=0;i<186;i++)
    {
        stat = _gt911_i2c_reg_read( 0x8047+i, &(data[0]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        printf("Register:(%#X),Val:(%#X)\r\n", (0x8047+i), data[0]);
        sum = sum + data[0];
        //tx_thread_sleep(50);
    }
    sum -= 0xC4;
    printf("sum:(%#X)\r\n", sum);
#endif

#if 0
    while(1)
    {
        // Coordinate Information Registers
        stat = _gt911_i2c_reg_read( 0x814E, &(data[0]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        printf("0x814E:%#X\r\n", data[0]);
        do{
              if(data[0] & 0x80)
              {
                  stat = _gt911_i2c_reg_write( 0x814E, 0x00 );
                  if( !stat )
                  {
                      printf("i2c read data failed\r\n");
                      return;
                  }
              }
              tx_thread_sleep(10);
              stat = _gt911_i2c_reg_read( 0x814E, &(data[0]) );
              if( !stat )
              {
                  printf("i2c read data failed\r\n");
                  return;
              }
              printf("BufferStatus:%#x\r\n", data[0]);
        }while(data[0] & 0x80);

        stat = _gt911_i2c_reg_read( 0x8150, &(data[0]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        stat = _gt911_i2c_reg_read( 0x8151, &(data[1]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        x_point = (uint16_t)((data[1]<<8) | data[0]);
        printf("x_point:%#X\r\n", x_point);

        stat = _gt911_i2c_reg_read( 0x8152, &(data[0]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        stat = _gt911_i2c_reg_read( 0x8153, &(data[1]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        y_point = (uint16_t)((data[1]<<8) | data[0]);
        printf("y_point:%#X\r\n", y_point);

    	tx_thread_sleep(500);
    }
#else
    // Read BufferStatus
    stat = _gt911_i2c_reg_read( 0x814E, &(data[0]) );
    if( !stat )
    {
        printf("i2c read data failed\r\n");
        return;
    }
    //printf("BufferStatus_3:%#x\r\n", data[0]);
    do{
        if(data[0] & 0x80)
        {
            stat = _gt911_i2c_reg_write( 0x814E, 0x00 );
            if( !stat )
            {
                printf("i2c read data failed\r\n");
                return;
            }
        }
        tx_thread_sleep(100);
        stat = _gt911_i2c_reg_read( 0x814E, &(data[0]) );
        if( !stat )
        {
            printf("i2c read data failed\r\n");
            return;
        }
        //printf("BufferStatus_4:%#x\r\n", data[0]);
    }while(data[0] & 0x80);
#endif

    _gt911_i2c_stop();
}

static bool _gt911_i2c_reg_read( uint16_t reg, uint8_t *p_data )
{
    fsp_err_t err;
    UINT status;
    bool stat;
    uint8_t addr[2] = {0};
    i2c_master_event_t cb_event;

    addr[0] = (uint8_t)((reg & 0xFF00)>>8);
    addr[1] = (uint8_t)((reg & 0x00FF));

    //_gt911_i2c_start();

    err = p_i2c->p_api->write( p_i2c->p_ctrl, addr, 2, true );
    if( err != FSP_SUCCESS )
    {
        printf("write failed (%d)\r\n", err);
        stat = false;
        goto READ_END;
    }
    else
    {
        status = _gt911_i2c_wait_cb( &cb_event );
        if( status != TX_SUCCESS )
        {
            printf("_gt911_i2c_wait_cb failed (%d)\r\n", status);
            stat = false;
            goto READ_END;
        }
        else
        {
            switch(cb_event)
            {
            case I2C_MASTER_EVENT_RX_COMPLETE:
            case I2C_MASTER_EVENT_TX_COMPLETE:
                stat = true;
                break;
            case I2C_MASTER_EVENT_ABORTED:
            default:
                printf("[R]I2C_MASTER_EVENT_ABORTED or UNKNOWN (%d)\r\n", cb_event);
                stat = false;
            }
        }
    }

    if(stat)
    {
        err = p_i2c->p_api->read( p_i2c->p_ctrl, p_data, 1, false );
        if( err != FSP_SUCCESS )
        {
            printf("read failed (%d)\r\n", err);
            stat = false;
        }
        else
        {
            status = _gt911_i2c_wait_cb( &cb_event );
            if( status != TX_SUCCESS )
            {
                printf("_gt911_i2c_wait_cb failed (%d)\r\n", status);
                stat = false;
                goto READ_END;
            }
            else
            {
                switch(cb_event)
                {
                case I2C_MASTER_EVENT_RX_COMPLETE:
                case I2C_MASTER_EVENT_TX_COMPLETE:
                    stat = true;
                    break;
                case I2C_MASTER_EVENT_ABORTED:
                default:
                    printf("[R]I2C_MASTER_EVENT_ABORTED or UNKNOWN (%d)\r\n", cb_event);
                    stat = false;
                }
            }
        }
    }

    //_gt911_i2c_stop();

READ_END:
    return stat;
}

static bool _gt911_i2c_reg_write( uint16_t reg, uint8_t data )
{
    fsp_err_t err;
    UINT status;
    uint8_t addr[3] = {0};
    i2c_master_event_t cb_event;

    addr[0] = (uint8_t)((reg & 0xFF00)>>8);
    addr[1] = (uint8_t)((reg & 0x00FF));
    addr[2] = data;

    //_gt911_i2c_start();

    err = p_i2c->p_api->write( p_i2c->p_ctrl, addr, 3, false );
    if( err != FSP_SUCCESS )
    {
        printf("write_1 failed (%d)\r\n", err);
        return false;
    }
    else
    {
        status = _gt911_i2c_wait_cb( &cb_event );
        if( status != TX_SUCCESS )
        {
            printf("_gt911_i2c_wait_cb failed (%d)\r\n", status);
            return false;
        }
        else
        {
            switch(cb_event)
            {
            case I2C_MASTER_EVENT_RX_COMPLETE:
            case I2C_MASTER_EVENT_TX_COMPLETE:
                break;
            case I2C_MASTER_EVENT_ABORTED:
            default:
                printf("[R]I2C_MASTER_EVENT_ABORTED or UNKNOWN (%d)\r\n", cb_event);
                return false;
            }
        }
    }

    //_gt911_i2c_stop();

    return true;
}

#endif /* TP_USE_GT911 */

