#include "hal_data.h"
#include "r_i2c_master_api.h"

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER

enum{
    WM8978_REG_SOFTWARE_RESET,
    WM8978_REG_POWER_MANAGENT_1,
    WM8978_REG_POWER_MANAGENT_2,
    WM8978_REG_POWER_MANAGENT_3,
    WM8978_REG_AUDIO_INTERFACE,
    WM8978_REG_COMPANDING_CTRL,
    WM8978_REG_CLOCK_GEN_CTRL,
    WM8978_REG_ADDITIONAL_CTRL,
    WM8978_REG_GPIO_STUFF,
    WM8978_REG_JACK_DETECT_CONTROL,
    WM8978_REG_DAC_CONTROL,
    WM8978_REG_LEFT_DAC_DIGITAL_VOL,
    WM8978_REG_RIGHT_DAC_DIGITAL_VOL,
    WM8978_REG_ACK_DETECT_CONTROL,
    WM8978_REG_ADC_CONTROL,
    WM8978_REG_LEFT_ADC_DIGITAL_VOL,
    WM8978_REG_RIGHT_ADC_DIGITAL_VOL,
    WM8978_REG_017,
    WM8978_REG_EQ1_LOW_SHELF,
    WM8978_REG_EQ2_PEAK_1,
    WM8978_REG_EQ3_PEAK_2,
    WM8978_REG_EQ4_PEAK_3,
    WM8978_REG_EQ5_HIGH_SHELF,
    WM8978_REG_023,
    WM8978_REG_DAC_LIMITER_1,
    WM8978_REG_DAC_LIMITER_2,
    WM8978_REG_026,
    WM8978_REG_NOTCH_FILTER_1,
    WM8978_REG_NOTCH_FILTER_2,
    WM8978_REG_NOTCH_FILTER_3,
    WM8978_REG_NOTCH_FILTER_4,
    WM8978_REG_031,
    WM8978_REG_ALC_CONTROL_1,
    WM8978_REG_ALC_CONTROL_2,
    WM8978_REG_ALC_CONTROL_3,
    WM8978_REG_NOISE_GATE,
    WM8978_REG_PLL_N,
    WM8978_REG_PLL_K_1,
    WM8978_REG_PLL_K_2,
    WM8978_REG_PLL_K_3,
    WM8978_REG_040,
    WM8978_REG_3D_CONTROL,
    WM8978_REG_042,
    WM8978_REG_BEEP_CONTROL,
    WM8978_REG_INPUT_CTRL,
    WM8978_REG_LEFT_INP_PGA_GAIN_CTRL,
    WM8978_REG_RIGHT_INP_PGA_GAIN_CTRL,
    WM8978_REG_LEFT_ADC_BOOST_CTRL,
    WM8978_REG_RIGHT_ADC_BOOST_CTRL,
    WM8978_REG_OUTPUT_CTRL_0_0_DACL2,
    WM8978_REG_LEFT_MIXER_CTRL,
    WM8978_REG_RIGHT_MIXER_CTRL,
    WM8978_REG_LOUT1_HP_VOLUME_CTRL,
    WM8978_REG_ROUT1_HP_VOLUME_CTRL,
    WM8978_REG_LOUT2_SPK_VOLUME_CTRL,
    WM8978_REG_ROUT2_SPK_VOLUME_CTRL,
    WM8978_REG_OUT3_MIXER_CTRL,
    WM8978_REG_OUT4_MONO_MIXER_CTRL,
    WM8978_REG_MAX
};

/* ==== Register Setting Value ==== */
/* RESET R0 */
#define WM8978_RESET_INI_VALUE          (0x00000000u)
/* WM8978_REGADR_POW_MANAGE1 R1 */
#define WM8978_MANAGE1_INI_VALUE        (0x00000000u)
#define WM8978_MANAGE1_VMIDSEL_75K      (0x00000001u)
#define WM8978_MANAGE1_BUFIOEN_ON       (0x00000004u)
#define WM8978_MANAGE1_BIASEN_ON        (0x00000008u)
#define WM8978_MANAGE1_MICBEN_ON        (0x00000010u)
#define WM8978_MANAGE1_PLLEN_ON         (0x00000020u)
/* WM8978_REGADR_POW_MANAGE2 R2 */
#define WM8978_MANAGE2_INI_VALUE        (0x00000000u)
#define WM8978_MANAGE2_ADCENL_ON        (0x00000001u)
#define WM8978_MANAGE2_ADCENR_ON        (0x00000002u)
#define WM8978_MANAGE2_INPPGAENL_ON     (0x00000004u)
#define WM8978_MANAGE2_INPPGAENR_ON     (0x00000008u)
#define WM8978_MANAGE2_BOOSTENL_ON      (0x00000010u)
#define WM8978_MANAGE2_BOOSTENR_ON      (0x00000020u)
#define WM8978_MANAGE2_LOUT1EN_ON       (0x00000080u)
#define WM8978_MANAGE2_ROUT1EN_ON       (0x00000100u)
/* WM8978_REGADR_POW_MANAGE3 R3 */
#define WM8978_MANAGE3_INI_VALUE        (0x00000000u)
#define WM8978_MANAGE3_DACENL_ON        (0x00000001u)
#define WM8978_MANAGE3_DACENR_ON        (0x00000002u)
#define WM8978_MANAGE3_LMIXEN_ON        (0x00000004u)
#define WM8978_MANAGE3_RMIXEN_ON        (0x00000008u)
/* WM8978_REGADR_AUDIO_IF_CTL R4 */
#define WM8978_AUDIO_IF_INI_VALUE       (0x00000050u)
#define WM8978_AUDIO_IF_WL_BIT          (0x00000060u)
#define WM8978_AUDIO_IF_WL_16BIT        (0x00000000u)
/* WM8978_REGADR_CLK_GEN_CTL  R6 */
#define WM8978_CLK_GEN_CTL_INI_VALUE    (0x00000040u)
#define WM8978_CLK_GEN_CTL_MCLKDIV_BIT  (0x000000e0u)
#define WM8978_CLK_GEN_CTL_MCLKDIV_DIV1 (0x00000000u)
/* WM8978_REGADR_ADC_CTL R14 */
#define WM8978_ADC_CTL_INI_VALUE        (0x00000100u)
#define WM8978_ADC_CTL_HPFEN_BIT        (0x00000100u)
#define WM8978_ADC_CTL_ADCOSR128_ON     (0x00000008u)
/* WM8978_REGADR_DAC_CTL R10 */
#define WM8978_DAC_CTL_INI_VALUE        (0x00000000u)
#define WM8978_DAC_CTL_DACOSR128_ON     (0x00000008u)
/* WM8978_REGADR_INPUT_CTL R44 */
#define WM8978_INPUTCTL_INI_VALUE       (0x00000033u)
#define WM8978_INPUTCTL_L2_2INPPGA_ON   (0x00000004u)
#define WM8978_INPUTCTL_R2_2INPPGA_ON   (0x00000040u)
/* WM8978_REGADR_LINPPGAGAIN R45 */
#define WM8978_LINPPGAGAIN_INI_VOLL     (0x00000018u)
#define WM8978_LINPPGAGAIN_MUTEL_ON     (0x00000040u)
/* WM8978_REGADR_RINPPGAGAIN R46 */
#define WM8978_RINPPGAGAIN_INI_VOLL     (0x00000018u)
#define WM8978_RINPPGAGAIN_MUTER_BIT    (0x00000040u)
/* WM8978_REGADR_LMIX_CTL R50 */
#define WM8978_LMIX_CTL_INI_VALUE       (0x00000001u)
#define WM8978_LMIX_CTL_DACL2LMIX_BIT   (0x00000001u)
#define WM8978_LMIX_CTL_BYPL2LMIX_BIT   (0x00000002u)
/* WM8978_REGADR_RMIX_CTL R51 */
#define WM8978_RMIX_CTL_INI_VALUE       (0x00000001u)
#define WM8978_RMIX_CTL_DACR2RMIX_BIT   (0x00000001u)
#define WM8978_RMIX_CTL_BYPR2RMIX_BIT   (0x00000002u)
/* WM8978_REGADR_LOUT1_VOL_CTL R52 */
#define WM8978_LOUT1_HPVU_BIT           (0x00000100u)
/* WM8978_REGADR_ROUT1_VOL_CTL R53 */
#define WM8978_ROUT1_HPVU_BIT           (0x00000100u)


#define WM8978_ADDR 0x1A
static uint16_t wm8978_data[WM8978_REG_MAX]=
{
     0x000,      /*  Software Reset  */
     0x000,      /*  Power manage’t 1    */
     0x000,      /*  Power manage’t 2    */
     0x000,      /*  Power manage’t 3    */
     0x050,      /*  Audio Interface */
     0x000,      /*  Companding ctrl */
     0x140,      /*  Clock Gen ctrl  */
     0x000,      /*  Additional ctrl */
     0x000,      /*  GPIO Stuff  */
     0x000,      /*  Jack detect control */
     0x000,      /*  DAC Control */
     0x0FF,      /*  Left DAC digital Vol    */
     0x0FF,      /*  Right DAC dig’l Vol */
     0x000,      /*  ack Detect Control  */
     0x100,      /*  ADC Control */
     0x0FF,      /*  Left ADC Digital Vol    */
     0x0FF,      /*  Right ADC Digital Vol   */
     0x000,      /*  000 */
     0x12C,      /*  EQ1 – low shelf */
     0x02C,      /*  EQ2 – peak 1    */
     0x02C,      /*  EQ3 – peak 2    */
     0x02C,      /*  EQ4 – peak 3    */
     0x02C,      /*  EQ5 – high shelf    */
     0x000,      /*  000 */
     0x032,      /*  DAC Limiter 1   */
     0x000,      /*  DAC Limiter 2   */
     0x000,      /*  000 */
     0x000,      /*  Notch Filter 1  */
     0x000,      /*  Notch Filter 2  */
     0x000,      /*  Notch Filter 3  */
     0x000,      /*  Notch Filter 4  */
     0x000,      /*  000 */
     0x038,      /*  ALC control 1   */
     0x00B,      /*  ALC control 2   */
     0x032,      /*  ALC control 3   */
     0x000,      /*  Noise Gat   */
     0x008,      /*  PLL N 0 */
     0x00C,      /*  PLL K 1 */
     0x093,      /*  PLL K 2 */
     0x0E9,      /*  PLL K 3 */
     0x000,      /*  000 */
     0x000,      /*  3D control  */
     0x000,      /*  000 */
     0x000,      /*  Beep control    */
     0x003,      /*  Input ctrl  */
     0x010,      /*  Left INP PGA gain ctrl  */
     0x010,      /*  Right INP PGA gain ctrl */
     0x100,      /*  Left ADC Boost ctrl */
     0x100,      /*  Right ADC Boost ctrl    */
     0x002,      /*  Output ctrl */
     0x001,      /*  Left mixer ctrl */
     0x001,      /*  Right mixer ctrl    */
     0x039,      /*  LOUT1 (HP) volume ctrl  */
     0x039,      /*  ROUT1 (HP) volume ctrl  */
     0x039,      /*  LOUT2 (SPK) volume ctrl */
     0x039,      /*  ROUT2 (SPK) volume ctrl */
     0x001,      /*  OUT3 mixer ctrl */
     0x001,      /*  OUT4 (MONO) mixer ctrl  */
 };


static i2c_master_instance_t *p_wm8978_i2c_instance;
static volatile uint32_t wm8978_wait;
static i2c_master_event_t wm8978_event;

static void wm8978_send_data(uint8_t addr, uint16_t data);

static char* const i2c_event_string[]=
{
 "None",
 "Aborted",
 "RX Complete",
 "TX Complete"
};

void wm8978_init(i2c_master_instance_t *pi2c_instance)
{
    /* open i2c */
    p_wm8978_i2c_instance = pi2c_instance;
    p_wm8978_i2c_instance->p_api->open(p_wm8978_i2c_instance->p_ctrl, p_wm8978_i2c_instance->p_cfg);

    /* set initialize data */
    /* ==== Soft Reset. ==== */
    wm8978_send_data(WM8978_REG_SOFTWARE_RESET, wm8978_data[WM8978_REG_SOFTWARE_RESET]);

    /* ==== Set L/RMIXEN = 1 and DACENL/R = 1 in register R3.  ==== */
    wm8978_data[WM8978_REG_POWER_MANAGENT_3] = WM8978_MANAGE3_INI_VALUE | WM8978_MANAGE3_RMIXEN_ON | WM8978_MANAGE3_LMIXEN_ON | WM8978_MANAGE3_DACENL_ON | WM8978_MANAGE3_DACENR_ON;
    wm8978_send_data(WM8978_REG_POWER_MANAGENT_3, wm8978_data[WM8978_REG_POWER_MANAGENT_3]);

    /* ==== Set BUFIOEN = 1 and VMIDSEL[1:0] to required value in register R1. ==== */
    wm8978_data[WM8978_REG_POWER_MANAGENT_1] = WM8978_MANAGE1_INI_VALUE | WM8978_MANAGE1_BUFIOEN_ON | WM8978_MANAGE1_VMIDSEL_75K | WM8978_MANAGE1_MICBEN_ON | WM8978_MANAGE1_PLLEN_ON;
    wm8978_send_data(WM8978_REG_POWER_MANAGENT_1, wm8978_data[WM8978_REG_POWER_MANAGENT_1]);

    /* ==== Set BIASEN = 1 in register R1.  ==== */
    wm8978_data[WM8978_REG_POWER_MANAGENT_1] |= WM8978_MANAGE1_BIASEN_ON;
    wm8978_send_data(WM8978_REG_POWER_MANAGENT_1, wm8978_data[WM8978_REG_POWER_MANAGENT_1]);

    /* ==== Set L/ROUT1EN = 1 in register R2.  ==== */
    wm8978_data[WM8978_REG_POWER_MANAGENT_2] = WM8978_MANAGE2_INI_VALUE | WM8978_MANAGE2_LOUT1EN_ON | WM8978_MANAGE2_ROUT1EN_ON | WM8978_MANAGE2_BOOSTENL_ON | WM8978_MANAGE2_BOOSTENR_ON;
    wm8978_send_data(WM8978_REG_POWER_MANAGENT_2, wm8978_data[WM8978_REG_POWER_MANAGENT_2]);

    /* ==== Set INPPGAENL = 1 in register R2.  ==== */
    wm8978_data[WM8978_REG_POWER_MANAGENT_2] |= WM8978_MANAGE2_INPPGAENL_ON;
    wm8978_send_data(WM8978_REG_POWER_MANAGENT_2, wm8978_data[WM8978_REG_POWER_MANAGENT_2]);

    /* ==== Set INPPGAENR = 1 in register R2.  ==== */
    wm8978_data[WM8978_REG_POWER_MANAGENT_2] |= WM8978_MANAGE2_INPPGAENR_ON;
    wm8978_send_data(WM8978_REG_POWER_MANAGENT_2, wm8978_data[WM8978_REG_POWER_MANAGENT_2]);

    /* ==== Set L2_2INPPGA = 1 and R2_2INPPGA = 1 in register R44.  ==== */
    wm8978_data[WM8978_REG_INPUT_CTRL] = WM8978_INPUTCTL_INI_VALUE | WM8978_INPUTCTL_L2_2INPPGA_ON | WM8978_INPUTCTL_R2_2INPPGA_ON;
    wm8978_send_data(WM8978_REG_INPUT_CTRL, wm8978_data[WM8978_REG_INPUT_CTRL]);

    /* ==== Set LINPPGAGAIN = 0x0018 and MUTEL = 0 in register R45.  ==== */
    wm8978_data[WM8978_REG_LEFT_INP_PGA_GAIN_CTRL] = WM8978_LINPPGAGAIN_INI_VOLL;
    wm8978_data[WM8978_REG_LEFT_INP_PGA_GAIN_CTRL] &= ~WM8978_LINPPGAGAIN_MUTEL_ON;
    wm8978_send_data(WM8978_REG_LEFT_INP_PGA_GAIN_CTRL, wm8978_data[WM8978_REG_LEFT_INP_PGA_GAIN_CTRL]);

    /* ==== Set RINPPGAGAIN = 0x0018 and MUTEL = 0 in register R46.  ==== */
    wm8978_data[WM8978_REG_RIGHT_INP_PGA_GAIN_CTRL] = WM8978_RINPPGAGAIN_INI_VOLL;
    wm8978_data[WM8978_REG_RIGHT_INP_PGA_GAIN_CTRL] &= ~WM8978_RINPPGAGAIN_MUTER_BIT;
    wm8978_send_data(WM8978_REG_RIGHT_INP_PGA_GAIN_CTRL, wm8978_data[WM8978_REG_RIGHT_INP_PGA_GAIN_CTRL]);

    /* ==== Set ADCENL/ADCENR = 1 in register R2.  ==== */
    wm8978_data[WM8978_REG_POWER_MANAGENT_2] |= WM8978_MANAGE2_ADCENL_ON | WM8978_MANAGE2_ADCENR_ON;
    wm8978_send_data(WM8978_REG_POWER_MANAGENT_2, wm8978_data[WM8978_REG_POWER_MANAGENT_2]);

    /* ==== Set ADCOSR128 = 1 in register R14.  ==== */
    wm8978_data[WM8978_REG_ADC_CONTROL] = WM8978_ADC_CTL_INI_VALUE | WM8978_ADC_CTL_ADCOSR128_ON;
    wm8978_send_data(WM8978_REG_ADC_CONTROL, wm8978_data[WM8978_REG_ADC_CONTROL]);

    /* ==== Set HPFEN = 0 in register R14.  ==== */
    wm8978_data[WM8978_REG_ADC_CONTROL] &= ~WM8978_ADC_CTL_HPFEN_BIT;
    wm8978_send_data(WM8978_REG_ADC_CONTROL, wm8978_data[WM8978_REG_ADC_CONTROL]);

    /* ==== Set MCLKDIV = 0 in register R6.  ==== */
    wm8978_data[WM8978_REG_CLOCK_GEN_CTRL] = WM8978_CLK_GEN_CTL_INI_VALUE;
    wm8978_data[WM8978_REG_CLOCK_GEN_CTRL] &= ~WM8978_CLK_GEN_CTL_MCLKDIV_BIT;
    wm8978_data[WM8978_REG_CLOCK_GEN_CTRL] |= WM8978_CLK_GEN_CTL_MCLKDIV_DIV1;
    wm8978_send_data(WM8978_REG_CLOCK_GEN_CTRL, wm8978_data[WM8978_REG_CLOCK_GEN_CTRL]);

    /* ==== Set WL = 0 in register R4.  ==== */
    wm8978_data[WM8978_REG_AUDIO_INTERFACE] = WM8978_AUDIO_IF_INI_VALUE;
    wm8978_data[WM8978_REG_AUDIO_INTERFACE] &= ~WM8978_AUDIO_IF_WL_BIT;
    wm8978_data[WM8978_REG_AUDIO_INTERFACE] |= WM8978_AUDIO_IF_WL_16BIT;
    wm8978_send_data(WM8978_REG_AUDIO_INTERFACE, wm8978_data[WM8978_REG_AUDIO_INTERFACE]);


    /* ==== Set DACOSR128 = 1 in register R10.  ==== */
    wm8978_data[WM8978_REG_DAC_CONTROL] = WM8978_DAC_CTL_INI_VALUE;
    wm8978_data[WM8978_REG_DAC_CONTROL] |= WM8978_DAC_CTL_DACOSR128_ON;
    wm8978_send_data(WM8978_REG_DAC_CONTROL, wm8978_data[WM8978_REG_DAC_CONTROL]);


}

static void wm8978_send_data(uint8_t addr, uint16_t data)
{
    uint8_t send_data[2];
    fsp_err_t err = FSP_ERR_INVALID_DATA;

    send_data[0] = ((addr << 1) & 0xFE) | ((data >> 8) & 0x01);
    send_data[1] = (uint8_t)data & 0xFF;


    wm8978_wait = 0;
    p_wm8978_i2c_instance->p_api->slaveAddressSet(p_wm8978_i2c_instance->p_ctrl,WM8978_ADDR,I2C_MASTER_ADDR_MODE_7BIT );
    err = p_wm8978_i2c_instance->p_api->write(p_wm8978_i2c_instance->p_ctrl, send_data, sizeof(send_data), 0);

    while(wm8978_wait == 0);

    while(wm8978_event == I2C_MASTER_EVENT_ABORTED)
    {
        wm8978_wait = 0;
        err = p_wm8978_i2c_instance->p_api->write(p_wm8978_i2c_instance->p_ctrl, send_data, 0, 0);
        while(wm8978_wait == 0);
    }

//@@@    printf("I2C {0x%02X,0x%02X) finished:%d,%s\n\r",send_data[0],send_data[1],err,i2c_event_string[wm8978_event]);
}


void wm8978_communication_callback(i2c_master_callback_args_t *p_args)
{
    wm8978_wait = 1;
    wm8978_event = p_args->event;
}


