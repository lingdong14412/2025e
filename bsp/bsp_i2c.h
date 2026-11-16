#ifndef BSP_I2C_H_
#define BSP_I2C_H_
#include "ti_msp_dl_config.h"
#include "interrupts.h"

static uint8_t dat;
void i2c_init();
void i2c_read_reg(uint32_t addr,uint8_t reg, uint8_t *buf);
#endif
