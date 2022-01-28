/**
* File Name: irq.h
* File Description: This file contains declarations for functions in irq.c
* File Author: Gautama Gandhi
* Tools used: Simplicity Studio IDE
**/
#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_

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
void letimer0_irq_init(void);

#endif /* SRC_IRQ_H_ */
