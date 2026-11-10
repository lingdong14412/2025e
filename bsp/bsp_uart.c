#include "bsp_uart.h"

void uart_send(uint8_t *buf, uint16_t len) {
  // DL_UART_fillTXFIFO(uart_INST, buf, len);
  // while(DL_UART_isBusy(uart_INST));

  for (uint16_t i = 0; i < len; i++) {
    DL_UART_transmitDataBlocking(uart_INST, buf[i]);
  }
}

void uart_recv(uint8_t *buf,uint16_t len) {
  // DL_UART_fillTXFIFO(uart_INST, buf, len);
  // while(DL_UART_isBusy(uart_INST));
//   uint16_t len = sizeof(buf) / sizeof(uint8_t);
  for (uint16_t i = 0; i < len; i++) {
    buf[i] = DL_UART_receiveDataBlocking(uart_INST);
  }
}