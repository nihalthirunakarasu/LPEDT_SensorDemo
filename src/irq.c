/*
* File Name: irq.c
* File Description: This file contains the interrupt related functions
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#include "em_device.h"
#include "em_letimer.h"
#include "src/gpio.h"
#include <stdint.h>
#include "src/scheduler.h"

/*
 * Function Name: letimer0_irq_init
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This functions enables interrupt for LETIMER0 for UF and COMP1.
 *
 */
void letimer0_irq_init(void)
{
  /*Enabling LETIMER interrupt for UF (underflow)*/
  LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF);
  NVIC_ClearPendingIRQ (LETIMER0_IRQn);
  NVIC_EnableIRQ(LETIMER0_IRQn);
}

/*
 * Function Name: LETIMER0_IRQHandler
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: Interrupt handler for LETIMER0 interrupts.
 *
 */
void LETIMER0_IRQHandler(void)
{
  /* Storing and clearing current LETIMER0 interrupt flags*/
  uint32_t flags = LETIMER_IntGetEnabled(LETIMER0);
  LETIMER_IntClear(LETIMER0, flags);

  if (flags & LETIMER_IF_UF) { /* Call schedulerSetEventUF for UF interrupt */
      schedulerSetEventUF();
  }

}
