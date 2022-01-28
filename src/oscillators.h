/**
* File Name: oscillators.h
* File Description: This file contains declarations for function in oscillators.c
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/


#ifndef SRC_OSCILLATORS_H_
#define SRC_OSCILLATORS_H_

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
void oscillatorInit(void);

#endif /* SRC_OSCILLATORS_H_ */
