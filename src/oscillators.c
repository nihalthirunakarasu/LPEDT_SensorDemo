/*
* File Name: oscillators.c
* File Description: This file contains the oscillators functions
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "app.h"

/*
 * Function Name: oscillatorInit
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This functions initializes the oscillator.
 * For EM0, EM1 and EM2, LFXO oscillator is given as the clock source.
 * For EM3, ULFRCO is given as the clock source.
 *
 */
void oscillatorInit(void)
{
  if(LOWEST_ENERGY_MODE == 3) { /* For EM3, setting ULFRCO (1000 Hz) as oscillator*/
      CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
      /* Enable and set ULFRCO for LFACLK */
      CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
      CMU_ClockEnable(cmuClock_LFA, true);
      CMU_ClockEnable(cmuClock_LETIMER0, true);
  }
  else { /* Setting LFXO (32768 Hz) as oscillator for modes EM0, EM1, EM2*/
      CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
      /* Enable and set LFXO for LFACLK */
      CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

      /* Clock frequency divided by 2: 32768/2 = 16384 Hz to extend timer period*/
      CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_2);

      CMU_ClockEnable(cmuClock_LFA, true);
      CMU_ClockEnable(cmuClock_LETIMER0, true);
  }
}
