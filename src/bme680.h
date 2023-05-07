/*
 * bme680.h
 *
 *  Created on: 22-Apr-2023
 *      Author: gauta
 */

#ifndef SRC_BME680_H_
#define SRC_BME680_H_


void i2c_Init(void);

void get_calibration_parameters(void);

void read_BME680(void);

void read_temp(void);


#endif /* SRC_BME680_H_ */
