/*
 * adcx.h
 *
 *  Created on: 01.07.2018.
 *      Author: vlada
 */

#ifndef ADCX_H_
#define ADCX_H_

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "common.h"

/**
 * Struktura koja sluzi za slanje ADC poruke.
 */
typedef struct
{
    Channel_t eADCNr;   /**< Redni broj kanala sa kog je ocitana vrednost */
    uint16_t ucADCData; /**< 12-bitni rezultat konverzije */
} ADCMsg_t;

/**
 * @brief Inicijalizacija ADC taska
 *
 * Funkcija koja inicijalizuje ADC task i kreira odgovarajuci red sa porukama.
 */
extern void vADCInitTask1(void);

/**
 * @brief Inicijalizacija AD konvertora
 *
 * Funkcija koja konfigurise hardver AD konvertora.
 * Multipleksira odgovarajuce pinove tako da se koriste za AD konvertor.
 * Konfigurise AD konvertor da radi u rezimu Repeat Sequence of channels.
 * Koristi memorijske lokacije 0 do 3 za smestanje rezultata konverzije.
 */
extern void vADCInitHardware(void);

/**
 * Odlozena obrada prekida
 */
extern TaskHandle_t xTask1Handle;

/**
 * Red sa porukama u koji se u prekidnoj rutini smestaju rezultati konverzije
 */
extern QueueHandle_t xADCDataQueue;

#endif /* ADCX_H_ */
