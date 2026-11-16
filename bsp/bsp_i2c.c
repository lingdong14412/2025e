#include "bsp_i2c.h"


static bool i2c_wait_idle(void)
{
    uint32_t timeout = 100000;

    while (timeout--) {
        uint32_t status = DL_I2C_getControllerStatus(I2C_INST);

        if (status & DL_I2C_CONTROLLER_STATUS_ERROR) {
            DL_I2C_flushControllerTXFIFO(I2C_INST);
            DL_I2C_resetControllerTransfer(I2C_INST);
            return false;
        }

        if (status & DL_I2C_CONTROLLER_STATUS_IDLE) {
            return true;
        }
    }
    return false;
}


bool i2c_read(uint8_t devAddr, uint8_t reg, uint8_t *data, uint8_t len)
{
    if (len > 8) return false;


    DL_I2C_resetControllerTransfer(I2C_INST);
    DL_I2C_fillControllerTXFIFO(I2C_INST, &reg, 1);       /* 先填 FIFO  */
    DL_I2C_startControllerTransfer(I2C_INST, devAddr,       /* 再启动    */
        DL_I2C_CONTROLLER_DIRECTION_TX, 1);

    if (!i2c_wait_idle()) {
        return false;
    }


    DL_I2C_startControllerTransfer(I2C_INST, devAddr,
        DL_I2C_CONTROLLER_DIRECTION_RX, len);

    if (!i2c_wait_idle()) {
        return false;
    }

    for (uint8_t i = 0; i < len; i++) {
        data[i] = DL_I2C_receiveControllerData(I2C_INST);
    }

    return true;
}



bool i2c_write(uint8_t devAddr, uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t txBuf[8];
    uint8_t totalLen;

    if (len > 7) return false;
    totalLen = 1 + len;

    txBuf[0] = reg;
    for (uint8_t i = 0; i < len; i++) {
        txBuf[1 + i] = data[i];
    }

    DL_I2C_resetControllerTransfer(I2C_INST);

    /* 先填 FIFO，再启动传输（顺序修复） */
    DL_I2C_fillControllerTXFIFO(I2C_INST, txBuf, totalLen);
    DL_I2C_startControllerTransfer(I2C_INST, devAddr,
        DL_I2C_CONTROLLER_DIRECTION_TX, totalLen);

    return i2c_wait_idle();
}









// void i2c_read_reg(uint32_t addr, uint8_t reg, uint8_t *buf) {
//   // dev_addr=&addr;
//   gTxPacket = &reg;
//   gRxPacket = buf;
//   gI2cControllerStatus = I2C_STATUS_IDLE;

//   gTxLen = I2C_TX_PACKET_SIZE;
//   /*
//    * Fill the FIFO
//    *  The FIFO is 8-bytes deep, and this function will return number
//    *  of bytes written to FIFO */
//   gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &reg, gTxLen);
//   /* Enable TXFIFO trigger interrupt if there are more bytes to send */
//   if (gTxCount < gTxLen) {
//     DL_I2C_enableInterrupt(I2C_INST,
//                            DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
//   } else {
//     DL_I2C_disableInterrupt(I2C_INST,
//                             DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
//   }

//   /*
//    * Send the packet to the controller.
//    * This function will send Start + Stop automatically.
//    */
//   gI2cControllerStatus = I2C_STATUS_TX_STARTED;
//   while (
//       !(DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
//     ;
//   DL_I2C_startControllerTransferAdvanced(
//       I2C_INST, addr, DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen,
//       DL_I2C_CONTROLLER_START_ENABLE, DL_I2C_CONTROLLER_STOP_DISABLE,
//       DL_I2C_CONTROLLER_ACK_DISABLE);

//   /* Wait until the Controller sends all bytes */
//   while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
//          (gI2cControllerStatus != I2C_STATUS_ERROR)) {
//     __WFE();
//   }

//   /* Trap if there was an error */
//   if (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_ERROR) {
//     /* LED will remain high if there is an error */
//     __BKPT(0);
//   }

//   /* Add delay between transfers */
//   delay_cycles(1000);

//   /* Send a read request to Target */
//   gRxLen = I2C_RX_PACKET_SIZE;
//   gRxCount = 0;
//   gI2cControllerStatus = I2C_STATUS_RX_STARTED;
//   DL_I2C_startControllerTransferAdvanced(
//       I2C_INST, addr, DL_I2C_CONTROLLER_DIRECTION_RX, gRxLen,
//       DL_I2C_CONTROLLER_START_ENABLE, DL_I2C_CONTROLLER_STOP_ENABLE,
//       DL_I2C_CONTROLLER_ACK_DISABLE);

//   /* Wait for all bytes to be received in interrupt */
//   while (gI2cControllerStatus != I2C_STATUS_RX_COMPLETE) {
//     __WFE();
//   }

//   while (DL_I2C_getControllerStatus(I2C_INST) &
//          DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
//     ;

//   /* If write and read were successful, toggle LED */
//   while (1) {
//     DL_GPIO_togglePins(GPIO0_PORT, GPIO0_LED_PIN);
//     delay_cycles(16000000);
//   }
// }
