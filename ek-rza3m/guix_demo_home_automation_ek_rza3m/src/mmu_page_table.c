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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_mmu.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define R_MMU_UNCACHE_FLASH_OFFESET    0x10000000
#define R_MMU_UNCACHE_DDR_OFFESET      0x40000000

#define R_MMU_PHYSICAL_FLASH_START     0x20000000
#define R_MMU_PHYSICAL_FLASH_END       0x2FFFFFFF

#define R_MMU_PHYSICAL_DDR_START       0x40000000
#define R_MMU_PHYSICAL_DDR_END         0x7FFFFFFF

#define R_MMU_UNCACHE_FLASH_START      0x30000000
#define R_MMU_UNCACHE_FLASH_END        0x3FFFFFFF

#define R_MMU_UNCACHE_DDR_START        0x80000000
#define R_MMU_UNCACHE_DDR_END          0xBFFFFFFF

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

r_mmu_pgtbl_cfg_t const g_mmu_pagetable_array[] =
{
    /* vaddress, paddress,   size,       attribute */
    {0x00000000, 0x00000000, 0x00200000, R_MMU_PG_ATTRIBUTE_NORMAL_CACHEABLE},
    {0x00200000, 0x00200000, 0x0FE00000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT    },
    {0x10000000, 0x10000000, 0x10000000, R_MMU_PG_ATTRIBUTE_DEVICE          },
    {0x20000000, 0x20000000, 0x10000000, R_MMU_PG_ATTRIBUTE_NORMAL_CACHEABLE},
    {0x30000000, 0x20000000, 0x10000000, R_MMU_PG_ATTRIBUTE_NORMAL_UNCACHE  },
    {0x40000000, 0x40000000, 0x40000000, R_MMU_PG_ATTRIBUTE_NORMAL_CACHEABLE},
    {0x80000000, 0x40000000, 0x40000000, R_MMU_PG_ATTRIBUTE_NORMAL_UNCACHE  },
    {0xC0000000, 0xC0000000, 0x40000000, R_MMU_PG_ATTRIBUTE_ACCESS_FAULT    },
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF                         }
};

/*******************************************************************************************************************//**
 * @addtogroup MMU
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Convert virtual address to physical address.
 *
 * @param[in]  p_api_ctrl        MMU driver control block.
 * @param[in]  vaddress          Virtual address to convert.
 * @param[out] p_paddress        Pointer to store physical address.
 * @retval FSP_SUCCESS           successful
 **********************************************************************************************************************/
fsp_err_t R_MMU_VAtoPA (mmu_ctrl_t * const p_api_ctrl, uint64_t vaddress, uint64_t * p_paddress)
{
    fsp_err_t err = FSP_SUCCESS;

    FSP_PARAMETER_NOT_USED(p_api_ctrl);

    if ((R_MMU_UNCACHE_FLASH_START <= vaddress) && (vaddress <= R_MMU_UNCACHE_FLASH_END))
    {
        *p_paddress = vaddress - R_MMU_UNCACHE_FLASH_OFFESET;
    }
    else if ((R_MMU_UNCACHE_DDR_START <= vaddress) && (vaddress <= R_MMU_UNCACHE_DDR_END))
    {
        *p_paddress = vaddress - R_MMU_UNCACHE_DDR_OFFESET;
    }
    else
    {
        *p_paddress = vaddress;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Convert physical address to virtual address.
 *
 * @param[in]  p_api_ctrl        MMU driver control block.
 * @param[in]  paddress          Physical address to convert.
 * @param[out] p_vaddress        Pointer to store virtual address.
 * @retval FSP_SUCCESS           successful
 **********************************************************************************************************************/
fsp_err_t R_MMU_PAtoVA (mmu_ctrl_t * const p_api_ctrl, uint64_t paddress, uint64_t * p_vaddress)
{
    fsp_err_t err = FSP_SUCCESS;

    FSP_PARAMETER_NOT_USED(p_api_ctrl);

    if ((R_MMU_PHYSICAL_FLASH_START <= paddress) && (paddress <= R_MMU_PHYSICAL_FLASH_END))
    {
        *p_vaddress = paddress + R_MMU_UNCACHE_FLASH_OFFESET;
    }
    else if ((R_MMU_PHYSICAL_DDR_START <= paddress) && (paddress <= R_MMU_PHYSICAL_DDR_END))
    {
        *p_vaddress = paddress + R_MMU_UNCACHE_DDR_OFFESET;
    }
    else
    {
        *p_vaddress = paddress;
    }

    return err;
}

/** @} (end addtogroup MMU) */
