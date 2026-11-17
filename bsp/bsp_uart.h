#ifndef BSP_UART_H_
#define BSP_UART_H_
#include "ti_msp_dl_config.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>




// void uart_send(uint8_t *buf, uint16_t len);

void uart_recv(uint8_t *buf,uint16_t len);

void uart_printf(const char *format, ...);

#endif