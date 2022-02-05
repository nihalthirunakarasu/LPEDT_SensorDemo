/*
* File Name: i2c.h
* File Description: This file contains the declarations for functions in i2c.c
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

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
void read_temperature_Si7021(void);

#endif /* SRC_I2C_H_ */
