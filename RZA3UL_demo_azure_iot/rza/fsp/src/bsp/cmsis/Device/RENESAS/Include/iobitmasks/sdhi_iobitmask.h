/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

/**********************************************************************************************************************
 * File Name    : sdhi_iobitmask.h
 * Version      : 1.00
 * Description  : IO bit mask file for sdhi.
 *********************************************************************************************************************/

#ifndef SDHI_IOBITMASK_H
#define SDHI_IOBITMASK_H

#define R_SDHI_SD_CMD_CF4_Msk                         (0x000000000000003FUL)
#define R_SDHI_SD_CMD_CF4_Pos                         (0UL)
#define R_SDHI_SD_CMD_C_Msk                           (0x00000000000000C0UL)
#define R_SDHI_SD_CMD_C_Pos                           (6UL)
#define R_SDHI_SD_CMD_MD_Msk                          (0x000000000000FF00UL)
#define R_SDHI_SD_CMD_MD_Pos                          (8UL)
#define R_SDHI_SD_ARG_CF_Msk                          (0x00000000FFFFFFFFUL)
#define R_SDHI_SD_ARG_CF_Pos                          (0UL)
#define R_SDHI_SD_ARG1_CF_Msk                         (0x000000000000FFFFUL)
#define R_SDHI_SD_ARG1_CF_Pos                         (0UL)
#define R_SDHI_SD_STOP_STP_Msk                        (0x0000000000000001UL)
#define R_SDHI_SD_STOP_STP_Pos                        (0UL)
#define R_SDHI_SD_STOP_SEC_Msk                        (0x0000000000000100UL)
#define R_SDHI_SD_STOP_SEC_Pos                        (8UL)
#define R_SDHI_SD_STOP_HPICMD_Msk                     (0x0000000000010000UL)
#define R_SDHI_SD_STOP_HPICMD_Pos                     (16UL)
#define R_SDHI_SD_STOP_HPIMODE_Msk                    (0x0000000000020000UL)
#define R_SDHI_SD_STOP_HPIMODE_Pos                    (17UL)
#define R_SDHI_SD_SECCNT_CNT_Msk                      (0x00000000FFFFFFFFUL)
#define R_SDHI_SD_SECCNT_CNT_Pos                      (0UL)
#define R_SDHI_SD_RSP10_R_Msk                         (0xFFFFFFFFFFFFFFFFUL)
#define R_SDHI_SD_RSP10_R_Pos                         (0UL)
#define R_SDHI_SD_RSP1_R_Msk                          (0x000000000000FFFFUL)
#define R_SDHI_SD_RSP1_R_Pos                          (0UL)
#define R_SDHI_SD_RSP32_R_Msk                         (0x00000000FFFFFFFFUL)
#define R_SDHI_SD_RSP32_R_Pos                         (0UL)
#define R_SDHI_SD_RSP3_R_Msk                          (0x000000000000FFFFUL)
#define R_SDHI_SD_RSP3_R_Pos                          (0UL)
#define R_SDHI_SD_RSP54_R_Msk                         (0x00FFFFFFFFFFFFFFUL)
#define R_SDHI_SD_RSP54_R_Pos                         (0UL)
#define R_SDHI_SD_RSP5_R_Msk                          (0x000000000000FFFFUL)
#define R_SDHI_SD_RSP5_R_Pos                          (0UL)
#define R_SDHI_SD_RSP76_R_Msk                         (0x0000000000FFFFFFUL)
#define R_SDHI_SD_RSP76_R_Pos                         (0UL)
#define R_SDHI_SD_RSP7_R_Msk                          (0x00000000000000FFUL)
#define R_SDHI_SD_RSP7_R_Pos                          (0UL)
#define R_SDHI_SD_INFO1_INFO0_Msk                     (0x0000000000000001UL)
#define R_SDHI_SD_INFO1_INFO0_Pos                     (0UL)
#define R_SDHI_SD_INFO1_INFO2_Msk                     (0x0000000000000004UL)
#define R_SDHI_SD_INFO1_INFO2_Pos                     (2UL)
#define R_SDHI_SD_INFO1_INFO3_Msk                     (0x0000000000000008UL)
#define R_SDHI_SD_INFO1_INFO3_Pos                     (3UL)
#define R_SDHI_SD_INFO1_INFO4_Msk                     (0x0000000000000010UL)
#define R_SDHI_SD_INFO1_INFO4_Pos                     (4UL)
#define R_SDHI_SD_INFO1_INFO5_Msk                     (0x0000000000000020UL)
#define R_SDHI_SD_INFO1_INFO5_Pos                     (5UL)
#define R_SDHI_SD_INFO1_INFO7_Msk                     (0x0000000000000080UL)
#define R_SDHI_SD_INFO1_INFO7_Pos                     (7UL)
#define R_SDHI_SD_INFO1_INFO8_Msk                     (0x0000000000000100UL)
#define R_SDHI_SD_INFO1_INFO8_Pos                     (8UL)
#define R_SDHI_SD_INFO1_INFO9_Msk                     (0x0000000000000200UL)
#define R_SDHI_SD_INFO1_INFO9_Pos                     (9UL)
#define R_SDHI_SD_INFO1_INFO10_Msk                    (0x0000000000000400UL)
#define R_SDHI_SD_INFO1_INFO10_Pos                    (10UL)
#define R_SDHI_SD_INFO1_HPIRES_Msk                    (0x0000000000010000UL)
#define R_SDHI_SD_INFO1_HPIRES_Pos                    (16UL)
#define R_SDHI_SD_INFO2_ERR_Msk                       (0x000000000000007FUL)
#define R_SDHI_SD_INFO2_ERR_Pos                       (0UL)
#define R_SDHI_SD_INFO2_DAT0_Msk                      (0x0000000000000080UL)
#define R_SDHI_SD_INFO2_DAT0_Pos                      (7UL)
#define R_SDHI_SD_INFO2_BRE_Msk                       (0x0000000000000100UL)
#define R_SDHI_SD_INFO2_BRE_Pos                       (8UL)
#define R_SDHI_SD_INFO2_BWE_Msk                       (0x0000000000000200UL)
#define R_SDHI_SD_INFO2_BWE_Pos                       (9UL)
#define R_SDHI_SD_INFO2_SCLKDIVEN_Msk                 (0x0000000000002000UL)
#define R_SDHI_SD_INFO2_SCLKDIVEN_Pos                 (13UL)
#define R_SDHI_SD_INFO2_CBSY_Msk                      (0x0000000000004000UL)
#define R_SDHI_SD_INFO2_CBSY_Pos                      (14UL)
#define R_SDHI_SD_INFO2_ILA_Msk                       (0x0000000000008000UL)
#define R_SDHI_SD_INFO2_ILA_Pos                       (15UL)
#define R_SDHI_SD_INFO1_MASK_IMASK0_Msk               (0x0000000000000001UL)
#define R_SDHI_SD_INFO1_MASK_IMASK0_Pos               (0UL)
#define R_SDHI_SD_INFO1_MASK_IMASK2_Msk               (0x0000000000000004UL)
#define R_SDHI_SD_INFO1_MASK_IMASK2_Pos               (2UL)
#define R_SDHI_SD_INFO1_MASK_IMASK3_Msk               (0x0000000000000008UL)
#define R_SDHI_SD_INFO1_MASK_IMASK3_Pos               (3UL)
#define R_SDHI_SD_INFO1_MASK_IMASK4_Msk               (0x0000000000000010UL)
#define R_SDHI_SD_INFO1_MASK_IMASK4_Pos               (4UL)
#define R_SDHI_SD_INFO1_MASK_IMASK8_Msk               (0x0000000000000100UL)
#define R_SDHI_SD_INFO1_MASK_IMASK8_Pos               (8UL)
#define R_SDHI_SD_INFO1_MASK_IMASK9_Msk               (0x0000000000000200UL)
#define R_SDHI_SD_INFO1_MASK_IMASK9_Pos               (9UL)
#define R_SDHI_SD_INFO1_MASK_IMASK16_Msk              (0x0000000000010000UL)
#define R_SDHI_SD_INFO1_MASK_IMASK16_Pos              (16UL)
#define R_SDHI_SD_INFO2_MASK_EMASK_Msk                (0x000000000000007FUL)
#define R_SDHI_SD_INFO2_MASK_EMASK_Pos                (0UL)
#define R_SDHI_SD_INFO2_MASK_BMASK_Msk                (0x0000000000000300UL)
#define R_SDHI_SD_INFO2_MASK_BMASK_Pos                (8UL)
#define R_SDHI_SD_INFO2_MASK_IMASK_Msk                (0x0000000000008000UL)
#define R_SDHI_SD_INFO2_MASK_IMASK_Pos                (15UL)
#define R_SDHI_SD_CLK_CTRL_DIV_Msk                    (0x00000000000000FFUL)
#define R_SDHI_SD_CLK_CTRL_DIV_Pos                    (0UL)
#define R_SDHI_SD_CLK_CTRL_SCLKEN_Msk                 (0x0000000000000100UL)
#define R_SDHI_SD_CLK_CTRL_SCLKEN_Pos                 (8UL)
#define R_SDHI_SD_CLK_CTRL_SDCLKOFFEN_Msk             (0x0000000000000200UL)
#define R_SDHI_SD_CLK_CTRL_SDCLKOFFEN_Pos             (9UL)
#define R_SDHI_SD_SIZE_LEN_Msk                        (0x00000000000003FFUL)
#define R_SDHI_SD_SIZE_LEN_Pos                        (0UL)
#define R_SDHI_SD_OPTION_CTOP_Msk                     (0x000000000000000FUL)
#define R_SDHI_SD_OPTION_CTOP_Pos                     (0UL)
#define R_SDHI_SD_OPTION_TOP_Msk                      (0x00000000000000F0UL)
#define R_SDHI_SD_OPTION_TOP_Pos                      (4UL)
#define R_SDHI_SD_OPTION_TOUTMASK_Msk                 (0x0000000000000100UL)
#define R_SDHI_SD_OPTION_TOUTMASK_Pos                 (8UL)
#define R_SDHI_SD_OPTION_EXTOP_Msk                    (0x0000000000000200UL)
#define R_SDHI_SD_OPTION_EXTOP_Pos                    (9UL)
#define R_SDHI_SD_OPTION_WIDTH8_Msk                   (0x0000000000002000UL)
#define R_SDHI_SD_OPTION_WIDTH8_Pos                   (13UL)
#define R_SDHI_SD_OPTION_WIDTH_Msk                    (0x0000000000008000UL)
#define R_SDHI_SD_OPTION_WIDTH_Pos                    (15UL)
#define R_SDHI_SD_ERR_STS1_E0_Msk                     (0x0000000000000001UL)
#define R_SDHI_SD_ERR_STS1_E0_Pos                     (0UL)
#define R_SDHI_SD_ERR_STS1_E1_Msk                     (0x0000000000000002UL)
#define R_SDHI_SD_ERR_STS1_E1_Pos                     (1UL)
#define R_SDHI_SD_ERR_STS1_E2_Msk                     (0x0000000000000004UL)
#define R_SDHI_SD_ERR_STS1_E2_Pos                     (2UL)
#define R_SDHI_SD_ERR_STS1_E3_Msk                     (0x0000000000000008UL)
#define R_SDHI_SD_ERR_STS1_E3_Pos                     (3UL)
#define R_SDHI_SD_ERR_STS1_E4_Msk                     (0x0000000000000010UL)
#define R_SDHI_SD_ERR_STS1_E4_Pos                     (4UL)
#define R_SDHI_SD_ERR_STS1_E5_Msk                     (0x0000000000000020UL)
#define R_SDHI_SD_ERR_STS1_E5_Pos                     (5UL)
#define R_SDHI_SD_ERR_STS1_E8_Msk                     (0x0000000000000100UL)
#define R_SDHI_SD_ERR_STS1_E8_Pos                     (8UL)
#define R_SDHI_SD_ERR_STS1_E9_Msk                     (0x0000000000000200UL)
#define R_SDHI_SD_ERR_STS1_E9_Pos                     (9UL)
#define R_SDHI_SD_ERR_STS1_E10_Msk                    (0x0000000000000400UL)
#define R_SDHI_SD_ERR_STS1_E10_Pos                    (10UL)
#define R_SDHI_SD_ERR_STS1_E11_Msk                    (0x0000000000000800UL)
#define R_SDHI_SD_ERR_STS1_E11_Pos                    (11UL)
#define R_SDHI_SD_ERR_STS1_E12_Msk                    (0x0000000000001000UL)
#define R_SDHI_SD_ERR_STS1_E12_Pos                    (12UL)
#define R_SDHI_SD_ERR_STS1_E13_Msk                    (0x0000000000002000UL)
#define R_SDHI_SD_ERR_STS1_E13_Pos                    (13UL)
#define R_SDHI_SD_ERR_STS1_E14_Msk                    (0x0000000000004000UL)
#define R_SDHI_SD_ERR_STS1_E14_Pos                    (14UL)
#define R_SDHI_SD_ERR_STS2_E_Msk                      (0x000000000000007FUL)
#define R_SDHI_SD_ERR_STS2_E_Pos                      (0UL)
#define R_SDHI_SD_BUF0_BUF_Msk                        (0xFFFFFFFFFFFFFFFFUL)
#define R_SDHI_SD_BUF0_BUF_Pos                        (0UL)
#define R_SDHI_SDIO_MODE_IOMOD_Msk                    (0x0000000000000001UL)
#define R_SDHI_SDIO_MODE_IOMOD_Pos                    (0UL)
#define R_SDHI_SDIO_MODE_RWREQ_Msk                    (0x0000000000000004UL)
#define R_SDHI_SDIO_MODE_RWREQ_Pos                    (2UL)
#define R_SDHI_SDIO_MODE_IOABT_Msk                    (0x0000000000000100UL)
#define R_SDHI_SDIO_MODE_IOABT_Pos                    (8UL)
#define R_SDHI_SDIO_MODE_C52PUB_Msk                   (0x0000000000000200UL)
#define R_SDHI_SDIO_MODE_C52PUB_Pos                   (9UL)
#define R_SDHI_SDIO_INFO1_IOIRQ_Msk                   (0x0000000000000001UL)
#define R_SDHI_SDIO_INFO1_IOIRQ_Pos                   (0UL)
#define R_SDHI_SDIO_INFO1_EXPUB52_Msk                 (0x0000000000004000UL)
#define R_SDHI_SDIO_INFO1_EXPUB52_Pos                 (14UL)
#define R_SDHI_SDIO_INFO1_EXWT_Msk                    (0x0000000000008000UL)
#define R_SDHI_SDIO_INFO1_EXWT_Pos                    (15UL)
#define R_SDHI_SDIO_INFO1_MASK_IOMSK_Msk              (0x0000000000000001UL)
#define R_SDHI_SDIO_INFO1_MASK_IOMSK_Pos              (0UL)
#define R_SDHI_SDIO_INFO1_MASK_MEXPUB52_Msk           (0x0000000000004000UL)
#define R_SDHI_SDIO_INFO1_MASK_MEXPUB52_Pos           (14UL)
#define R_SDHI_SDIO_INFO1_MASK_MEXWT_Msk              (0x0000000000008000UL)
#define R_SDHI_SDIO_INFO1_MASK_MEXWT_Pos              (15UL)
#define R_SDHI_CC_EXT_MODE_DMASDRW_Msk                (0x0000000000000002UL)
#define R_SDHI_CC_EXT_MODE_DMASDRW_Pos                (1UL)
#define R_SDHI_SOFT_RST_SDRST_Msk                     (0x0000000000000001UL)
#define R_SDHI_SOFT_RST_SDRST_Pos                     (0UL)
#define R_SDHI_VERSION_IP_Msk                         (0x00000000000000FFUL)
#define R_SDHI_VERSION_IP_Pos                         (0UL)
#define R_SDHI_VERSION_UR_Msk                         (0x000000000000FF00UL)
#define R_SDHI_VERSION_UR_Pos                         (8UL)
#define R_SDHI_HOST_MODE_WMODE_Msk                    (0x0000000000000001UL)
#define R_SDHI_HOST_MODE_WMODE_Pos                    (0UL)
#define R_SDHI_HOST_MODE_ENDIAN_Msk                   (0x0000000000000002UL)
#define R_SDHI_HOST_MODE_ENDIAN_Pos                   (1UL)
#define R_SDHI_HOST_MODE_BUSWIDTH_Msk                 (0x0000000000000100UL)
#define R_SDHI_HOST_MODE_BUSWIDTH_Pos                 (8UL)
#define R_SDHI_SDIF_MODE_DDR_Msk                      (0x0000000000000001UL)
#define R_SDHI_SDIF_MODE_DDR_Pos                      (0UL)
#define R_SDHI_SDIF_MODE_NOCHKCR_Msk                  (0x0000000000000100UL)
#define R_SDHI_SDIF_MODE_NOCHKCR_Pos                  (8UL)
#define R_SDHI_SD_STATUS_SD_RST_Msk                   (0x0000000000000002UL)
#define R_SDHI_SD_STATUS_SD_RST_Pos                   (1UL)
#define R_SDHI_DM_CM_DTRAN_MODE_BUS_WIDTH_Msk         (0x0000000000000030UL)
#define R_SDHI_DM_CM_DTRAN_MODE_BUS_WIDTH_Pos         (4UL)
#define R_SDHI_DM_CM_DTRAN_MODE_CH_NUM_Msk            (0x0000000000030000UL)
#define R_SDHI_DM_CM_DTRAN_MODE_CH_NUM_Pos            (16UL)
#define R_SDHI_DM_CM_DTRAN_CTRL_DM_START_Msk          (0x0000000000000001UL)
#define R_SDHI_DM_CM_DTRAN_CTRL_DM_START_Pos          (0UL)
#define R_SDHI_DM_CM_RST_SEQRST_Msk                   (0x0000000000000001UL)
#define R_SDHI_DM_CM_RST_SEQRST_Pos                   (0UL)
#define R_SDHI_DM_CM_RST_DTRANRST_Msk                 (0x0000000000000300UL)
#define R_SDHI_DM_CM_RST_DTRANRST_Pos                 (8UL)
#define R_SDHI_DM_CM_INFO1_SEQEND_Msk                 (0x0000000000000001UL)
#define R_SDHI_DM_CM_INFO1_SEQEND_Pos                 (0UL)
#define R_SDHI_DM_CM_INFO1_DTRANEND0_Msk              (0x0000000000010000UL)
#define R_SDHI_DM_CM_INFO1_DTRANEND0_Pos              (16UL)
#define R_SDHI_DM_CM_INFO1_DTRANEND1_Msk              (0x0000000000100000UL)
#define R_SDHI_DM_CM_INFO1_DTRANEND1_Pos              (20UL)
#define R_SDHI_DM_CM_INFO1_MASK_SEQEND_MASK_Msk       (0x0000000000000001UL)
#define R_SDHI_DM_CM_INFO1_MASK_SEQEND_MASK_Pos       (0UL)
#define R_SDHI_DM_CM_INFO1_MASK_DTRANEND0_MASK_Msk    (0x0000000000010000UL)
#define R_SDHI_DM_CM_INFO1_MASK_DTRANEND0_MASK_Pos    (16UL)
#define R_SDHI_DM_CM_INFO1_MASK_DTRANEND1_MASK_Msk    (0x0000000000100000UL)
#define R_SDHI_DM_CM_INFO1_MASK_DTRANEND1_MASK_Pos    (20UL)
#define R_SDHI_DM_CM_INFO2_SEQERR_Msk                 (0x0000000000000001UL)
#define R_SDHI_DM_CM_INFO2_SEQERR_Pos                 (0UL)
#define R_SDHI_DM_CM_INFO2_DTRANERR0_Msk              (0x0000000000010000UL)
#define R_SDHI_DM_CM_INFO2_DTRANERR0_Pos              (16UL)
#define R_SDHI_DM_CM_INFO2_DTRANERR1_Msk              (0x0000000000020000UL)
#define R_SDHI_DM_CM_INFO2_DTRANERR1_Pos              (17UL)
#define R_SDHI_DM_CM_INFO2_MASK_SEQERR_MASK_Msk       (0x0000000000000001UL)
#define R_SDHI_DM_CM_INFO2_MASK_SEQERR_MASK_Pos       (0UL)
#define R_SDHI_DM_CM_INFO2_MASK_DTRANERR0_MASK_Msk    (0x0000000000010000UL)
#define R_SDHI_DM_CM_INFO2_MASK_DTRANERR0_MASK_Pos    (16UL)
#define R_SDHI_DM_CM_INFO2_MASK_DTRANERR1_MASK_Msk    (0x0000000000020000UL)
#define R_SDHI_DM_CM_INFO2_MASK_DTRANERR1_MASK_Pos    (17UL)
#define R_SDHI_DM_DTRAN_ADDR_DADDR_Msk                (0x00000000FFFFFFF8UL)
#define R_SDHI_DM_DTRAN_ADDR_DADDR_Pos                (3UL)
#define R_SDHI_SCC_DTCNTL_TAPEN_Msk                   (0x0000000000000001UL)
#define R_SDHI_SCC_DTCNTL_TAPEN_Pos                   (0UL)
#define R_SDHI_SCC_DTCNTL_TAPNUM_Msk                  (0x0000000000FF0000UL)
#define R_SDHI_SCC_DTCNTL_TAPNUM_Pos                  (16UL)
#define R_SDHI_SCC_TAPSET_TAPSET_Msk                  (0x00000000000000FFUL)
#define R_SDHI_SCC_TAPSET_TAPSET_Pos                  (0UL)
#define R_SDHI_SCC_DT2FF_DT2NS_Msk                    (0x00000000000000FFUL)
#define R_SDHI_SCC_DT2FF_DT2NS_Pos                    (0UL)
#define R_SDHI_SCC_DT2FF_DT2NE_Msk                    (0x000000000000FF00UL)
#define R_SDHI_SCC_DT2FF_DT2NE_Pos                    (8UL)
#define R_SDHI_SCC_CKSEL_DTSEL_Msk                    (0x0000000000000001UL)
#define R_SDHI_SCC_CKSEL_DTSEL_Pos                    (0UL)
#define R_SDHI_SCC_RVSCNTL_RVSEN_Msk                  (0x0000000000000001UL)
#define R_SDHI_SCC_RVSCNTL_RVSEN_Pos                  (0UL)
#define R_SDHI_SCC_RVSCNTL_RVSW_Msk                   (0x0000000000000002UL)
#define R_SDHI_SCC_RVSCNTL_RVSW_Pos                   (1UL)
#define R_SDHI_SCC_RVSCNTL_TAPSEL_Msk                 (0x000000000000FF00UL)
#define R_SDHI_SCC_RVSCNTL_TAPSEL_Pos                 (8UL)
#define R_SDHI_SCC_RVSREQ_REQTAPDWN_Msk               (0x0000000000000001UL)
#define R_SDHI_SCC_RVSREQ_REQTAPDWN_Pos               (0UL)
#define R_SDHI_SCC_RVSREQ_REQTAPUP_Msk                (0x0000000000000002UL)
#define R_SDHI_SCC_RVSREQ_REQTAPUP_Pos                (1UL)
#define R_SDHI_SCC_RVSREQ_RVSERR_Msk                  (0x0000000000000004UL)
#define R_SDHI_SCC_RVSREQ_RVSERR_Pos                  (2UL)
#define R_SDHI_SCC_SMPCMP_CMPNGD_Msk                  (0x00000000000001FFUL)
#define R_SDHI_SCC_SMPCMP_CMPNGD_Pos                  (0UL)
#define R_SDHI_SCC_SMPCMP_CMPNGU_Msk                  (0x0000000001FF0000UL)
#define R_SDHI_SCC_SMPCMP_CMPNGU_Pos                  (16UL)
#define R_SDHI_SCC_TMPPORT_TMPOUT_Msk                 (0x000000000000FFFFUL)
#define R_SDHI_SCC_TMPPORT_TMPOUT_Pos                 (0UL)

#endif