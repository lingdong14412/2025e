#ifndef BSP_I2C_H_
#define BSP_I2C_H_
#include "ti_msp_dl_config.h"

bool i2c_read(uint8_t devAddr, uint8_t reg, uint8_t *data, uint8_t len);
bool i2c_write(uint8_t devAddr, uint8_t reg, uint8_t *data, uint8_t len);

#endif
