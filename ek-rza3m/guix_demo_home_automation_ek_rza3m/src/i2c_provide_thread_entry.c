#include "i2c_provide_thread.h"
#include "../device/i2c/inc/i2c_ctrl.h"

/* I2C Provide Thread entry function */
void i2c_provide_thread_entry(void) {
    // set I2C instance
    i2c_set_ch0_func((i2c_master_instance_t *)&g_i2c_master0);

    i2c_set_ch1_func((i2c_master_instance_t *)&g_i2c_master1);

    /* TODO: add your own code here */
    while (1)
    {
        tx_thread_sleep (1000);
    }
}
