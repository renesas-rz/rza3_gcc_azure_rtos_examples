/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "hal_data.h"
#include <unistd.h>
#include "adv7513.h"
#include "adv7513_data.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
typedef struct
{
    __IOM uint32_t IEN0E;
} R_IIC_IEN0E_Type;

#define R_IIC_IEN0E  ((R_IIC_IEN0E_Type *) 0x11031870)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static void iic_master_init_1(void);
static void adv7513_set_reg_array_byte(const struct reg_value *settings, uint16_t num_settings, uint8_t slave_addr_wr);
static void adv7513_set_reg_byte(uint8_t reg, uint8_t val, uint8_t slave_addr_wr);
static void adv7513_write_reg2(uint8_t reg, uint8_t val);


/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * adv7513_init
 **********************************************************************************************************************/
void adv7513_init( void )
{
    uint8_t slave_addr_wr = 0x72;
    uint16_t num_settings;

    iic_master_init_1();

    num_settings = sizeof(adv7513_init_setting)/sizeof(uint16_t);

    adv7513_set_reg_array_byte(adv7513_init_setting, num_settings, slave_addr_wr);
}

/*******************************************************************************************************************//**
 * iic_master_init_1
 **********************************************************************************************************************/
static void iic_master_init_1(void)
{
    R_IIC_IEN0E->IEN0E = 0x01010000;

    R_BSP_MODULE_START(FSP_IP_RIIC, 1);

    R_BSP_MODULE_CLKON(FSP_IP_RIIC, 1);
    R_BSP_MODULE_RSTOFF(FSP_IP_RIIC, 1);

    R_RIIC1->ICCR1_b.ICE    = 0b0;
    R_RIIC1->ICCR1_b.IICRST = 0b1;
    R_RIIC1->ICCR1_b.ICE    = 0b1;

    R_RIIC1->ICSAR0_hword.L = 0x0055;
    R_RIIC1->ICBRH_byte.LL  = 0x30; //0xca;
    R_RIIC1->ICBRL_byte.LL  = 0x30; //0xca;
    R_RIIC1->ICSER_byte.LL  = 0x01;
    R_RIIC1->ICMR1_byte.LL  = 0x38; //0x08; 20
    R_RIIC1->ICMR2_byte.LL  = 0x26;
    R_RIIC1->ICMR3_byte.LL  = 0x00; //0x00;
    R_RIIC1->ICFER_byte.LL  = 0x40;
    R_RIIC1->ICIER_byte.LL  = 0x00;

    R_RIIC1->ICCR1_b.IICRST = 0b0;
}

/*******************************************************************************************************************//**
 * adv7513_set_reg_array_byte
 **********************************************************************************************************************/
void adv7513_set_reg_array_byte(const struct reg_value *settings, uint16_t num_settings, uint8_t slave_addr_wr)
{
    uint16_t i;

    for(i = 0; i < num_settings; ++i, ++settings)
    {
        iic_master_init_1();
        adv7513_set_reg_byte(settings->reg, settings->val, slave_addr_wr);
    }
}

/*******************************************************************************************************************//**
 * adv7513_set_reg_byte
 **********************************************************************************************************************/
void adv7513_set_reg_byte(uint8_t reg, uint8_t val, uint8_t slave_addr_wr)
{
    R_RIIC1->ICCR2_b.ST    = 0b1;

    while(R_RIIC1->ICSR2_b.START == 0)
    {
        ; /* Do nothing */
    }

    R_RIIC1->ICDRT_byte.LL = slave_addr_wr | 0x00;  /* slave address */

    while(R_RIIC1->ICSR2_b.TEND == 0)
    {
        ; /* Do nothing */
    }

    while(R_RIIC1->ICSR2_b.NACKF == 1)
    {
        ; /* Do nothing */
    }

    adv7513_write_reg2(reg, val);

    R_RIIC1->ICSR2_b.STOP = 0x0;
    R_RIIC1->ICCR2_b.SP   = 0x1;

    while(R_RIIC1->ICSR2_b.STOP != 1)
    {
        ; /* Do nothing */
    }

    R_RIIC1->ICSR2_b.STOP = 0b0;
}

/*******************************************************************************************************************//**
 * adv7513_write_reg2
 **********************************************************************************************************************/
void adv7513_write_reg2(uint8_t reg, uint8_t val)
{
    uint8_t regbuf[2];
    uint32_t  i;

    regbuf[0] = reg;
    regbuf[1] = val;

    R_RIIC1->ICDRT_byte.LL = regbuf[0];

    while(R_RIIC1->ICSR2_b.TEND == 0)
    {
        ; /* Do nothing */
    }

    R_RIIC1->ICDRT_byte.LL = regbuf[1];

    while(R_RIIC1->ICSR2_b.TEND == 0)
    {
        ; /* Do nothing */
    }

    for(i=0; i<200000; i++)
    {
        ; /* Do nothing */
    }
}
