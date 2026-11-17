#include "bsp_uart.h"

// void uart_send(uint8_t *buf, uint16_t len) {

//   for (uint16_t i = 0; i < len; i++) {
//     DL_UART_transmitDataBlocking(UART0_INST, buf[i]);
//   }
// }

void uart_recv(uint8_t *buf, uint16_t len) {

  for (uint16_t i = 0; i < len; i++) {
    buf[i] = DL_UART_receiveDataBlocking(UART0_INST);
  }
}

void uart_printf(const char *format, ...) {
  uint32_t length;
  va_list args;
  uint32_t i;
  char TxBuffer[128] = {0};

  va_start(args, format);
  length = vsnprintf((char *)TxBuffer, sizeof(TxBuffer), (char *)format, args);
  va_end(args);

  for (i = 0; i < length; i++) {
    DL_UART_Main_transmitDataBlocking(UART0_INST, TxBuffer[i]);
  }
}