/*
* File Name: i2c.c
* File Description: This file contains the I2C related functions
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#include <stdio.h>
#include "em_chip.h"
#include "em_device.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "sl_i2cspm.h"
#include "src/timers.h"
#include "src/gpio.h"

#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

#define SI7021_DEVICE_ADDR 0x40 /*Slave address for Si7021 sensor*/
#define measure_temp_cmd 0xF3 /* Command for measuring temperature */
#define MS_DELAY_80 80000
#define MS_DELAY_10_8 10800

I2C_TransferReturn_TypeDef transferStatus;
I2C_TransferSeq_TypeDef transferSequence;
uint8_t cmd_data; /* Send commands to the Si7021 sensor*/
uint8_t buffer[2]; /* Read buffer used to store temperature values*/

/*
 * Function Name: i2c_Init
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This function initializes the I2C peripheral.
 *
 */
static void i2c_Init(void)
{
  /* Initialize the I2C hardware using I2CSPM_Init_TypeDef*/
  I2CSPM_Init_TypeDef I2C_Config = {
    .port = I2C0,
    .sclPort = gpioPortC,
    .sclPin = 10,
    .sdaPort = gpioPortC,
    .sdaPin = 11,
    .portLocationScl = 14,
    .portLocationSda = 16,
    .i2cRefFreq = 0,
    .i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
    .i2cClhr = i2cClockHLRStandard
  };

  I2CSPM_Init(&I2C_Config);
}

/*
 * Function Name: I2C_Write
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This function writes a value (command/data) to the I2C slave device based on
 * transferSequence structure values.
 *
 */
static void I2C_Write(void)
{
  cmd_data = measure_temp_cmd;
  transferSequence.addr = SI7021_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

/*
 * Function Name: I2C_Read
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This function reads a value from the I2C slave device based on
 * transferSequence structure values and stores it in buffer.
 *
 */
static void I2C_Read(void)
{
  transferSequence.addr = SI7021_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_READ;
  transferSequence.buf[0].data = buffer;
  transferSequence.buf[0].len = sizeof(buffer);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
       LOG_ERROR ("I2CSPM_Transfer: I2C bus read with error code: %d\n\r", transferStatus);
  }
}

/*
 * Function Name: read_temperature_Si7021
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This function is used to read and measure the temperature in degrees
 * Celsius from the Si7021 sensor. The temperature values are logged onto UART.
 *
 */
void read_temperature_Si7021(void)
{
  /* The below sequence is written based on the datasheet sequence and considerations for Si7021*/
  Si7021Enable();
  timerWaitUs(MS_DELAY_80);
  i2c_Init();
  I2C_Write();
  timerWaitUs(MS_DELAY_10_8);
  I2C_Read();
  Si7021Disable();
  /* Temperature formula taken from Si7021-A20 datasheet */
  uint32_t temp = ((175*((buffer[0]<<8)+buffer[1]))/65536)-47;
  LOG_INFO("Temperature is %u degrees Celcius.\n\r", temp);
}
