#include "tsu_thread.h"
#include <stdio.h>
#include "tx_api.h"
#include "hal_data.h"
#include "bsp_api.h"


#define OTPTSUTRIM0_PRL           (3148)
#define OTPTSUTRIM1_PRL           (503)
TX_EVENT_FLAGS_GROUP    tmout_event_flags;

static uint32_t l_otptsutrim0;
static uint32_t l_otptsutrim1;
static uint8_t l_otptsutrim_flag = 0;

static void g_timer1_callback(timer_callback_args_t * p_args);

int32_t  Tj = 0;

/* TSU Thread entry function */
void tsu_thread_entry(void)
{
    INT i=0;
    fsp_err_t err = FSP_SUCCESS;
    uint32_t temp = 1;
    uint32_t temp_arr[8] = {0};
    uint32_t temp_ave = 1;
    uint32_t dsensor = 0;
//    int32_t  Tj = 0;
    ULONG   actual_flags;

    tx_event_flags_create(&tmout_event_flags, "timeout event flags");

    // Open GTM
    err = g_timer1.p_api->open(g_timer1.p_ctrl, g_timer1.p_cfg);
    if(FSP_SUCCESS != err)
    {
        printf("[TSU] GTM open fail. (%d)\r\n", err);
        return;
    }

    // Set callback
    err = g_timer1.p_api->callbackSet(g_timer1.p_ctrl, g_timer1_callback, NULL, NULL);
    if(FSP_SUCCESS != err)
    {
        printf("[TSU] Set callback fail. (%d)\r\n", err);
        return;
    }

    // Start GTM
    err = g_timer1.p_api->start(g_timer1.p_ctrl);
    if(FSP_SUCCESS != err)
    {
        printf("[TSU] GTM start fail. (%d)\r\n", err);
        return;
    }

//    printf("\r\n");
//    printf("The acquisition result of the temperature sensor is displayed every 5 seconds.\r\n\r\n");

    R_CPG->CPG_CLKON_TSU = 0x00010001;
    R_CPG->CPG_RST_TSU = 0x00010001;

    tx_thread_sleep(1);

    R_TSU->TSU_SM_b.EN_TS = 0b1;
    R_TSU->TSU_SM_b.ADC_EN_TS = 0b1;

    tx_thread_sleep(1);

    R_TSU->TSU_ST_b.START = 0b0;

    tx_thread_sleep(1);

    R_TSU->TSU_ST_b.START = 0b1;

    tx_thread_sleep(1);

    if ((R_TSU->OTPTSUTRIM0_REG) && (R_TSU->OTPTSUTRIM1_REG))
    {
        l_otptsutrim_flag = 1;
        l_otptsutrim0 = (R_TSU->OTPTSUTRIM0_REG & 0xFFF);
        l_otptsutrim1 = (R_TSU->OTPTSUTRIM1_REG & 0xFFF);
    }
    else
    {
        l_otptsutrim_flag = 0;
        l_otptsutrim0 = OTPTSUTRIM0_PRL;
        l_otptsutrim1 = OTPTSUTRIM1_PRL;
    }
    /* TODO: add your own code here */
    while (1)
    {
        actual_flags = 0;
        tx_event_flags_get(&tmout_event_flags, 0x01, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

        if (actual_flags & 0x01)
        {
            while(1)
            {
                if(R_TSU->TSU_SS_b.CONV == 0b1)
                {
                    temp = R_TSU->TSU_SAD_b.OUT_12BIT_TS;
                    if(i < 8)
                    {
                        temp_arr[i] = temp;
                    }
                    else
                    {
                        temp_ave = (uint32_t)((temp_arr[0] + temp_arr[1] + temp_arr[2] + temp_arr[3] + temp_arr[4] + temp_arr[5] + temp_arr[6] + temp_arr[7]) / 8);
                        dsensor = (uint32_t)(temp_ave / (1 + (uint32_t)(((uint64_t)temp_ave * 13) / 1000000)));
                        Tj = (int32_t)(((uint64_t)((dsensor - l_otptsutrim1) * 165) / (uint64_t)(l_otptsutrim0 - l_otptsutrim1))) - 40;
                        if(l_otptsutrim_flag)
                        {
//                            printf("Tj = (%d)\r\n", Tj);
                        }
                        else
                        {
//                            printf("Tj(preliminary) = (%d)\r\n", Tj);
                        }
                        i = 0;
                        temp_ave = 0;
                        dsensor = 0;
                        memset(temp_arr, 0, sizeof(temp_arr));
                        break;
                    }
                    tx_thread_sleep(1);
                    i++;
                }
            }
        }
    }
}

static void g_timer1_callback(timer_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);

    tx_event_flags_set(&tmout_event_flags, 0x01, TX_OR);
}
