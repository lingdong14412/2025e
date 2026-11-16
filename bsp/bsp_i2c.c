#include "bsp_i2c.h"


/* Maximum size of TX packet */
#define I2C_TX_MAX_PACKET_SIZE (1)

/* Number of bytes to send to target device */
#define I2C_TX_PACKET_SIZE (1)

/* Maximum size of RX packet */
#define I2C_RX_MAX_PACKET_SIZE (1)

/* Number of bytes to received from target */
#define I2C_RX_PACKET_SIZE (1)

/* I2C Target address */
// #define I2C_TARGET_ADDRESS (0x68)

/* Data sent to the Target */
// static uint32_t *dev_addr;
// static uint8_t *gTxPacket;

/* Counters for TX length and bytes sent */
uint32_t gTxLen, gTxCount;

/* Data received from Target */
static uint8_t *gRxPacket;
/* Counters for TX length and bytes sent */
uint32_t gRxLen, gRxCount;
/* Indicates status of I2C */
enum I2cControllerStatus {
  I2C_STATUS_IDLE = 0,
  I2C_STATUS_TX_STARTED,
  I2C_STATUS_TX_INPROGRESS,
  I2C_STATUS_TX_COMPLETE,
  I2C_STATUS_RX_STARTED,
  I2C_STATUS_RX_INPROGRESS,
  I2C_STATUS_RX_COMPLETE,
  I2C_STATUS_ERROR,
} gI2cControllerStatus;


void i2c_init(){
  NVIC_EnableIRQ(I2C_INST_INT_IRQN);
  DL_SYSCTL_disableSleepOnExit();
}




void i2c_read_reg(uint32_t addr,uint8_t reg, uint8_t *buf){
  dev_addr=&addr;
  gTxPacket=&reg;
  gRxPacket=buf;
  gI2cControllerStatus = I2C_STATUS_IDLE;

  gTxLen = I2C_TX_PACKET_SIZE;
  /*
   * Fill the FIFO
   *  The FIFO is 8-bytes deep, and this function will return number
   *  of bytes written to FIFO */
  gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &*gTxPacket, gTxLen);
  /* Enable TXFIFO trigger interrupt if there are more bytes to send */
  if (gTxCount < gTxLen) {
    DL_I2C_enableInterrupt(I2C_INST,
                           DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
  } else {
    DL_I2C_disableInterrupt(I2C_INST,
                            DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
  }

  /*
   * Send the packet to the controller.
   * This function will send Start + Stop automatically.
   */
  gI2cControllerStatus = I2C_STATUS_TX_STARTED;
  while (
      !(DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
    ;
  DL_I2C_startControllerTransferAdvanced(
      I2C_INST, addr, DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen,
      DL_I2C_CONTROLLER_START_ENABLE, DL_I2C_CONTROLLER_STOP_DISABLE,
      DL_I2C_CONTROLLER_ACK_DISABLE);

  /* Wait until the Controller sends all bytes */
  while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
         (gI2cControllerStatus != I2C_STATUS_ERROR)) {
    __WFE();
  }

  /* Trap if there was an error */
  if (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_ERROR) {
    /* LED will remain high if there is an error */
    __BKPT(0);
  }

  /* Add delay between transfers */
  delay_cycles(1000);

  /* Send a read request to Target */
  gRxLen = I2C_RX_PACKET_SIZE;
  gRxCount = 0;
  gI2cControllerStatus = I2C_STATUS_RX_STARTED;
  DL_I2C_startControllerTransferAdvanced(
      I2C_INST, addr, DL_I2C_CONTROLLER_DIRECTION_RX, gRxLen,
      DL_I2C_CONTROLLER_START_ENABLE, DL_I2C_CONTROLLER_STOP_ENABLE,
      DL_I2C_CONTROLLER_ACK_DISABLE);

  /* Wait for all bytes to be received in interrupt */
  while (gI2cControllerStatus != I2C_STATUS_RX_COMPLETE) {
    __WFE();
  }

  while (DL_I2C_getControllerStatus(I2C_INST) &
         DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
    ;

  /* If write and read were successful, toggle LED */
  while (1) {
    DL_GPIO_togglePins(GPIO0_PORT, GPIO0_LED_PIN);
    delay_cycles(16000000);
  }
}





void I2C_INST_IRQHandler(void) {
  switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
  case DL_I2C_IIDX_CONTROLLER_RX_DONE:
    gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
    break;
  case DL_I2C_IIDX_CONTROLLER_TX_DONE:
    DL_I2C_disableInterrupt(I2C_INST,
                            DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
    break;
  case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
    gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
    /* Receive all bytes from target */
    while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST) != true) {
      if (gRxCount <= gRxLen) {
        *gRxPacket = DL_I2C_receiveControllerData(I2C_INST);
      } else {
        /* Ignore and remove from FIFO if the buffer is full */
        DL_I2C_receiveControllerData(I2C_INST);
      }
    }
    break;
  case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
    gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
    /* Fill TX FIFO with next bytes to send */
    if (gTxCount <= gTxLen) {
      gTxCount +=
          DL_I2C_fillControllerTXFIFO(I2C_INST, &*gTxPacket, gTxLen - gTxCount);
    }
    break;
    /* Not used for this example */
  case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
  case DL_I2C_IIDX_CONTROLLER_NACK:
    if ((gI2cControllerStatus == I2C_STATUS_RX_STARTED) ||
        (gI2cControllerStatus == I2C_STATUS_TX_STARTED)) {
      /* NACK interrupt if I2C Target is disconnected */
      gI2cControllerStatus = I2C_STATUS_ERROR;
    }
  case DL_I2C_IIDX_CONTROLLER_RXFIFO_FULL:
  case DL_I2C_IIDX_CONTROLLER_TXFIFO_EMPTY:
  case DL_I2C_IIDX_CONTROLLER_START:
  case DL_I2C_IIDX_CONTROLLER_STOP:
  case DL_I2C_IIDX_CONTROLLER_EVENT1_DMA_DONE:
  case DL_I2C_IIDX_CONTROLLER_EVENT2_DMA_DONE:
  default:
    break;
  }
}
