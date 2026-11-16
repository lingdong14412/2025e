#ifndef BSP_I2C_H_
#define BSP_I2C_H_
<<<<<<< HEAD

#include "ti_msp_dl_config.h"
#include <stddef.h>

bool i2c_read_reg(I2C_Regs *i2c, uint8_t *buf, uint32_t addr, uint8_t reg);
bool i2c_read_multi_byte(I2C_Regs *i2c, uint32_t addr, uint8_t reg,
                         uint16_t len, uint8_t *buf);
bool i2c_write(I2C_Regs *i2c, uint32_t addr, uint8_t *buf, uint16_t len);
bool i2c_wait(I2C_Regs *i2c);
=======
#include "ti_msp_dl_config.h"
#include "interrupts.h"

static uint8_t dat;
void i2c_init();
void i2c_read_reg(uint32_t addr,uint8_t reg, uint8_t *buf);
>>>>>>> 75b89cf (修改bsp_i2c.c/.h)
#endif
