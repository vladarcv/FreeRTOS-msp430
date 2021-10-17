/*
 * uartx.c
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
#include "uart.h"
#include "queue.h"
#include "timers.h"

/* Hardware includes. */
#include "msp430.h"
#include "hal_ETF5438A.h"

#include "uartx.h"

/** uart test task priority */
#define uartxTASK_2_PRIO       ( 3 )
#define uartxQUEUET2_LEN       ( 16 )

#define uartxTIMER100T2_PERIOD ( pdMS_TO_TICKS( 100 ) ) //100ms
#define uartxLED_TIMER_PERIOD  ( pdMS_TO_TICKS( 5 ) )   //5ms

static void prvReceiveByteCallback( uint8_t ucData );
static void prvUartInitTimer100T2(void);
static void prvUartInit7Seg( void );
static void prvNumberToDigits();
static void prvDigitsToNumber();

//digits definitions, for 7seg display
static const uint8_t segtab[] = {
        0x7e,
        0x30,
        0x6d,
        0x79,
        0x33,
        0x5b,
        0x5f,
        0x70,
        0x7f,
        0x7b
};

static TimerHandle_t xTimer100T2 = NULL;        //timer used for counting while LEDs are on
static TimerHandle_t xTimerDisplay = NULL;      //timer used for managing 7seg display

/** buffer where data received over UART is placed */
static uint16_t usTrigger = 0;                  //used to send message to queue
static uint16_t usActiveTrigger;                //used by xTask2 to manage trigger value

/** number of received characters */
static uint8_t ucRxCnt = 0;
static uint8_t pucRxData[5];

static uint16_t usCounter = 0;                  //counted value while all 4 LEDs are on
static uint8_t ucTriggerDigitsCount = 0;        //represents number of digits in trigger value received via UART
static uint8_t ucCounterDigitsCount = 0;        //represents number of digits in counted value of usCounter
static uint8_t ucCounterDigits [ 4 ] ;          //array of digits or usCounter
static uint8_t ucDigitIndex = 0;                //current digit being displayed on 7seg display, and currently active 7seg display

/** uart test task handle */
TaskHandle_t xTask2Handle = NULL;

/**
 * @brief UART test task
 *
 * Sends "Hello" over UART every 1 second.
 */
void xTask2( void *pvParameters )
{
    T2Msg_t xT2Data;

    for ( ;; )
    {
        /* wait for notification */
        if ( xQueueReceive( xQueueT2, &xT2Data, portMAX_DELAY ) == pdTRUE )
        {
            //sets trigger value, if message is about trigger
            if ( xT2Data.eMsgT == Trigger )
            {
                usActiveTrigger = xT2Data.usT2Data;  //new trigger variable was needed, cause in this task, trigger value can be lowered or raised
            }
            //and if message was about buttons, then on appropriate button press we lower or raise trigger value
            else
            {
                if ( xT2Data.usT2Data == BIT1 )
                {
                    usActiveTrigger++;
                }
                else if ( xT2Data.usT2Data == BIT2 )
                {
                    usActiveTrigger--;
                }
                //in case all LEDs are on
                if ( ( ( P4OUT & LED1 ) != 0 ) && ( ( P4OUT & LED2 ) != 0 ) && ( ( P4OUT & LED3 ) != 0 ) && ( ( P4OUT & LED4 ) != 0 ) )
                {
                    if ( xTimerIsTimerActive( xTimer100T2 ) == pdFALSE )
                    {
                        xTimerStart( xTimer100T2, portMAX_DELAY );
                        xTimerStart( xTimerDisplay, portMAX_DELAY );
                    }
                }
                else
                {
                    if ( xTimerIsTimerActive( xTimer100T2 ) == pdTRUE )
                    {
                        xTimerStop( xTimer100T2, portMAX_DELAY );
                        xTimerStop( xTimerDisplay, portMAX_DELAY );
                        P10OUT |= BIT7 | BIT6;
                        P11OUT |= BIT1 | BIT0;
                    }
                }
            }
        }
        //taking mutex, while accessing mutex protected value
        if( xSemaphoreTake( xSemCH0Mean, portMAX_DELAY ) == pdTRUE )
        {
            //if channel mean value is lower then trigger value, sets appropriate LED on
            if ( ulCH0Mean < usActiveTrigger )
            {
                halSET_LED(LED1);
            }
            //else it sets it appropriate LED off
            else
            {
            halCLR_LED(LED1);
            }
        }
        xSemaphoreGive( xSemCH0Mean ); //giving mutex
        if( xSemaphoreTake( xSemCH1Mean, portMAX_DELAY ) == pdTRUE )
        {
            if ( ulCH1Mean < usActiveTrigger )
            {
                halSET_LED(LED2);
            }
            else
            {
                halCLR_LED(LED2);
            }
        }
        xSemaphoreGive( xSemCH1Mean );
        if( xSemaphoreTake( xSemCH2Mean, portMAX_DELAY ) == pdTRUE )
        {
            if ( ulCH2Mean < usActiveTrigger )
            {
                halSET_LED(LED3);
            }
            else
            {
                halCLR_LED(LED3);
            }
        }
        xSemaphoreGive( xSemCH2Mean );
        if( xSemaphoreTake( xSemCH3Mean, portMAX_DELAY ) == pdTRUE )
        {
            if ( ulCH3Mean < usActiveTrigger )
            {
                halSET_LED(LED4);
            }
            else
            {
                halCLR_LED(LED4);
            }
        }
        xSemaphoreGive( xSemCH3Mean );
        char pucDataL1[10];
        sprintf( pucDataL1, "LED1: %s\r\n", (P4OUT & LED1) ? "1" : "0" );
        xUartSendString( pucDataL1, 0 );                                    //sends LED status over UART
        char pucDataL2[10];
        sprintf( pucDataL2, "LED2: %s\r\n", (P4OUT & LED2) ? "1" : "0" );
        xUartSendString( pucDataL2, 0 );
        char pucDataL3[10];
        sprintf( pucDataL3, "LED3: %s\r\n", (P4OUT & LED3) ? "1" : "0" );
        xUartSendString( pucDataL3, 0 );
        char pucDataL4[10];
        sprintf( pucDataL4, "LED4: %s\r\n", (P4OUT & LED4) ? "1" : "0" );
        xUartSendString( pucDataL4, 0 );

    }
}

/**
 * @brief converting a number into array of digits
 *
 * Function converts some value into array of digits,
 *  so the number could be displayed on 7seg display.
 */
static void prvNumberToDigits()
{
    uint8_t ucIndex;
    uint16_t usNumber = usCounter;

    if ( ( usCounter / 1000 ) > 0 )
    {
        ucCounterDigitsCount = 4;
    }
    else if ( ( usCounter / 100 ) > 0 )
    {
        ucCounterDigitsCount = 3;
    }
    else if ( ( usCounter / 10 ) > 0 )
    {
        ucCounterDigitsCount = 2;
    }
    else
    {
        ucCounterDigitsCount = 1;
    }
    ucIndex = 0;
    while ( ucIndex < ucCounterDigitsCount )
    {
        ucCounterDigits[ ucIndex ] = usNumber % 10;
        usNumber /= 10;
        ucIndex++;
    }
}

/**
 * @brief converting digits array into a number
 *
 * Function converts array of digits into a meaningful value,
 * which could be understood by program.
 */
static void prvDigitsToNumber()
{
    uint8_t ucIndex;
    uint16_t usNumber = 0;

   for ( ucIndex = 1; ucIndex < ucTriggerDigitsCount; ucIndex++ )
   {
       usNumber *= 10;
       usNumber += pucRxData[ucIndex] - '0';
   }
   usTrigger = usNumber;
}

/**
 * @brief 7seg display initialization
 *
 * Function initializes 7seg display.
 */
static void prvUartInit7Seg( void )
{
    P11DIR |= BIT1 | BIT0;      // set P11.1 and P11.0 as out, (SEL1) and (SEL2)
    P10DIR |= BIT7 | BIT6;      // set P10.7 and P10.6 as out, (SEL3) and (SEL4)
    P11OUT |= BIT1 | BIT0;      // enable display 1 and 2
    P10OUT |= BIT7 | BIT6;      // enable display 3 and 4
    P6DIR |= 0x7F;              // set P6.6-P6.0 as out (a-g)
}

/**
 * @brief 100ms timer callback
 *
 * Function that is called every time timer counts to 100ms.
 * Function is counting while all 4 LEDs are on,
 * and it converts counter number into digits array, so it can be displayed on 7seg display.
 */
static void vTimer100T2Callback( TimerHandle_t xTimer100T2 )
{
    if ( usCounter++ == 9999 )
    {
        usCounter = 0;
    }
    prvNumberToDigits();
}

/**
 * @brief display timer callback
 *
 * Function that is called every time 5ms passes.
 * Function firstly turns off all 5 7seg displays,
 * then it sets up digit which should be displayed in appropriate 7seg  display
 * then it turns on appropriate 7seg display and it remains on,
 * until function is called again after 5ms, which assures that 7seg display will be lit appropriately.
 */
static void vTimerDisplayCallback( TimerHandle_t xTimerDisplay )
{
    P10OUT |= BIT7 | BIT6;                              //turning displays 1 and 2 off
    P11OUT |= BIT1 | BIT0;                              //turning displays 3 and 4 off
    P6OUT = segtab[ ucCounterDigits[ ucDigitIndex ] ];
    //turn on appropriate 7seg display based on which digit in array is currently being displayed
    switch ( ucDigitIndex )
    {
        case 0:
            P10OUT &= ~BIT6;
            break;
        case 1:
            P10OUT &= ~BIT7;
            break;
        case 2:
            P11OUT &= ~BIT0;
            break;
        case 3:
            P11OUT &= ~BIT1;
            break;
    }
    //changes which 7seg display will activate, and which digit should be displayed next time function is called
    if ( ucDigitIndex++ == 3 )
    {
        ucDigitIndex = 0;
    }
}

static void prvUartInitTimer100T2(void)
{
    xTimer100T2 = xTimerCreate( "Timer100T2", uartxTIMER100T2_PERIOD, pdTRUE, NULL, vTimer100T2Callback);
}

static void prvUartInitTimerDisplay(void)
{
    xTimerDisplay = xTimerCreate( "TimerDisplay", uartxLED_TIMER_PERIOD, pdTRUE, NULL, vTimerDisplayCallback);
}

/**
 * @brief all necessary initialization
 *
 * Function initializes everything needed for xTask2 and UART to function.
 */
void vUARTInitTask2(void)
{
    prvUartInit7Seg();                                                                                  //7seg-display initialization
    xTaskCreate( xTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, uartxTASK_2_PRIO, &xTask2Handle );   //creating xTask2
    xQueueT2 = xQueueCreate( uartxQUEUET2_LEN, sizeof( T2Msg_t ) );                                     //creating queue xQueueT2
    vUartSetRxCallback( prvReceiveByteCallback );                                                       //setting UART callback function name
    prvUartInitTimer100T2();                                                                            //sets up 100ms timer
    prvUartInitTimerDisplay();                                                                          //sets up a timer used to manage 7seg display
}

/**
 * @brief UART receive callback
 * @param ucData received character
 *
 * Function that is called every time a byte is received over UART.
 * Function is also processing information received from UART,
 *  and formulates  it into a message and send that message into xQueueT2.
 */
static void prvReceiveByteCallback( uint8_t ucData )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* append received byte to buffer */
    pucRxData[ucRxCnt++] = ucData;
    if (ucRxCnt >= 2)
    {
        //ignoring non-digits
        if ( pucRxData[ucRxCnt] != 't' )
        {
            if ( ( pucRxData[ucRxCnt - 1 ] < '0' ) && ( pucRxData[ucRxCnt - 1 ] > '9' ) )
            {
                ucRxCnt--;
            }
        }
    }
    /* when we have received 5 bytes or letter "t", we trigger task */
    if ( ( ucRxCnt == 5 ) || ( pucRxData[ucRxCnt - 1] == 't' ) )
    {
        ucTriggerDigitsCount = (pucRxData[ucRxCnt-1] == 't') ? ucRxCnt - 1 : ucRxCnt;   //determining length of UART message array, which is used in function that converts digits array into a number
        prvDigitsToNumber();                                                            //converts array of digits into a number which represents trigger value
        T2Msg_t xMsg = { Trigger, usTrigger };                                          //formulating message
        xQueueSendToBackFromISR( xQueueT2, &xMsg, &xHigherPriorityTaskWoken );          //sends message to xQueueT2
        usTrigger=0;                                                                    //resets trigger value in preparation to receive new one
        ucRxCnt = 0;                                                                    //resets message array length in preparation to receive new message from UART
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
