/**
* File Name: timers.h
* File Description: This file contains declarations for function in timers.c
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

/*
 * Function Name: low_energy_timerInit
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This functions initializes the Low Energy Timer module by initializing
 * counter value and also sets the compare register value based on LETIMER0 clock
 * frequency set in oscillators.c and LETIMER_PERIOD_MS & LETIMER_ON_TIME_MS macros.
 *
 */
void low_energy_timerInit(void);

#endif /* SRC_TIMERS_H_ */
