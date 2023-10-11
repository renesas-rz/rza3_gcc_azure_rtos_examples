/* generated pin source file - do not edit */
#include "bsp_api.h"
#include "r_ioport_api.h"
#include "r_ioport.h"

extern const ioport_extend_cfg_t g_ioport_cfg_extend;

const ioport_pin_cfg_t g_bsp_pin_cfg_data[] =
        {
        { .pin = BSP_IO_PORT_03_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                | (uint32_t) IOPORT_PERIPHERAL_MODE2) },
          { .pin = BSP_IO_PORT_03_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE2) },
          { .pin = BSP_IO_PORT_03_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE2) },
          { .pin = BSP_IO_PORT_03_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE2) },
          { .pin = BSP_IO_PORT_05_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_05_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_06_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_06_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_06_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_06_PIN_04, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_07_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_07_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_07_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_07_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_07_PIN_04, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_08_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_08_PIN_04, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_09_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_09_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_09_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_09_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_10_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_10_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_10_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_10_PIN_04, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
                  | (uint32_t) IOPORT_PERIPHERAL_MODE1) },
          { .pin = BSP_IO_PORT_11_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_11_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_11_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_11_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_12_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_12_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_13_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_13_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_13_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_13_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_13_PIN_04, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_14_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_14_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_14_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_15_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_15_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_15_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_15_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_16_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_16_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_17_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_17_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_17_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_17_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_18_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_18_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_18_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_PORT_18_PIN_03, .pin_cfg = ((uint32_t) IOPORT_CFG_DRIVE_B01
                  | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST
                  | (uint32_t) IOPORT_PERIPHERAL_MODE6) },
          { .pin = BSP_IO_AUDIO_CLK1, .pin_cfg = ((uint32_t) IOPORT_CFG_SPECIAL_PURPOSE_PORT_INPUT_ENABLE) },
          { .pin = BSP_IO_AUDIO_CLK2, .pin_cfg = ((uint32_t) IOPORT_CFG_SPECIAL_PURPOSE_PORT_INPUT_ENABLE) },
          { .pin = BSP_IO_RIIC0_SCL, .pin_cfg = ((uint32_t) IOPORT_CFG_SPECIAL_PURPOSE_PORT_INPUT_ENABLE) },
          { .pin = BSP_IO_RIIC0_SDA, .pin_cfg = ((uint32_t) IOPORT_CFG_SPECIAL_PURPOSE_PORT_INPUT_ENABLE) },
          { .pin = BSP_IO_RIIC1_SCL, .pin_cfg = ((uint32_t) IOPORT_CFG_SPECIAL_PURPOSE_PORT_INPUT_ENABLE) },
          { .pin = BSP_IO_RIIC1_SDA, .pin_cfg = ((uint32_t) IOPORT_CFG_SPECIAL_PURPOSE_PORT_INPUT_ENABLE) }, };

const ioport_cfg_t g_bsp_pin_cfg =
{ .number_of_pins = sizeof(g_bsp_pin_cfg_data) / sizeof(ioport_pin_cfg_t),
  .p_pin_cfg_data = &g_bsp_pin_cfg_data[0],
  .p_extend = &g_ioport_cfg_extend };
