#include <stdint.h>
#include "r_usb_typedef.h"

#include "new_thread0.h"

#include "ux_api.h"
#include "ux_device_class_cdc_acm.h"

#include "command_list.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* USBx device configuration settings */
#define DEVICE_FRAME_LENGTH_HIGH_SPEED              (103U)
#define DEVICE_FRAME_LENGTH_FULL_SPEED              (93U)
#define STRING_FRAMEWORK_LENGTH                     (105U)
#define LANGUAGE_ID_FRAME_WORK_LENGTH               (2U)
#define CONFIG_NUMB                                 (1U)
#define INTERFACE_NUMB0                             (0x00)

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/
extern uint8_t g_device_framework_full_speed[];
extern uint8_t g_device_framework_hi_speed[];
extern uint8_t g_language_id_framework[];
extern uint8_t g_string_framework[];

/******************************************************************************
 Global functions and variables
 ******************************************************************************/
UINT usbx_status_callback (ULONG status);

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void ux_cdc_device0_instance_activate(void * cdc_instance);
static void ux_cdc_device0_instance_deactivate(void * cdc_instance);

/******************************************************************************
Exported global functions (to be accessed by other files)
 ******************************************************************************/
extern const usb_api_t g_usb_on_usb;

/* Define local prototypes and definitions.  */

/* Define local constants.  */
#define UX_DEMO_BUFFER_SIZE     2048

#define MEMPOOL_SIZE    (64*1024)
#define BYTE_SIZE        sizeof(uint32_t)

#define RESET_VALUE             (0x00)


/* Define global variables.  */
UX_SLAVE_CLASS_CDC_ACM              *g_cdc = UX_NULL;
UCHAR                               buffer[UX_DEMO_BUFFER_SIZE];


/* Define local variables.  */
static uint32_t g_ux_pool_memory[MEMPOOL_SIZE / BYTE_SIZE];
static UX_SLAVE_CLASS_CDC_ACM_PARAMETER g_ux_device_class_cdc_acm0_parameter;
static ULONG g_actual_length;

/* New Thread entry function */
void new_thread0_entry(void)
{
    UINT    status;
    ULONG   actual_length;
    ULONG   requested_length;

    tx_thread_sleep(10);

    /* ux_system_initialization */
    status = ux_system_initialize(g_ux_pool_memory, MEMPOOL_SIZE, UX_NULL, RESET_VALUE);
    if (UX_SUCCESS != status)
    {
        printf("Initialization failed.\r\n");
        return;
    }

    /* ux_device stack initialization */
    status =     ux_device_stack_initialize(g_device_framework_hi_speed,
                                            DEVICE_FRAME_LENGTH_HIGH_SPEED,
                                             g_device_framework_full_speed,
                                             DEVICE_FRAME_LENGTH_FULL_SPEED,
                                             g_string_framework,
                                             STRING_FRAMEWORK_LENGTH,
                                             g_language_id_framework,
                                             LANGUAGE_ID_FRAME_WORK_LENGTH,
                                             &usbx_status_callback);
    /* Error Handle */
    if (UX_SUCCESS != status)
    {
        printf("Initialization failed.\r\n");
        return;
    }

    /* The activate command is used when the host has sent a SET_CONFIGURATION command
     and this interface has to be mounted. Both Bulk endpoints have to be mounted
     and the cdc_acm thread needs to be activated.  */
    g_ux_device_class_cdc_acm0_parameter.ux_slave_class_cdc_acm_instance_activate = ux_cdc_device0_instance_activate;

    /* The deactivate command is used when the device has been extracted.
     The device endpoints have to be dismounted and the cdc_acm thread canceled.  */
    g_ux_device_class_cdc_acm0_parameter.ux_slave_class_cdc_acm_instance_deactivate = ux_cdc_device0_instance_deactivate;

    /* ux_device stack class registration */
    status = ux_device_stack_class_register (_ux_system_slave_class_cdc_acm_name, _ux_device_class_cdc_acm_entry, CONFIG_NUMB,
                                             INTERFACE_NUMB0, (void*) &g_ux_device_class_cdc_acm0_parameter);
    /* Error Handle */
    if (UX_SUCCESS != status)
    {
        printf("Initialization failed.\r\n");
        return;
    }

    /* Open usb driver */
    if (g_usb_on_usb.open(&g_basic0_ctrl, &g_basic0_cfg) != FSP_SUCCESS)
    {
        printf("Failed to open USB.\r\n");
        return;
    }

    printf("initialization process succeeded.\r\n");

    while(1)
    {
        /* Ensure the CDC class is mounted.  */
        if(g_cdc != UX_NULL)
        {
            /* Read from the CDC class.  */
            status = ux_device_class_cdc_acm_read(g_cdc, buffer, UX_DEMO_BUFFER_SIZE, &actual_length);

            if(status==UX_SUCCESS)
            {
                /* The actual length becomes the requested length.  */
                requested_length = actual_length;

                /* Check the status.  If OK, we will write to the CDC instance.  */
                status = ux_device_class_cdc_acm_write(g_cdc, buffer, requested_length, &g_actual_length);

                /* Clear buffer to 0. */
                ux_utility_memory_set(buffer, 0, sizeof(buffer));
            }
            else
            {
                /* If reading from host fails, wait 50tick. */
                tx_thread_sleep(50);
            }
        }
        else
        {
            /* If the CDC class fails to mount, 50tick wait. */
            tx_thread_sleep(50);
        }
    }
}

/* Activate Function */
static void ux_cdc_device0_instance_activate (void * cdc_instance)
{
    /* Save the CDC instance.  */
    g_cdc = (UX_SLAVE_CLASS_CDC_ACM *) cdc_instance;
}

/* Deactivate Function */
static void ux_cdc_device0_instance_deactivate (void * cdc_instance)
{
    FSP_PARAMETER_NOT_USED(cdc_instance);
    g_cdc = UX_NULL;
}

/* Callback Function */
UINT usbx_status_callback (ULONG status)
{
    switch (status)
    {
        case UX_DEVICE_ATTACHED:
        break;

        case UX_DEVICE_REMOVED:
        break;

        default:
        {
            /* do nothing */
        }
        break;
    }
    return 0;
}

