#include "bsp_uart.h"

void UART0_send(uint8_t *buf, uint16_t len) {
  // DL_UART0_fillTXFIFO(UART0_INST, buf, len);
  // while(DL_UART0_isBusy(UART0_INST));

  for (uint16_t i = 0; i < len; i++) {
    DL_UART_transmitDataBlocking(UART0_INST, buf[i]);
  }
}

void UART0_recv(uint8_t *buf,uint16_t len) {
  // DL_UART0_fillTXFIFO(UART0_INST, buf, len);
  // while(DL_UART0_isBusy(UART0_INST));
//   uint16_t len = sizeof(buf) / sizeof(uint8_t);
  for (uint16_t i = 0; i < len; i++) {
    buf[i] = DL_UART_receiveDataBlocking(UART0_INST);
  }
}