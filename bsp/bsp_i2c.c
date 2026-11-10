#include "bsp_i2c.h"

bool i2c_wait_idle(I2C_Regs *i2c) {
  uint32_t timeout = 100000;
  // if(status)
  while (timeout--) {
    uint8_t status = DL_I2C_getControllerStatus(i2c);
    if (status & DL_I2C_CONTROLLER_STATUS_ERROR) {
      DL_I2C_flushControllerTXFIFO(i2c);
      DL_I2C_resetControllerTransfer(i2c);
      return false;
    }
    if (status & DL_I2C_CONTROLLER_STATUS_IDLE) {
      return true;
    }
  }

  return true;
}

bool i2c_read_reg(I2C_Regs *i2c, uint8_t *data, uint32_t addr, uint8_t reg) {
  uint32_t to = 10000;
  DL_I2C_resetControllerTransfer(i2c);
  DL_I2C_fillControllerTXFIFO(i2c, &reg, 1);
  DL_I2C_startControllerTransferAdvanced(
      i2c, addr, DL_I2C_CONTROLLER_DIRECTION_TX, 1,
      DL_I2C_CONTROLLER_START_ENABLE, DL_I2C_CONTROLLER_STOP_DISABLE,
      DL_I2C_CONTROLLER_ACK_DISABLE);
  // if (!i2c_wait_idle()) {
  //   return false;
  // }

  while ((DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_BUSY) &&
         to--)
    ;

  // while (!(DL_I2C_getControllerStatus(i2c) &
  //      DL_I2C_CONTROLLER_STATUS_IDLE) && to--)
  // ;

  DL_I2C_startControllerTransferAdvanced(
      i2c, addr, DL_I2C_CONTROLLER_DIRECTION_RX, 1,
      DL_I2C_CONTROLLER_START_ENABLE, DL_I2C_CONTROLLER_STOP_ENABLE,
      DL_I2C_CONTROLLER_ACK_DISABLE);

  // if (!i2c_wait_idle()) {
  //   return false;
  // }

  // while (!(DL_I2C_getControllerStatus(i2c) &
  //        DL_I2C_CONTROLLER_STATUS_IDLE) && to--)
  //   ;

  // while (
  //     (DL_I2C_getControllerStatus(i2c) &
  //      (DL_I2C_CONTROLLER_STATUS_BUSY | DL_I2C_CONTROLLER_STATUS_BUSY_BUS))
  //      &&
  //     to--)
  //   ;

  while (DL_I2C_getControllerStatus(i2c) &
         (DL_I2C_CONTROLLER_STATUS_BUSY | DL_I2C_CONTROLLER_STATUS_BUSY_BUS))
    ;
  *data = DL_I2C_receiveControllerData(i2c);
  return true;
}

bool i2c_read_multi_byte(I2C_Regs *i2c, uint32_t addr, uint8_t reg,
                         uint16_t len, uint8_t *buf) {
  for (uint16_t i = 0; i < len; i++) {
    if (!i2c_read_reg(i2c, &buf[i], addr, reg)) {
      return false;
    }
  }
  return true;
}

bool i2c_write(I2C_Regs *i2c, uint32_t addr, uint8_t *buf, uint16_t len) {

  DL_I2C_flushControllerTXFIFO(i2c);
  DL_I2C_resetControllerTransfer(i2c);
  DL_I2C_fillControllerTXFIFO(i2c, buf, len);
  if (!i2c_wait_idle(i2c)) {
    return false;
  }
  DL_I2C_startControllerTransfer(i2c, addr, DL_I2C_CONTROLLER_DIRECTION_TX,
                                 len);

  if (!i2c_wait_idle(i2c)) {
    return false;
  }

  return true;
}