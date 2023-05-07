/*
 * bme680.c
 *
 *  Created on: 22-Apr-2023
 *      Author: gauta
 */
/*
* File Name: bme680.c
* File Description: This file contains the I2C driver functions for BME680.
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

#define BME_680_DEVICE_ADDR 0x76 /* Slave address for BME 280 */

#define MS_DELAY_80 80000
#define MS_DELAY_10_8 10800

I2C_TransferReturn_TypeDef transferStatus;
I2C_TransferSeq_TypeDef transferSequence;
uint8_t cmd_data; /* Send commands to the Si7021 sensor*/
uint8_t buffer[2]; /* Read buffer used to store temperature values*/

uint8_t new_buffer[3]; /* Read buffer used to store temperature values*/

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
void i2c_Init(void)
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
//    .i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
    .i2cMaxFreq = I2C_FREQ_FAST_MAX,
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
static void I2C_Write_Reset(void)
{
  cmd_data = 0xE0;
  uint8_t data = 0xB6;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &data;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

/* Function to set BME680 in forced mode */
static void I2C_Set_Mode_Forced(void)
{
  cmd_data = 0x74; /* Control Meas register*/
  uint8_t temp_buffer[2]; // Temporary buffer to store ctrl meas contents
//  uint8_t data = (0x02 << 5) | (0x05 << 2) | 0x01; // Temperature oversampling, pressure oversampling, forced mode
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = temp_buffer;
  transferSequence.buf[1].len = sizeof(temp_buffer);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }

  LOG_INFO("BME680 CTRL_MEAS register data is 0x%02X\n\r", temp_buffer[0]);

  uint8_t data = temp_buffer[0] | 0x01; // Temperature oversampling, pressure oversampling, forced mode
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = data;
  transferSequence.buf[1].len = sizeof(data);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }

}

/* Function to set oversampling for humidity*/
static void I2C_Set_Oversampling_Hum(void)
{
  cmd_data = 0x72; /* Control Hum register*/
  uint8_t data = 0x01; // Temperature oversampling, pressure oversampling, forced mode
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &data;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

/* Function to set oversampling for temperature and pressure*/
static void I2C_Set_Oversampling_Temp_Pres(void)
{
  cmd_data = 0x74; /* Control Meas register*/
  uint8_t data = (0x02 << 5) | (0x05 << 2); // Temperature oversampling, pressure oversampling, forced mode
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &data;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

/* Function to set config for BME280*/
static void I2C_Set_IIR_Filter(void)
{
  cmd_data = 0x75; /* Config register */
  uint8_t data = 0x01 << 2; /* Filter coefficient*/
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &data;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

/* Function to set ctrl gas 1 for BME680*/
static void I2C_Set_Ctrl_Gas_1(void)
{
  cmd_data = 0x71; /* ctrl gas 1 register */
  uint8_t data = (0x01 << 4) | (0x01); /* run_gas and heater set-point*/
//  0b11111000
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &data;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

/* Function to set ctrl gas 0 for BME680*/
static void I2C_Set_Ctrl_Gas_0(void)
{
  cmd_data = 0x70; /* ctrl gas 1 register */
  uint8_t data = (0x01 << 3); /* run_gas and heater set-point*/
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &data;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

static void set_res_heat_0(void)
{

}

/* Function to set gas wait 0 for BME680*/
static void I2C_Set_Gas_Wait_0(void)
{
  cmd_data = 0x64; /* gas wait 0 register */
  uint8_t data = 0x41; // 4 ms wait time
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &data;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

/* Function to set res heat 0 for BME680*/
static void I2C_Set_Res_Heat_0(void)
{
  cmd_data = 0x64; /* res heat 0 register */
  uint8_t data = (0x01 << 4) | (0x01); /* run_gas and heater set-point*/
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &data;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

// Calibration parameters
uint8_t par_t1_data[2], par_t2_t3_data[3];
// uint32_t par_t1_32bit, par_t2_32bit, par_t3_32bit;
uint16_t par_t1;
int16_t par_t2;
int8_t par_t3;

// Gas calibration parameters
uint8_t par_g2_g1_g3_data[4];
int8_t par_gh1, par_gh3;
int16_t par_gh2;

void get_calibration_parameters(void)
{
//  Temperature Calibration Parameters
  uint8_t par_t1_addr = 0xE9;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &par_t1_addr;
  transferSequence.buf[0].len = sizeof(par_t1_addr);
  transferSequence.buf[1].data = par_t1_data;
  transferSequence.buf[1].len = sizeof(par_t1_data);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }

  par_t1 = (uint16_t)(par_t1_data[0] | (par_t1_data[1] << 8));

  uint8_t par_t2_addr = 0x8A;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &par_t2_addr;
  transferSequence.buf[0].len = sizeof(par_t2_addr);
  transferSequence.buf[1].data = par_t2_t3_data;
  transferSequence.buf[1].len = sizeof(par_t2_t3_data);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }

  par_t3 = (int8_t)par_t2_t3_data[2];

  par_t2 = (int16_t)(par_t2_t3_data[0] | (par_t2_t3_data[1] << 8));

  // Gas Calibration Parameters
  uint8_t par_g2_addr = 0xEB;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &par_g2_addr;
  transferSequence.buf[0].len = sizeof(par_g2_addr);
  transferSequence.buf[1].data = par_g2_g1_g3_data;
  transferSequence.buf[1].len = sizeof(par_g2_g1_g3_data);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }

  par_gh2 = (int16_t)(par_g2_g1_g3_data[0] | (par_g2_g1_g3_data[1] << 8));
  par_gh1 = (int8_t)par_g2_g1_g3_data[2];
  par_gh3 = (int8_t)par_g2_g1_g3_data[3];

  uint8_t res_heat_val_addr = 0x00;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &res_heat_val_addr;
  transferSequence.buf[0].len = sizeof(res_heat_val_addr);
  transferSequence.buf[1].data = par_g2_g1_g3_data;
  transferSequence.buf[1].len = sizeof(par_g2_g1_g3_data);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

void BME680_init(void)
{
  i2c_Init();
  I2C_Write_Reset();
  timerWaitUs(MS_DELAY_80);

  I2C_Set_Oversampling_Temp_Pres();

  I2C_Set_Gas_Wait_0();

  I2C_Set_Ctrl_Gas_0();
  I2C_Set_Res_Heat_0();
  I2C_Set_Ctrl_Gas_1();

}

// Temperature compensated value
int16_t temp_comp;

static void Compute_Temperature(void)
{
  uint8_t temp_adc[3];
  uint8_t temp_adc_addr = 0x22;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &temp_adc_addr;
  transferSequence.buf[0].len = sizeof(temp_adc_addr);
  transferSequence.buf[1].data = temp_adc;
  transferSequence.buf[1].len = sizeof(temp_adc);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }

  uint32_t temp_32_bit = (temp_adc[1] << 4) | (temp_adc[0] << 12) | ((temp_adc[2] & 0xF0) >> 4);

  int64_t var1 = ((int32_t)temp_32_bit >> 3) - ((int32_t)par_t1 << 1);
  int64_t var2 = (var1 * (int32_t)par_t2) >> 11;
  int64_t var3 = ((((var1 >> 1)*(var1 >> 1)) >> 12)*(par_t3 << 4)) >> 14;
  int32_t t_fine = (int32_t)(var2 + var3);
  temp_comp = (int16_t)((t_fine*5) + 128) >> 8;

}

uint8_t gas_adc_range[2];
uint8_t range_switching_error;

uint32_t lookup_table_gas[16] = {
    4096000000, 2048000000, 1024000000,
    512000000, 255744255, 127110228, 64000000,
    32258064, 16016016, 8000000, 4000000,
    2000000, 1000000, 500000, 250000, 125000
};

const int array1_int = 2147483647;

void Compute_AIQ(void)
{
  uint8_t gas_adc_addr = 0x2A;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &gas_adc_addr;
  transferSequence.buf[0].len = sizeof(gas_adc_addr);
  transferSequence.buf[1].data = gas_adc_range;
  transferSequence.buf[1].len = sizeof(gas_adc_range);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }

  uint8_t range_switching_addr = 0x04;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &range_switching_addr;
  transferSequence.buf[0].len = sizeof(range_switching_addr);
  transferSequence.buf[1].data = &range_switching_error;
  transferSequence.buf[1].len = 1;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }



}

/*
 * Function Name: I2C_Read_ID
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
void I2C_Read_ID(void)
{
  cmd_data = 0xD0;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = new_buffer;
  transferSequence.buf[1].len = sizeof(new_buffer);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}


/*
 * Function Name: read_BME680
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
void read_BME680(void)
{
  /* The below sequence is written based on the datasheet sequence and considerations for Si7021*/
  Si7021Enable();
  /* The below sequence is written based on the datasheet sequence and considerations for BME280*/
  I2C_Write_Reset();
  timerWaitUs(MS_DELAY_80);
  i2c_Init();

  I2C_Read_ID();

  LOG_INFO("BME680 (0x%2X) Chip ID registor (0x%D0) is 0x%02X\n\r", BME_680_DEVICE_ADDR, new_buffer[0]);
}


void read_temp(void)
{
  Si7021Enable();
  /* The below sequence is written based on the datasheet sequence and considerations for BME280*/

//  I2C_Set_IIR_Filter();
  I2C_Set_Mode_Forced();
  timerWaitUs(5000); // 5ms wait
  Compute_Temperature();


  LOG_INFO("BME680 temperature in celsius is %d\n\r", temp_comp);


}
