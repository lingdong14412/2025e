/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "Sensors/mpu6500/mpu.h"
#include "bsp/bsp_i2c.h"
#include "bsp/bsp_uart.h"
#include "ti_msp_dl_config.h"
#include <stdio.h>

uint8_t ch;
uint16_t accel_x;
uint8_t accel_xh, accel_xl;
uint8_t buf[4] = "ab\r\n";
int main(void) {
  SYSCFG_DL_init();

  /* Set LED to indicate start of transfer */
  DL_GPIO_togglePins(GPIO0_PORT, GPIO0_LED_PIN);
  i2c_read(0x68, 0x75, &ch, 1);
  while (1) {
    i2c_read(0x68, ACCEL_XOUT_H, &accel_xh, 1);
    i2c_read(0x68, ACCEL_XOUT_L, &accel_xl, 1);
    accel_x = (uint16_t)(accel_xh << 8 | accel_xl);


    uart_printf("%d\r\n",accel_xh);
    uart_printf("%d\r\n",accel_x);
    uart_printf("%d\r\n",accel_x);

  uart_printf("\r\n");
    delay_cycles(3200000);
  }
}
