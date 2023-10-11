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

#ifndef ADV7513_DATA_H
#define ADV7513_DATA_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
struct reg_value{
    uint8_t  reg;
    uint8_t  val;
};

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
static const struct reg_value adv7513_init_setting[] = {
    { 0x41, 0x10 },
    { 0x98, 0x03 },
    { 0x9a, 0xe0 },
    { 0x9c, 0x30 },
    { 0x9D, 0x61 },
    { 0xa2, 0xa4 },
    { 0xa3, 0xa4 },
    { 0xe0, 0xd0 },
    { 0xf9, 0x00 },
    { 0x55, 0x02 },
    { 0x44, 0x00 },
    { 0x43, 0x7e },
    { 0x45, 0x70 },
    { 0xe1, 0x78 },
    { 0x16, 0x38 },
    { 0x48, 0x10 },
    { 0xD0, 0x0C },
    { 0xba, 0x60 },
    { 0xe2, 0x01 },
    { 0xaf, 0x14 },
    { 0x96, 0x80 },
    { 0x98, 0x03 },
    { 0x9A, 0xE0 },
    { 0x9C, 0x30 },
    { 0x9D, 0x61 },
    { 0xA2, 0xA4 },
    { 0xA3, 0xA4 },
    { 0xE0, 0xD0 },
    { 0xF9, 0x00 },
    { 0x55, 0x02 },
    { 0x16, 0x38 },
    { 0x41, 0x10 },
    { 0x44, 0x10 },
    { 0x48, 0x10 },
    { 0xAF, 0x16 },
    { 0xBA, 0x60 },
    { 0xD0, 0x0C },
    { 0xD6, 0xC0 },
};
#endif                                 /* ADV7513_DATA_H */
