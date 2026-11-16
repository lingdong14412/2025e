#ifndef __AS5600_H_
#define __AS5600_H_

#define ZMOS 0x00
#define ZPOS 0x01
#define MPOS_H 0x03
#define MPOS_L 0x04
#define MANG_H 0x05
#define MANG_L 0x06
#define CONF_H 0x07
#define CONF_L 0x08
#define RAW_ANGLE_H 0x0C
#define RAW_ANGLE_L 0x0D
#define ANGLE_H 0x0E
#define ANGLE_L 0x0F
#define AS5600_STATUS 0x0B
#define AGC 0x1A
#define MAGNITUDE_H 0x1B
#define MAGNITUDE_L 0x1C
#define BURN 0xFF

#define AS5600_ADDR 0x36


#include "bsp/bsp_i2c.h"
#include <stdint.h>



void as5600_writereg(uint8_t reg,uint8_t *buf,uint16_t len);
void as5600_readreg(uint8_t reg,uint8_t *buf,uint16_t len);


#endif