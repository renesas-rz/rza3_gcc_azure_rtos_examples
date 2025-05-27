/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "ili9881.h"
#include "hal_data.h"
#include "r_mipi_dsi_b.h"

extern bool g_message_sent;
static uint8_t send_data[4] __attribute__((section("UNCACHED_BSS")));

/*****************************************************************************************************************
 *  @brief      Initialization of
 *  @param[in]  None
 *  @retval     None
 ****************************************************************************************************************/
void ili9881_init(mipi_dsi_ctrl_t * const p_api_ctrl)
{
    ili9881_cmd_send(p_api_ctrl, command_flow_ili9881);
}

void ili9881_changepage(mipi_dsi_ctrl_t * const p_api_ctrl, uint8_t page)
{
    send_data[0] = 0xFF;
    send_data[1] = 0x98;
    send_data[2] = 0x81;
    send_data[3] = page;

    mipi_dsi_cmd_t cmd =
    {
     .channel = 0,
     .cmd_id = MIPI_DSI_CMD_ID_DCS_LONG_WRITE,
     .flags = (mipi_dsi_cmd_flag_t) (MIPI_DSI_CMD_FLAG_LOW_POWER),
     .tx_len = 4,
     .p_tx_buffer = send_data,
    };

    g_message_sent = false;
    R_MIPI_DSI_B_Command(p_api_ctrl, &cmd);
    while (!g_message_sent)
    {
        ;
    }
}

void ili9881_cmd_send(mipi_dsi_ctrl_t * const p_api_ctrl, command_type_t * command_flow)
{

    uint32_t i = 0;

    while(command_flow[i].operation != 0xFF)
    {
        if(command_flow[i].operation == 0)
        {
            ili9881_changepage(p_api_ctrl, command_flow[i].data0);
        }
        else if(command_flow[i].operation == 1)
        {
            send_data[0] = command_flow[i].data0;
            send_data[1] = command_flow[i].data1;
            mipi_dsi_cmd_t cmd =
            {
             .channel = 0,
             .cmd_id = MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_0_PARAM,
             .flags = (mipi_dsi_cmd_flag_t) (MIPI_DSI_CMD_FLAG_LOW_POWER),
             .tx_len = 2,
             .p_tx_buffer = send_data,
            };

            g_message_sent = false;
            R_MIPI_DSI_B_Command(p_api_ctrl, &cmd);
            while (!g_message_sent)
            {
                ;
            }
        }
        else if(command_flow[i].operation == 2)
        {
            send_data[0] = command_flow[i].data0;
            send_data[1] = command_flow[i].data1;
            mipi_dsi_cmd_t cmd =
            {
             .channel = 0,
             .cmd_id = MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,
             .flags = (mipi_dsi_cmd_flag_t) (MIPI_DSI_CMD_FLAG_LOW_POWER),
             .tx_len = 2,
             .p_tx_buffer = send_data,
            };

            g_message_sent = false;
            R_MIPI_DSI_B_Command(p_api_ctrl, &cmd);
            while (!g_message_sent)
            {
                ;
            }
        }
        else if(command_flow[i].operation == 0x10)
        {
            R_BSP_SoftwareDelay(command_flow[i].data0, BSP_DELAY_UNITS_MILLISECONDS);
        }
        else
        {
            /* no operation*/
        }
        i++;
    }
}

//void ili9881_cmd_reception(uint8_t data0, uint8_t data1)
//{
//    /*packet reception Step 1*/
//    mipi_dsi_cmd_send_short(0x37, 0x01, 0x00);
//
//    /*packet reception Step 2*/
//    mipi_dsi_cmd_reception(0x06, data0, data1);
//}
