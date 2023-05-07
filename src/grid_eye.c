/*
 * grid_eye.c
 *
 *  Created on: 23-Apr-2023
 *      Author: gauta
 */


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


#define AMG8833_DEVICE_ADDR 0x68 /* Slave address for Grid Eye */

I2C_TransferReturn_TypeDef transferStatus1;
I2C_TransferSeq_TypeDef transferSequence1;
uint8_t cmd_data1; /* Send commands to the Si7021 sensor*/


uint16_t thermistor_value_grideye = 0xFFFF;
uint8_t thermistor_buffer[2];

static void grid_eye_test(void)
{

  thermistor_buffer[0] = 0xFF;
  thermistor_buffer[1] = 0xFF;

  cmd_data1 = 0x0E; // TTHL
//    uint8_t data = 0x59;
  transferSequence1.addr = AMG8833_DEVICE_ADDR << 1;
  transferSequence1.flags = I2C_FLAG_WRITE_READ;
  transferSequence1.buf[0].data = &cmd_data1;
  transferSequence1.buf[0].len = sizeof(cmd_data1);
  transferSequence1.buf[1].data = &thermistor_buffer[0];
  transferSequence1.buf[1].len = 2;
  transferStatus1 = I2CSPM_Transfer(I2C0, &transferSequence1);
  if (transferStatus1 != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data1, transferStatus1);
  }
}

uint8_t temp_byte;

static void get_pwr_ctl(void)
{
  cmd_data1 = 0x00; // PWR_CTL
  transferSequence1.addr = AMG8833_DEVICE_ADDR << 1;
  transferSequence1.flags = I2C_FLAG_WRITE_READ;
  transferSequence1.buf[0].data = &cmd_data1;
  transferSequence1.buf[0].len = sizeof(cmd_data1);
  transferSequence1.buf[1].data = &temp_byte;
  transferSequence1.buf[1].len = 1;
  transferStatus1 = I2CSPM_Transfer(I2C0, &transferSequence1);
  if (transferStatus1 != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data1, transferStatus1);
  }
}

static void set_pwr_ctl(uint8_t data)
{
  cmd_data1 = 0x00; // PWR_CTL
  transferSequence1.addr = AMG8833_DEVICE_ADDR << 1;
  transferSequence1.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence1.buf[0].data = &cmd_data1;
  transferSequence1.buf[0].len = sizeof(cmd_data1);
  transferSequence1.buf[1].data = &data;
  transferSequence1.buf[1].len = 1;
  transferStatus1 = I2CSPM_Transfer(I2C0, &transferSequence1);
  if (transferStatus1 != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data1, transferStatus1);
  }
}

static void set_frame_rate(void)
{
  cmd_data1 = 0x02; // Frame rate register
  uint8_t data = 0x01; // 1FPS frame rates
  transferSequence1.addr = AMG8833_DEVICE_ADDR << 1;
  transferSequence1.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence1.buf[0].data = &cmd_data1;
  transferSequence1.buf[0].len = sizeof(cmd_data1);
  transferSequence1.buf[1].data = &data;
  transferSequence1.buf[1].len = 1;
  transferStatus1 = I2CSPM_Transfer(I2C0, &transferSequence1);
  if (transferStatus1 != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data1, transferStatus1);
  }
}

static void set_reset_reg(void)
{
  cmd_data1 = 0x01; // Reset register
  uint8_t data = 0x3F; // Initial Reset Value
  transferSequence1.addr = AMG8833_DEVICE_ADDR << 1;
  transferSequence1.flags = I2C_FLAG_WRITE_WRITE;
  transferSequence1.buf[0].data = &cmd_data1;
  transferSequence1.buf[0].len = sizeof(cmd_data1);
  transferSequence1.buf[1].data = &data;
  transferSequence1.buf[1].len = 1;
  transferStatus1 = I2CSPM_Transfer(I2C0, &transferSequence1);
  if (transferStatus1 != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data1, transferStatus1);
  }

}

void grid_eye_init(void)
{
  set_frame_rate();
  set_reset_reg();

}

// Temperature data for pixel
uint8_t pixel_reg_data[128];

static void get_temperature_readings(void)
{
  uint8_t pixel_temp_addr = 0x80;
  transferSequence1.addr = AMG8833_DEVICE_ADDR << 1;
  transferSequence1.flags = I2C_FLAG_WRITE_READ;
  transferSequence1.buf[0].data = &pixel_temp_addr;
  transferSequence1.buf[0].len = sizeof(pixel_temp_addr);
  transferSequence1.buf[1].data = pixel_reg_data;
  transferSequence1.buf[1].len = sizeof(pixel_reg_data);
  transferStatus1 = I2CSPM_Transfer(I2C0, &transferSequence1);
  if (transferStatus1 != i2cTransferDone) {
      LOG_ERROR ("I2CSPM_Transfer: I2C bus write of cmd = %02X with error code: %d\n\r", cmd_data1, transferStatus1);
  }
}

uint16_t pixel_data[64];

static void compute_pixel_data(void)
{
  for (int i = 0; i < 64; i++) {
      pixel_data[i] = (uint16_t) (pixel_reg_data[2*i+1] << 8) | ((uint16_t)pixel_reg_data[2*i]);
  }
}

void grid_eye_temp_test(void)
{

  Si7021Enable();

  uint8_t normal_mode = 0x00;

  set_pwr_ctl(normal_mode);
  timerWaitUs(10000);
  get_temperature_readings();
  compute_pixel_data();

  LOG_INFO("Frame \n\r");
  for (int i = 64; i>=0; i=i-8) {
      LOG_INFO("%d %d %d %d %d %d %d %d\n\r", pixel_data[i-8], pixel_data[i-7], pixel_data[i-6], pixel_data[i-5], pixel_data[i-4], pixel_data[i-3], pixel_data[i-2], pixel_data[i-1]);
  }
  LOG_INFO("\n\n\n\r");
}

void read_grid_eye(void)
{
  Si7021Enable();

  grid_eye_test();

  get_pwr_ctl();

  uint16_t temp = ((uint16_t)thermistor_buffer[1] << 8 | (uint16_t)thermistor_buffer[0]);
  float temperature = (float)temp*0.0625;

  LOG_INFO("Grid Eye power control val is 0x%02X \n\n\n\r", temp_byte);

  LOG_INFO("Grid Eye (0x%2X) thermistor val is 0x%02X (0x0F), 0x%02X (0x0E) the temperature is %0.2f C\n\n\n\r", AMG8833_DEVICE_ADDR, thermistor_buffer[1], thermistor_buffer[0], temperature);
}

/*
 // Initialize the I2C communication protocol
Wire.begin();

// Configure the sensor settings
Wire.beginTransmission(GRID_EYE_ADDRESS);
Wire.write(0x02); // Configuration register
Wire.write(0x01); // 10Hz refresh rate
Wire.endTransmission();

// Initialize the sensor
Wire.beginTransmission(GRID_EYE_ADDRESS);
Wire.write(0x00); // Initial reset register
Wire.write(0x3F); // Reset value
Wire.endTransmission();

// Start measurements
Wire.beginTransmission(GRID_EYE_ADDRESS);
Wire.write(0x01); // Operating mode register
Wire.write(0x01); // Start measurement
Wire.endTransmission();

 * */
