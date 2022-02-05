/*
* File Name: scheduler.c
* File Description: This file contains the scheduler related functions
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "app.h"
#include "src/scheduler.h"
#include <stdint.h>

uint32_t currentEvent = evtNoEvent; /* Global variable for currentEvent*/

/*
 * Function Name: schedulerSetEventUF
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 *
 * Brief: This routine sets the scheduler event evtLETIMER0UF upon LETIMER0
 * underflow.
 *
 */
void schedulerSetEventUF(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  currentEvent |= evtLETIMER0_UF; /* Set Event evtLETIMER0_UF*/
  CORE_EXIT_CRITICAL();
}

/*
 * Function Name: getNextEvent
 *
 * Parameters:
 * none
 *
 * Returns:
 * uint32_t Return the next event
 *
 * Brief: This routine returns 1 event (the next event) to main()code and
 * clears that event.
 *
 */
uint32_t getNextEvent()
{
  uint32_t theEvent, tempEvent, clearEvent;

  if ((currentEvent & (currentEvent - 1)) == 0) { /* Checking if only 1 event is set */
      theEvent = currentEvent; /* Assigning the event and equating clearEvent*/
      clearEvent = evtNoEvent;
  }
  else { /* Check for multiple events; Event priority is assigned from LSB to MSB with LSB of uint32_t having highest priority*/
      tempEvent = currentEvent & (currentEvent - 1); /* Clearing the event with highest priority */
      theEvent = currentEvent ^ tempEvent; /* Setting the nextEvent to be bitwise XOR between tempEvent and currentEvent*/
      clearEvent = tempEvent; /* clearEvent set to the remaining events */
  }

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  currentEvent = clearEvent;
  CORE_EXIT_CRITICAL();

  return (theEvent);
}
