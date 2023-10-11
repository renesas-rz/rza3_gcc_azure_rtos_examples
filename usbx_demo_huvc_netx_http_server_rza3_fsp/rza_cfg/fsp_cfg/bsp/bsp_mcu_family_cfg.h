/* generated configuration header file - do not edit */
#ifndef BSP_MCU_FAMILY_CFG_H_
#define BSP_MCU_FAMILY_CFG_H_
#include "bsp_mcu_device_pn_cfg.h"
#include "bsp_mcu_device_cfg.h"
#include "bsp_mcu_info.h"
#include "bsp_clock_cfg.h"
#define BSP_MCU_GROUP_RZA3UL (1)
#define BSP_CORTEX_VECTOR_TABLE_ENTRIES    (0U)
#define BSP_VECTOR_TABLE_MAX_ENTRIES       (512U)

#if defined(_RZG2L_TZ_SECURE)
            #define BSP_TZ_SECURE_BUILD           (1)
            #define BSP_TZ_NONSECURE_BUILD        (0)
            #elif defined(_RZG2L_TZ_NONSECURE)
            #define BSP_TZ_SECURE_BUILD           (0)
            #define BSP_TZ_NONSECURE_BUILD        (1)
            #else
#define BSP_TZ_SECURE_BUILD           (0)
#define BSP_TZ_NONSECURE_BUILD        (0)
#endif

/* TrustZone Settings */
#define BSP_TZ_CFG_INIT_SECURE_ONLY       (BSP_CFG_CLOCKS_SECURE || (!BSP_CFG_CLOCKS_OVERRIDE))
#define BSP_TZ_CFG_SKIP_INIT              (BSP_TZ_NONSECURE_BUILD && BSP_TZ_CFG_INIT_SECURE_ONLY)

#define SCB_SCR_SLEEPDEEPS_VAL            (0)
#define SCB_SCR_SLEEPDEEP_VAL             (0)

/* CMSIS TrustZone Settings */
#define SCB_CSR_AIRCR_INIT                (1)
#define SCB_AIRCR_BFHFNMINS_VAL           (1)
#define SCB_AIRCR_SYSRESETREQS_VAL        (0)
#define SCB_AIRCR_PRIS_VAL                (0)
#define TZ_FPU_NS_USAGE                   (1)
#ifndef SCB_NSACR_CP10_11_VAL
#define SCB_NSACR_CP10_11_VAL             (3U)
#endif
#define SCB_SHCSR_SECUREFAULTENA_VAL      (1)

#ifndef FPU_FPCCR_TS_VAL
#define FPU_FPCCR_TS_VAL                  (0U)
#endif
#define FPU_FPCCR_CLRONRETS_VAL           (0)

#ifndef FPU_FPCCR_CLRONRET_VAL
#define FPU_FPCCR_CLRONRET_VAL            (1)
#endif
#endif /* BSP_MCU_FAMILY_CFG_H_ */
