/*
 * tastx.c
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

/* Hardware includes. */
#include "msp430.h"
#include "hal_ETF5438A.h"

#include "tastx.h"


#define tastTASK_3_PRIO  (2)

TaskHandle_t xTask3Handle;

void vTastInitHardware ( void )
{
    P2DIR &= ~(BIT4 | BIT5 | BIT6 | BIT7);
}

/**
 * @brief xTask3
 *
 * Task xTask3 is sends information to task xTask2 via queue xQueueT2,
 * about button 1 and 2 states,
 * if both buttons are pressed at same time it will count as none of them is pressed
 */
void xTask3 ( void *pvParameters )
{

    for ( ;; )
    {
        uint16_t usT2Data = 0;
        if ( ( P2IN & BIT4 ) == 0x00 )
        {
            if ( ( P2IN & BIT5 ) != 0 )
            {
                usT2Data |= BIT1;
                usT2Data &= ~BIT2;
            }
            else if ( ( P2IN & BIT5 ) == 0x00 )
            {
                usT2Data &= ~(BIT1 | BIT2);
            }
        }
        if ( ( P2IN & BIT5 ) == 0x00)
        {
            if ( ( P2IN & BIT4 ) != 0 )
            {
                usT2Data |= BIT2;
                usT2Data &= ~BIT1;
            }
        }

        T2Msg_t xMsg = { TasterLED, usT2Data };
        xQueueSendToBack( xQueueT2, &xMsg, portMAX_DELAY );
    }
}

void vTastInitTask3 ( void )
{
    xTaskCreate( xTask3, "Task 3", configMINIMAL_STACK_SIZE, NULL, tastTASK_3_PRIO, &xTask3Handle );
}
