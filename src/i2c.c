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

#define BME_680_DEVICE_ADDR 0x76 /* Slave address for BME 280 */
#define SI7021_DEVICE_ADDR 0x40 /*Slave address for Si7021 sensor*/
#define MAX_17048_DEVICE_ADDR 0x36 /* Slave address for BME 280 */
//#define AMG8833_DEVICE_ADDR 0x68 /* Slave address for Grid Eye */
#define AMG8833_DEVICE_ADDR 0x69 /* Slave address for Grid Eye */
#define measure_temp_cmd 0xF3 /* Command for measuring temperature */
#define read_pres_cmd1 0x21 /* Reading Pressure 1 for BME280 sensor*/
#define read_pres_cmd2 0x20 /* Reading Pressure 2 for BME280 sensor*/
#define read_pres_cmd3 0x1F /* Reading Pressure 3 for BME280 sensor*/
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
//    .i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
    .i2cMaxFreq = I2C_FREQ_FAST_MAX,
    .i2cClhr = i2cClockHLRStandard
  };

  I2CSPM_Init(&I2C_Config);
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
//  uint8_t data = 0xB6;
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

/* Function to set BME280 in forced mode*/
static void I2C_Set_Mode(void)
{
  cmd_data = 0x74; /* Control Meas register*/
  uint8_t data = 0x06;
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
static void I2C_Set_Config(void)
{
  cmd_data = 0x75; /* Config register */
  uint8_t data = 0x10; /**/
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

/* Function to read pressure from BME280*/
static void I2C_Read_Pressure1(void)
{
  cmd_data = read_pres_cmd1;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = 1;
  transferSequence.buf[1].data = new_buffer;
  transferSequence.buf[1].len = sizeof(new_buffer);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
       LOG_ERROR ("I2CSPM_Transfer: I2C bus read with error code: %d\n\r", transferStatus);
  }
}

/* Function to read version*/
static void MAX_17048_huami(void)
{
  cmd_data = 0x08; /* Control Meas register*/
  transferSequence.addr = 0x36 << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = buffer;
  transferSequence.buf[1].len = sizeof(buffer);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

///* Function to read version*/
//static void MAX_17048_huami(void)
//{
//  cmd_data = 0x08; /* Control Meas register*/
//  transferSequence.addr = (0x6C) << 1;
//  transferSequence.flags = I2C_FLAG_WRITE_READ;
//  transferSequence.buf[0].data = &cmd_data;
//  transferSequence.buf[0].len = sizeof(cmd_data);
//  transferSequence.buf[1].data = buffer;
//  transferSequence.buf[1].len = sizeof(buffer);
//  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
//  if (transferStatus != i2cTransferDone) {
//      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
//  }
//}
//
///* Function to read version*/
//static void MAX_17048_huami(void)
//{
//  cmd_data = 0x08; /* Control Meas register*/
//  transferSequence.addr = (0x36) << 1;
//  transferSequence.flags = I2C_FLAG_WRITE_READ;
//  transferSequence.buf[0].data = &cmd_data;
//  transferSequence.buf[0].len = sizeof(cmd_data);
//  transferSequence.buf[1].data = buffer;
//  transferSequence.buf[1].len = sizeof(buffer);
//  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
//  if (transferStatus != i2cTransferDone) {
//      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
//  }
//}

/* Temperature compensation enum*/
enum temperature_comp {dig_T1_LSB, dig_T1_MSB, dig_T2_LSB, dig_T2_MSB, dig_T3_LSB, dig_T3_MSB};

/* Pressure compensation enum */
enum pressue_comp {dig_P1_LSB = 6, dig_P1_MSB, dig_P2_LSB, dig_P2_MSB, dig_P3_LSB, dig_P3_MSB,
  dig_P4_LSB, dig_P4_MSB, dig_P5_LSB, dig_P5_MSB, dig_P6_LSB, dig_P6_MSB,
  dig_P7_LSB, dig_P7_MSB, dig_P8_LSB, dig_P8_MSB, dig_P9_LSB, dig_P9_MSB};

uint8_t compensation_buffer[24]; /* Compensation buffer */

/* Function to set temperature mode*/
static void set_temperature_mode(void)
{
  cmd_data = 0x74; /* Ctrl meas register */
  uint8_t data = 0x21; /*Extracting single value for temperature*/
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

static uint32_t temperature_raw;

/* Function to retrieve temperature value*/
static void get_temperature_value(void)
{
  cmd_data = 0xFA; /* Temperature register*/
  uint8_t temp_buffer[3];
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

  temperature_raw = (temp_buffer[0] << 12) | (temp_buffer[1] << 4) | ((temp_buffer[2] & 0xF0) >> 4);
}

uint16_t dig_T1_val;
int16_t dig_T2_val, dig_T3_val;
uint16_t dig_P1_val;
int16_t dig_P2_val, dig_P3_val, dig_P4_val, dig_P5_val, dig_P6_val, dig_P7_val, dig_P8_val, dig_P9_val;

static int32_t t_fine;

int32_t var1;
int32_t var2;
int32_t var3;
int32_t var4;
uint32_t var5;
uint32_t pressure;
uint32_t pressure_min = 30000;
uint32_t pressure_max = 110000;

/* Compensation formula adapted from Bosch library*/
static void get_sensor_compensation_values(void)
{
  cmd_data = 0x88;
  transferSequence.addr = BME_680_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = 1;
  transferSequence.buf[1].data = compensation_buffer;
  transferSequence.buf[1].len = sizeof(compensation_buffer);
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
       LOG_ERROR ("I2CSPM_Transfer: I2C bus read with error code: %d\n\r", transferStatus);
  }

  dig_T1_val = (compensation_buffer[dig_T1_MSB] << 8) | (compensation_buffer[dig_T1_LSB]);
  dig_T2_val = (compensation_buffer[dig_T2_MSB] << 8) | (compensation_buffer[dig_T2_LSB]);
  dig_T3_val = (compensation_buffer[dig_T3_MSB] << 8) | (compensation_buffer[dig_T3_LSB]);
  dig_P1_val = (compensation_buffer[dig_P1_MSB] << 8) | (compensation_buffer[dig_P1_LSB]);
  dig_P2_val = (compensation_buffer[dig_P2_MSB] << 8) | (compensation_buffer[dig_P2_LSB]);
  dig_P3_val = (compensation_buffer[dig_P3_MSB] << 8) | (compensation_buffer[dig_P3_LSB]);
  dig_P4_val = (compensation_buffer[dig_P4_MSB] << 8) | (compensation_buffer[dig_P4_LSB]);
  dig_P5_val = (compensation_buffer[dig_P5_MSB] << 8) | (compensation_buffer[dig_P5_LSB]);
  dig_P6_val = (compensation_buffer[dig_P6_MSB] << 8) | (compensation_buffer[dig_P6_LSB]);
  dig_P7_val = (compensation_buffer[dig_P7_MSB] << 8) | (compensation_buffer[dig_P7_LSB]);
  dig_P8_val = (compensation_buffer[dig_P8_MSB] << 8) | (compensation_buffer[dig_P8_LSB]);
  dig_P9_val = (compensation_buffer[dig_P9_MSB] << 8) | (compensation_buffer[dig_P9_LSB]);

  int32_t tvar1 = ((int32_t)((temperature_raw >> 3) - ((int32_t)dig_T1_val << 1)))*((int32_t)dig_T2_val) >> 11;
  int32_t tvar2 = ((((int32_t)((temperature_raw >> 4) - (int32_t)dig_T1_val))*((int32_t)((temperature_raw >> 4) - (int32_t)dig_T1_val)) >> 12) * (((int32_t)dig_T3_val))) >> 14;

  t_fine = tvar1 + tvar2;
}

/* Function to convert pressure in uint32_t format from raw pressure readings*/
static uint32_t compute_pressure(void)
{
  uint32_t pressure_raw = (buffer[0] << 12) | (buffer[1] << 4) | ((buffer[2] & 0xF0) >> 4);
  uint32_t pressure_final;
  var1 = (((int32_t)t_fine) / 2) - (int32_t)64000;
  var2 = (((var1 / 4) * (var1 / 4)) / 2048) * ((int32_t)dig_P6_val);
  var2 = var2 + ((var1 * ((int32_t)dig_P5_val)) * 2);
  var2 = (var2 / 4) + (((int32_t)dig_P4_val) * 65536);
  var3 = (dig_P3_val * (((var1 / 4) * (var1 / 4)) / 8192)) / 8;
  var4 = (((int32_t)dig_P2_val) * var1) / 2;
  var1 = (var3 + var4) / 262144;
  var1 = (((32768 + var1)) * ((int32_t)dig_P1_val)) / 32768;
  /* avoid exception caused by division by zero */
  if (var1)
  {
      var5 = (uint32_t)((uint32_t)1048576) - pressure_raw;
      pressure_final = ((uint32_t)(var5 - (uint32_t)(var2 / 4096))) * 3125;

      if (pressure_final < 0x80000000)
      {
          pressure_final = (pressure_final << 1) / ((uint32_t)var1);
      }
      else
      {
          pressure_final = (pressure_final / (uint32_t)var1) * 2;
      }

      var1 = (((int32_t)dig_P9_val) * ((int32_t)(((pressure_final / 8) * (pressure_final / 8)) / 8192))) / 4096;
      var2 = ((((int32_t)(pressure_final / 4)) * ((int32_t)dig_P8_val))) / 8192;
      pressure = (uint32_t)((int32_t)pressure_final + ((var1 + var2 + dig_P7_val) / 16));

      if (pressure_final < pressure_min)
      {
          pressure_final = pressure_min;
      }
      else if (pressure_final > pressure_max)
      {
          pressure_final = pressure_max;
      }
  }
  else
  {
      pressure_final = pressure_min;
  }

  return pressure_final;
}

/* Calibration initialization function for BME280*/
void BME280_calib_init(void)
{
  i2c_Init();
  I2C_Write_Reset();
  timerWaitUs(10000);
  set_temperature_mode();
  timerWaitUs(10000);
  get_temperature_value();
  get_sensor_compensation_values();
}


/*
 * Function Name: read_pressure_BME280
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This function is used to read and measure the pressure in hecto
 * Pascals from the BME280 sensor.
 *
 */
uint32_t read_pressure_BME280(void)
{
  /* The below sequence is written based on the datasheet sequence and considerations for BME280*/
  I2C_Write_Reset();
  timerWaitUs(10000);
  I2C_Set_Mode();
  I2C_Set_Config();

  timerWaitUs(5000);
  I2C_Read_Pressure1();

  uint32_t pressure = compute_pressure();

  return (pressure/100);
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

// Setting the oversampling value for the humidity
static void set_humidity_oversampling(void)
{
    cmd_data = 0x72; /* Ctrl hum register */
    uint8_t data = 0x01; /*Extracting single value for temperature*/
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

// Setting the oversampling value for the temperature and pressure
static void set_temp_pres_oversampling(void)
{
    cmd_data = 0x72; /* Ctrl meas register */
    uint8_t data = (0x02 << 5) | (0x05 << 2); // Setting oversampling for temperature and pressure
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

// Setting the measurement for forced mode
static void set_forced_mode(void)
{
    cmd_data = 0x72; /* Ctrl meas register */
    uint8_t data = (0x02 << 5) | (0x05 << 2) | (0x01);
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

// Setting the measurement for forced mode
static void set_iir_filter(void)
{
    cmd_data = 0x75; /* Config register */
    uint8_t data = (0x01 << 2);
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

static void set_gas_wait0(void)
{
    cmd_data = 0x64; /* Gas Wait register */
    uint8_t data = 0x59;
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

static void compute_res_heat0(void)
{
    cmd_data = 0x64; /* Gas Wait register */
    uint8_t data = 0x59;
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

static void set_res_heat0(void)
{
    cmd_data = 0x64; /* Gas Wait register */
    uint8_t data = 0x59;
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

static void set_nb_conv(void)
{
    cmd_data = 0x71; /* CTRL GAS 1 register */
    uint8_t data = 0x59;
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

static void set_run_gas_l(void)
{
    cmd_data = 0x71; /* CTRL GAS 1 register */
    uint8_t data = 0x59;
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

uint16_t thermistor_value_grideye = 0xFFFF;
uint8_t thermistor_buffer[2];

static void grid_eye_test(void)
{

  thermistor_buffer[0] = 0xFF;
  thermistor_buffer[1] = 0xFF;

  cmd_data = 0x0E; // TTHL
//    uint8_t data = 0x59;
  transferSequence.addr = AMG8833_DEVICE_ADDR << 1;
  transferSequence.flags = I2C_FLAG_WRITE_READ;
  transferSequence.buf[0].data = &cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferSequence.buf[1].data = &thermistor_buffer[0];
  transferSequence.buf[1].len = 2;
  transferStatus = I2CSPM_Transfer(I2C0, &transferSequence);
  if (transferStatus != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data, transferStatus);
  }
}

void read_grid_eye(void)
{
  Si7021Enable();

  grid_eye_test();

  uint16_t temp = ((uint16_t)thermistor_buffer[1] << 8 | (uint16_t)thermistor_buffer[0]);
  float temperature = (float)temp*0.0625;

  LOG_INFO("Grid Eye (0x%2X) thermistor val is 0x%02X (0x0F), 0x%02X (0x0E) the temperature is %0.2f C\n\n\n\r", AMG8833_DEVICE_ADDR, thermistor_buffer[1], thermistor_buffer[0], temperature);
}




int32_t read_temperature_BME680(void)
{
  int32_t temperature = 0;

  int32_t var1 = 5;


  return temperature;
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
void read_BME680(void)
{
  /* The below sequence is written based on the datasheet sequence and considerations for Si7021*/
  Si7021Enable();
  /* The below sequence is written based on the datasheet sequence and considerations for BME280*/
  I2C_Write_Reset();
  timerWaitUs(MS_DELAY_80);
  i2c_Init();
//  I2C_Write();
//  timerWaitUs(MS_DELAY_10_8);
//  I2C_Read();
//  Si7021Disable();
  /* Temperature formula taken from Si7021-A20 datasheet */
//  uint32_t temp = ((175*((buffer[0]<<8)+buffer[1]))/65536)-47;

  I2C_Read_ID();

  LOG_INFO("BME680 (0x%2X) Chip ID registor (0x%D0) is 0x%02X\n\r", BME_680_DEVICE_ADDR, new_buffer[0]);
}

void read_max_17048(void)
{
  Si7021Enable();
  i2c_Init();

  timerWaitUs(MS_DELAY_80);

  MAX_17048_huami();


//  Si7021Disable();

  uint16_t ID = (buffer[0] << 8) | (buffer[1]);

  LOG_INFO("MAX17048 (0x%2X) IC version register (0x08) is: 0x%02X\n\r", MAX_17048_DEVICE_ADDR, ID);

}
