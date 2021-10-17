/*
 * uartx.h
 *
 *  Created on: 01.07.2018.
 *      Author: vlada
 */

#ifndef UARTX_H_
#define UARTX_H_


#include "common.h"


extern void vUARTInitTask2(void);

/** uart test task handle */
extern TaskHandle_t xTask2Handle;

extern QueueHandle_t xQueueT2;


#endif /* UARTX_H_ */
