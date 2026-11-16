#include "bsp_uart.h"

void uart_send(uint8_t *buf, uint16_t len) {

  for (uint16_t i = 0; i < len; i++) {
    DL_UART_transmitDataBlocking(UART0_INST, buf[i]);
  }
}

void uart_recv(uint8_t *buf, uint16_t len) {

  for (uint16_t i = 0; i < len; i++) {
    buf[i] = DL_UART_receiveDataBlocking(UART0_INST);
  }
}