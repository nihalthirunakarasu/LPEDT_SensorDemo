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
  uint32_t clock_freq = CMU_ClockFreqGet(cmuClock_LETIMER0); /*Get LETIMER0 clock frequency*/

  /* Computing values to load in topValue and comparator 1*/
  uint32_t counter_value = ((LETIMER_PERIOD_MS)*(clock_freq))/1000;
  uint32_t comp_value = ((LETIMER_ON_TIME_MS)*(clock_freq))/1000;

  LETIMER_Init_TypeDef letimerInit;
  letimerInit.enable = false;
  letimerInit.topValue = counter_value;

  /* Initialize and enable LETIMER */
  LETIMER_Init(LETIMER0, &letimerInit);
  LETIMER_CompareSet(LETIMER0, 1, comp_value);
  LETIMER_Enable(LETIMER0, true);
}
