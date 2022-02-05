/*
* File Name: timers.h
* File Description: This file contains declarations for function in timers.c
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#include <stdint.h>

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

/*
 * Function Name: timerWaitUs
 *
 * Parameters:
 * uint32_t us_wait Wait time in microseconds
 *
 * Returns:
 * none
 *
 * Brief: This function implements a polling based delay based on the time
 * specified by us_wait parameter. The delay is implemented relative to the
 * current LETIMER counter value with roll around handled.
 *
 */
void timerWaitUs(uint32_t us_wait);

#endif /* SRC_TIMERS_H_ */
