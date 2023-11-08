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

#include "bsp_api.h"

/* List of the section of variables with initial value */
typedef struct
{
    void * rom_s;                      /* Top address on ROM    */
    void * rom_e;                      /* Bottom address on ROM */
    void * ram_s;                      /* Top address on RAM    */
} dtbl_t;

/* List of the section of variables with zero clear */
typedef struct
{
    void * b_s;                        /* Top address    */
    void * b_e;                        /* Bottom address */
} btbl_t;

extern uint16_t __bss_start;
extern uint16_t __bss_end;

extern uint16_t __bss2_start;
extern uint16_t __bss2_end;

extern uint16_t __uncached_RAM_start;
extern uint16_t __uncached_RAM_end;

#if !defined(BSP_SRAM_LOAD)
extern uint16_t __sram_load_start;
extern uint16_t __sram_load_end;
extern uint16_t __sram_exec_start;
#endif

#if defined(BSP_XIP_LOAD)
extern uint16_t __data_load_start;
extern uint16_t __data_load_end;
extern uint16_t __data_exec_start;
#endif

const dtbl_t DTBL[] =
{
/*  {  rom start address,     rom end address,     ram start address    },*/
#if !defined(BSP_SRAM_LOAD)
    {&__sram_load_start, &__sram_load_end, &__sram_exec_start},
#endif
#if defined(BSP_XIP_LOAD)
    {&__data_load_start, &__data_load_end, &__data_exec_start},
#endif
    {0,                  0,                0                 }, /* terminator */
};

const btbl_t BTBL[] =
{
/*  {  bss start address,    bss end address     },*/
    {&__bss_start,          &__bss_end         },
    {&__bss2_start,         &__bss2_end        },
#if !defined(BSP_SRAM_LOAD)
    {&__uncached_RAM_start, &__uncached_RAM_end},
#endif
    {0,                     0                  }, /* terminator */
};

const dtbl_t DTBL_F[] =
{
/*  {  rom start address,     rom end address,     ram start address    },*/
    {0, 0, 0},                         /* terminator */
};

const btbl_t BTBL_F[] =
{
/*  {  bss start address,    bss end address     },*/
    {0, 0},                            /* terminator */
};

/* End of File */
