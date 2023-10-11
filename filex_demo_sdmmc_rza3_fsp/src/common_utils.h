/***********************************************************************************************************************
 * File Name    : common_utils.h
 * Description  : Contains macros, data structures and functions used  common to the EP
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#ifndef COMMON_UTILS_H_
#define COMMON_UTILS_H_

/* generic headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_data.h"

#define BIT_SHIFT_8  (8u)
#define SIZE_64      (64u)

#define LVL_ERR      (1u)       /* error conditions   */
#define LVL_DEBUG    (3u)       /* debug-level messages */

#define LOG_LEVEL    LVL_ERR    /* To See the Debug Messages, LOG_LEVEL should be set to LVL_DEBUG */

#define RESET_VALUE             (0x00)

#define EP_VERSION              ("1.0")
#define MODULE_NAME             "FileX Blockmedia sdmmc"
#define BANNER_INFO             "\r\n********************************************************************************"\
                                "\r\n*   Renesas FSP Example Project for "MODULE_NAME" Module              *"\
                                "\r\n*   Example Project Version %s                                                *"\
                                "\r\n*   Flex Software Pack Version  %d.%d.%d                                          *"\
                                "\r\n********************************************************************************"\
                                "\r\nRefer to readme.txt file for more details on Example Project and" \
                                "\r\nFSP User's Manual for more information about "MODULE_NAME" module\r\n"

#define SEGGER_INDEX            (0)

#define APP_PRINT(fn_, ...)      SEGGER_RTT_printf (SEGGER_INDEX,(fn_), ##__VA_ARGS__);

#define APP_ERR_PRINT(fn_, ...)  if(LVL_ERR)\
        SEGGER_RTT_printf (SEGGER_INDEX, "[ERR] In Function: %s(), %s",__FUNCTION__,(fn_),##__VA_ARGS__);

#define APP_ERR_TRAP(err)        if(err) {\
        SEGGER_RTT_printf(SEGGER_INDEX, "\r\nReturned Error Code: 0x%x  \r\n", err);\
        __asm("BKPT #0\n");} /* trap upon the error  */

#define APP_READ(read_data)     SEGGER_RTT_Read (SEGGER_INDEX, read_data, sizeof(read_data));

#define APP_CHECK_DATA          SEGGER_RTT_HasKey()

#define APP_DBG_PRINT(fn_, ...) if(LOG_LEVEL >= LVL_DEBUG)\
        SEGGER_RTT_printf (SEGGER_INDEX, "[DBG] In Function: %s(), %s",__FUNCTION__,(fn_),##__VA_ARGS__);

/* macro for rtt rtos. */
#define AZURE_RTOS  (1)
#define FREERTOS    (2)

#if (BSP_CFG_RTOS == AZURE_RTOS)
#include "tx_api.h"
/*
 * RTOS - data structures and macros
 */

/* change or modify this based on your application requirement */
#define BYTE_POOL_SIZE      (ULONG)256U
#define TIME_STAMP_MSG_SIZE (35U)

/* As per SEGGER_RTT_conf.h size */
#define IP_MSG_MAX_SIZE     (BUFFER_SIZE_DOWN-1)
#ifndef BYTE_POOL_SIZE
#error "byte allocated size is essential to use RTT-rtos aware framework"
#else
/* ONLY 50% of POOL memory can be used for output buffer
 * for example - 256 bytes  is pool allocated
 * and SEGGER defined BUFFER_SIZE_UP macro is 1024
 *
 * hence only 128 bytes can be used for output buffer maximum
 * so formula made here is
 * 1) (1024/256) = 4
 * 2)  4*32 = 128
 *
 */
#define OP_MSG_MAX_SIZE     (UINT)( (UINT)(BUFFER_SIZE_UP/BYTE_POOL_SIZE) * (UINT)32U )
#endif

typedef enum event_id
{
    RTT_INPUT_MESSAGE_TYPE_STR_QUEUED,
    RTT_OUTPUT_MESSAGE_BANNER,
    RTT_OUTPUT_MESSAGE_APP_INFO_STR,
    RTT_OUTPUT_MESSAGE_APP_ERR_STR,
    RTT_OUTPUT_MESSAGE_APP_ERR_TRAP,
    RTT_OUTPUT_MESSAGE_APP_PRINT_MENU,
    RTT_OUTPUT_MESSAGE_APP_PRINT_MEDIA_SPACE,
    RTT_OUTPUT_MESSAGE_APP_PRINT_FILE_ENTRIES
}event_id_t;


/* Signature of function passed to tx_thread_info_get */
typedef struct
{
    TX_THREAD*   thread_ptr;
    CHAR *      name;
    UINT        state;
    ULONG       run_count;
    UINT        priority;
    UINT        preemption_threshold;
    ULONG       time_slice;
    TX_THREAD * next_thread;
    TX_THREAD * suspended_thread;
}thread_info_t;



#endif /* rtos aware RTT ds and func */

#endif /* COMMON_UTILS_H_ */
