/*
* File Name: scheduler.h
* File Description: This file contains the declarations for functions in scheduler.c
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

/*Macro to set event number whenever a new event is added; each event is a multiple of 2 starting from (2^0) = 1*/
#define event_number_set(x) (1<<((x)-1))

/* Enum for events; each new event is added with event_number_set(event number)*/
typedef enum {
  evtNoEvent = 0,
  evtLETIMER0_UF = event_number_set(1),
} evt_t;

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
void schedulerSetEventUF(void);

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
uint32_t getNextEvent();

#endif /* SRC_SCHEDULER_H_ */
