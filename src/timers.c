/**
* File Name: timers.c
* File Description: This file contains the timer functions
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_letimer.h"
#include "app.h"
#include <stdint.h>

#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

#define WAIT_LOWER_LIMIT 62
#define WAIT_UPPER_LIMIT 3000000

uint32_t clock_freq;
uint32_t counter_value;

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
void low_energy_timerInit(void)
{
  clock_freq = CMU_ClockFreqGet(cmuClock_LETIMER0); /*Get LETIMER0 clock frequency*/

  /* Computing values to load in topValue and comparator 1*/
  counter_value = ((LETIMER_PERIOD_MS)*(clock_freq))/1000;

  LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;
  letimerInit.enable = false;
  letimerInit.topValue = counter_value;

  /* Initialize and enable LETIMER */
  LETIMER_Init(LETIMER0, &letimerInit);
  LETIMER_Enable(LETIMER0, true);
}

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
void timerWaitUs(uint32_t us_wait)
{
  /* Range check for input us_wait*/
  if (us_wait < WAIT_LOWER_LIMIT || us_wait > WAIT_UPPER_LIMIT) {
      LOG_ERROR("Error: Wait Time out of range.\n\r");
      return;
  }

  uint32_t current_count = LETIMER_CounterGet(LETIMER0); /* Retrieve current count*/
  uint32_t wait_count = ((us_wait)*(clock_freq/1000))/1000; /* Compute count to wait for microseconds*/

  /* Computing value to reach for delay with roll around based on topValue*/
  uint32_t count_difference = (current_count - wait_count + counter_value) % (counter_value);

  while (LETIMER_CounterGet(LETIMER0) != (count_difference)); /* Blocking for delay */

}
