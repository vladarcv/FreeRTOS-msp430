/*
 * common.h
 *
 *  Created on: 01.07.2018.
 *      Author: vlada
 */

#ifndef COMMON_H_
#define COMMON_H_


#include "semphr.h"

uint32_t ulCH0Mean;
uint32_t ulCH1Mean;
uint32_t ulCH2Mean;
uint32_t ulCH3Mean;

SemaphoreHandle_t xSemOffload;
SemaphoreHandle_t xSemCH0Mean;
SemaphoreHandle_t xSemCH1Mean;
SemaphoreHandle_t xSemCH2Mean;
SemaphoreHandle_t xSemCH3Mean;

QueueHandle_t xQueueT2;
/**
 * Tip za opis rednog broja kanala
 */
typedef enum
{
    CH0,/**< kanal 0 */
    CH1,/**< kanal 1 */
    CH2,/**< kanal 2 */
    CH3 /**< kanal 3 */
} Channel_t;

typedef enum
{
    Trigger,
    TasterLED,
} MessageType_t;

typedef struct
{
    MessageType_t eMsgT;   /**< Redni broj kanala sa kog je ocitana vrednost */
    uint16_t usT2Data;     /**< 12-bitni rezultat konverzije */
} T2Msg_t;


#endif /* COMMON_H_ */
