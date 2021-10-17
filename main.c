/**
 * @file main.c
 * @author Strahinja Jankovic (jankovics@etf.bg.ac.rs)
 * @date 2016
 * @brief SRV Template
 *
 * Template to be used for lab work and projects
 */

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Hardware includes. */
#include "msp430.h"
#include "hal_ETF5438A.h"

#include "adcx.h"
#include "uartx.h"
#include "tastx.h"


static void prvSetupHardware( void );

/**
 * @brief main function
 */
void main( void )
{
    /* Configure peripherals */
    prvSetupHardware();

    vADCInitTask1();
    vUARTInitTask2();
    vTastInitTask3();

    /* Start the scheduler. */
    vTaskStartScheduler();

    /* If all is well then this line will never be reached.  If it is reached
    then it is likely that there was insufficient (FreeRTOS) heap memory space
    to create the idle task.  This may have been trapped by the malloc() failed
    hook function, if one is configured. */	
    for( ;; );
}

/**
 * @brief Configure hardware upon boot
 */
static void prvSetupHardware( void )
{
    taskDISABLE_INTERRUPTS();

    /* Disable the watchdog. */
    WDTCTL = WDTPW + WDTHOLD;

    /* Configure Clock. Since we aren't using XT1 on the board,
     * configure REFOCLK to source FLL adn ACLK.
     */
    SELECT_FLLREF(SELREF__REFOCLK);
    SELECT_ACLK(SELA__REFOCLK);
    hal430SetSystemClock( configCPU_CLOCK_HZ, configLFXT_CLOCK_HZ );

    /* Enable buttons */
    P2DIR &= ~(0xF0);

    /* initialize LEDs */
    vHALInitLED();

    vADCInitHardware();
    vUartInit();
    vTastInitHardware();
}
