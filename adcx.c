/*
 * adcx.c
 *
 *  Created on: 01.07.2018.
 *      Author: vlada
 */


/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* Hardware includes. */
#include "msp430.h"
#include "hal_ETF5438A.h"

#include "adcx.h"

/* Task includes. */

#define adcxTASK_1_PRIO          ( 3 )
#define adcxSTART_CONV           do { ADC12CTL0 |= ADC12SC; } while( 0 )
#define adcxTIMER100_PERIOD      ( pdMS_TO_TICKS( 100 ) )
#define adcxADC_DATA_QUEUE_LEN   ( 16 )
#define adcxNUM_OF_SAMPLES       ( 4 )
#define adcxNUM_OF_CHANNELS      ( 4 )

static void prvADCInitTimer100(void);

static TimerHandle_t xTimer100 = NULL;
static uint8_t ucQueueRecivedCounter;
static uint8_t ucSentToQueueCounter;

TaskHandle_t xTask1Handle;
QueueHandle_t xADCDataQueue;

void xTask1( void *pvParameters )
{
    //uint8_t ucCount = 0; //needs fixing
    uint32_t ulCH0Sum = 0;
    uint8_t ucCH0Count = 0;
    uint32_t ulCH1Sum = 0;
    uint8_t ucCH1Count = 0;
    uint32_t ulCH2Sum = 0;
    uint8_t ucCH2Count = 0;
    uint32_t ulCH3Sum = 0;
    uint8_t ucCH3Count = 0;
    ADCMsg_t xADCData;

    for ( ;; )
    {
        // if we received semaphore from adc interrupt routine
        if ( xSemaphoreTake( xSemOffload, portMAX_DELAY ) == pdTRUE )
        {
           //reading 4 messages, one from each channel
            while ( ucQueueRecivedCounter < adcxNUM_OF_CHANNELS )
            {
                // receiving message
                if ( xQueueReceive( xADCDataQueue, &xADCData, portMAX_DELAY ) == pdTRUE )
                {
                    switch ( xADCData.eADCNr )
                    {
                    case CH0:
                        ucQueueRecivedCounter ++;           //counting received messages
                        ucCH0Count++;                       //counting received messages from same channel
                        ulCH0Sum += xADCData.ucADCData;     //adding channel value to channel sum
                        // if we received 4 messages from same channel, we write their average value into  mutex protected variable
                        if ( ucCH0Count == adcxNUM_OF_SAMPLES )
                        {
                            if( xSemaphoreTake( xSemCH0Mean, portMAX_DELAY ) == pdTRUE )
                            {
                                ulCH0Mean = ulCH0Sum / adcxNUM_OF_SAMPLES;
                            }
                            else
                            {
                                /* we should never get here, halt */
                                for( ;; );
                            }
                            xSemaphoreGive( xSemCH0Mean );  //releasing mutex
                            ulCH0Sum = 0;                   //reseting channel sum
                            ucCH0Count = 0;                 //reseting  count  of messages from same channel
                        }
                        break;
                    case CH1:
                        ucQueueRecivedCounter ++;
                        ucCH1Count++;
                        ulCH1Sum += xADCData.ucADCData;
                        if ( ucCH1Count == adcxNUM_OF_SAMPLES )
                        {
                            if( xSemaphoreTake( xSemCH1Mean, portMAX_DELAY ) == pdTRUE )
                            {
                                ulCH1Mean = ulCH1Sum / adcxNUM_OF_SAMPLES;
                            }
                            else
                            {
                                /* we should never get here, halt */
                                for( ;; );
                            }
                            xSemaphoreGive( xSemCH1Mean );
                            ulCH1Sum = 0;
                            ucCH1Count = 0;
                        }
                        break;
                    case CH2:
                        ucQueueRecivedCounter ++;
                        ucCH2Count++;
                        ulCH2Sum += xADCData.ucADCData;
                        if ( ucCH2Count == adcxNUM_OF_SAMPLES )
                        {
                            if( xSemaphoreTake( xSemCH2Mean, portMAX_DELAY ) == pdTRUE )
                            {
                                ulCH2Mean = ulCH2Sum / adcxNUM_OF_SAMPLES;
                            }
                            else
                            {
                                /* we should never get here, halt */
                                for( ;; );
                            }
                            xSemaphoreGive( xSemCH2Mean );
                            ulCH2Sum = 0;
                            ucCH2Count = 0;
                        }
                        break;
                    case CH3:
                        ucQueueRecivedCounter ++;
                        ucCH3Count++;
                        ulCH3Sum += xADCData.ucADCData;
                        if ( ucCH3Count == adcxNUM_OF_SAMPLES )
                        {
                            if( xSemaphoreTake( xSemCH3Mean, portMAX_DELAY ) == pdTRUE )
                            {
                                ulCH3Mean = ulCH3Sum / adcxNUM_OF_SAMPLES;
                            }
                            else
                            {
                                /* we should never get here, halt */
                                for( ;; );
                            }
                            xSemaphoreGive( xSemCH3Mean );
                            ulCH3Sum = 0;
                            ucCH3Count = 0;
                        }
                        break;
                    }
                }
            }
        }
//        /* delay for synchronization */
//        vTaskDelay( mainTASK_SYNC_DELAY );
    }
}

static void vTimer100Callback( TimerHandle_t xTimer )
{
    ucQueueRecivedCounter = 0; //reseting counters
    ucSentToQueueCounter = 0;
    adcxSTART_CONV;            //starts conversion
}

void vADCInitTask1(void)
{
    xTaskCreate( xTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, adcxTASK_1_PRIO, &xTask1Handle );
    xADCDataQueue = xQueueCreate( adcxADC_DATA_QUEUE_LEN, sizeof( ADCMsg_t ) );
    xSemOffload = xSemaphoreCreateBinary();
    xSemCH0Mean = xSemaphoreCreateMutex();
    xSemCH1Mean = xSemaphoreCreateMutex();
    xSemCH2Mean = xSemaphoreCreateMutex();
    xSemCH3Mean = xSemaphoreCreateMutex();
    prvADCInitTimer100();

}

static void prvADCInitTimer100(void)
{
    xTimer100 = xTimerCreate( "Timer100", adcxTIMER100_PERIOD, pdTRUE, NULL, vTimer100Callback);
    if (xTimer100 != 0 )
    {
        xTimerStart( xTimer100, 0 );
    }
}

void vADCInitHardware(void)
{
    /* multipleksiranje pinova */
    P7SEL |= BIT6 | BIT7;
    P5SEL |= BIT0 | BIT1;

    /* podesavanje AD konvertora */
    ADC12CTL0 = ADC12ON | ADC12MSC;     /* koristi MSC */
    ADC12CTL1 = ADC12SHS_0 | ADC12CONSEQ_1 | ADC12SHP;  /* repeat-sequence, SC startuje */
    ADC12MCTL0 = ADC12INCH_14;
    ADC12MCTL1 = ADC12INCH_15;
    ADC12MCTL2 = ADC12INCH_8;
    ADC12MCTL3 = ADC12INCH_9 | ADC12EOS;    /* MEM3 je kraj sekvence */
    ADC12IE |= ADC12IE0 | ADC12IE1 | ADC12IE2 | ADC12IE3;   /* dozvoli sve prekide */
    ADC12CTL0 |= ADC12ENC;      /* dozvoli konverziju */
}

void __attribute__ ( ( interrupt( ADC12_VECTOR ) ) ) vADCISR( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    switch( ADC12IV ) {
        case  6:                    /* Vector  6: ADC12IFG0 */
        {
            ucSentToQueueCounter++;
            ADCMsg_t xMsg = { CH0, ADC12MEM0 };
            xQueueSendToBackFromISR( xADCDataQueue, &xMsg, NULL );
        }
        break;
        case  8:                    /* Vector  8: ADC12IFG1 */
        {
            ucSentToQueueCounter++;
            ADCMsg_t xMsg = { CH1, ADC12MEM1 };
            xQueueSendToBackFromISR( xADCDataQueue, &xMsg, NULL );
        }
        break;
        case 10:                    /* Vector 10: ADC12IFG2 */
        {
            ucSentToQueueCounter++;
            ADCMsg_t xMsg = { CH2, ADC12MEM2 };
            xQueueSendToBackFromISR( xADCDataQueue, &xMsg, NULL );
        }
        break;
        case 12:                    /* Vector 12: ADC12IFG3 */
        {
            ucSentToQueueCounter++;
            ADCMsg_t xMsg = { CH3, ADC12MEM3 };
            xQueueSendToBackFromISR( xADCDataQueue, &xMsg, NULL );
        }
        break;
        default: break;
    }
    // after all 4 channels send a message, we give semaphore, so xTask1 can take over
    if ( ucSentToQueueCounter == 4 )
    {
        xSemaphoreGiveFromISR( xSemOffload, &xHigherPriorityTaskWoken );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}
