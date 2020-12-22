
/* 
Simple FreeRTOS project using: Arduino Mega 2560 + DfRobot LCD Keypad Shield + DS1820

*/
#define F_CPU 16000000L

// Scheduler include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "semphr.h"
#include "serial.h"
#include "queue.h"
#include "event_groups.h"

// avr needed for timer
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// c functions
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// local devices
#include "../ds1820/sensor.h"   // temperature functions
#include "..\devices\device.h"  // lcd shield

#include "applications_globals.h" // globals defines and data
#include "../application_functions/application_functions.h"

// main starts
// main creates the tasks and starts the system
int main( void )
{
	// create queueu for lcd task
	xDisplay = xQueueCreate( 2, sizeof(DISPLAY_MESSAGE));

	// create semaphore
	vSemaphoreCreateBinary( xADC );
	vSemaphoreCreateBinary( xDisplaySemaphore);

	// init serial port
    xSerialPort = xSerialPortInitMinimal(0, mainCOM_BAUD_RATE, comBUFFER_LEN ,10);
	
	// init lcd
	lcd_init(LCD_DISP_ON);

	// create tasks
	xTaskCreate( vLcdHandler, 0, configMINIMAL_STACK_SIZE, 0,  (tskIDLE_PRIORITY + 2), NULL );
	xTaskCreate( vDoMeasurements, 0, configMINIMAL_STACK_SIZE, 0,  (tskIDLE_PRIORITY + 2), NULL );
	xTaskCreate( vKeyPadHandler, 0, configMINIMAL_STACK_SIZE, 0,  (tskIDLE_PRIORITY + 2), NULL );
	xTaskCreate( vClock, 0, configMINIMAL_STACK_SIZE, 0, (tskIDLE_PRIORITY + 2), NULL);
	xTaskCreate( vTerminal, 0, configMINIMAL_STACK_SIZE, 0, (tskIDLE_PRIORITY + 2), NULL);
	
	// start scheduler
	vTaskStartScheduler();
	
	// ! - should never get here - !
	return 0;
} // main ends

// task vDoMeasurements starts
// this task measures temperature from our DS1820 sensor, connected to A7 on Arduino Mega
// temperature value is kept in ints[IDD_TEMPERATURE]
// also keeping track of max and min values
static void vDoMeasurements( void *pvParameters )
{
	( void ) pvParameters; // Just to stop compiler warnings.
	int value ;

		static unsigned char  numberOfSensors;
		
		// number of sensors on the same wire (on this case only 1)
		numberOfSensors = GetSensorCount();
		
		// max and min temperature values, temperature times 10x because integer
		ints[IDD_MAXTEMPERATURE] = -600;
		ints[IDD_MINTEMPERATURE] = 1300;

		for( ;; )
		{

			if (numberOfSensors)
			{
				value  = (int)(GetTemperature(0)/1000);
		        taskENTER_CRITICAL();
		        ints[ IDD_TEMPERATURE ] = value ;
		        taskEXIT_CRITICAL();
				
				if (value >ints[IDD_MAXTEMPERATURE])
				{
					ints[IDD_MAXTEMPERATURE] = value;
				}
				if (value <ints[IDD_MINTEMPERATURE])
				{
					ints[IDD_MINTEMPERATURE] = value;
				}
			}

			vTaskDelay(100);
		}
	
} // task vDoMeasurements ends

// task vLcdHandler starts
// this task handles the lcd, showing the needed and requested information on the screen
static void vLcdHandler( void *pvParameters )
{
	static DISPLAY_MESSAGE message;

	( void ) pvParameters;	/* Just to stop compiler warnings. */

	for( ;; )
	{
		xQueueReceive(xDisplay,&message, portMAX_DELAY); // wait until message received
		
		// check if the screen is the same as the last one, to reduce the screen interrupt
		/*if (LastScreen != message.idMessage)
		{
			// clear screen
			lcd_clrscr();
			LastScreen = message.idMessage;
		}*/
		
		
		switch( message.idMessage)
		{
					
			
		}
	}
} // task vLcdHandler ends

// task vKeyPadHandler starts
// this task handles the button presses on the shield from the user
static void vKeyPadHandler( void *pvParameters )
{
	static unsigned char ch = 0;
	static DISPLAY_MESSAGE message;
	
	( void ) pvParameters; /* Just to stop compiler warnings. */

	for( ;; )
	{
		do
		{
			xSemaphoreTake( xADC, portMAX_DELAY );
			ch =GetKey();vTaskDelay(1); // get the key value through ADC
			xSemaphoreGive( xADC );

		}while (ch == NO_KEY);

		switch( ch )
		{
			case IDK_DOWN: case IDK_UP: case IDK_RIGHT: case IDK_LEFT: case IDK_SELECT:
			
			taskENTER_CRITICAL();
			//ints[IDD_LASTKEY] = ch;
			taskEXIT_CRITICAL();
			// send message to lcd task to update the screen
			message.data      = 0; // näytön numero
			message.idMessage = IDM_UPDATE_DISPLAY;
			xQueueSend( xDisplay, (void*)&message,0);
			break;
			default:; // other buttons
		}
	}
} // task vKeyPadHandler ends

// task vClock starts
// this task keeps track of time in the background
static void vClock( void *pvParameters )
{
	( void ) pvParameters; // Just to stop compiler warnings.

	vSemaphoreCreateBinary( xClock ); // create semaphore

	StartTimer(125); // = interrupt every 8ms, 8*125 = 1000ms = 1s
	
	for( ;; )//.........
	{
		xSemaphoreTake( xClock, portMAX_DELAY ); // wait for interrupt

		taskENTER_CRITICAL();
		ints[ IDD_HOUR ]   =  secondsFromMidNight / 3600L;
		ints[ IDD_MINUTES ]= (secondsFromMidNight % 3600L) / 60L ;
		ints[ IDD_SECONDS ]=  secondsFromMidNight % 60L;
		taskEXIT_CRITICAL();
		// show the time
		ShowTime();
	}
} // task vClock ends

// task vTerminal starts
// use this task to print data to the terminal
static void vTerminal( void *pvParameters )
{
	char szVariable [8];
	
	const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

	( void ) pvParameters; /* Just to stop compiler warnings. */
	
	for( ;; )
	{
		// print temperature to serial every second
		itoa(ints[IDD_TEMPERATURE],szVariable,10);
		xSerialxPrintf(&xSerialPort, szVariable);
		xSerialxPrintf(&xSerialPort, "\n\r");
		vTaskDelay( xDelay );
	}
} // task vTerminal ends

// timer0
SIGNAL(TIMER0_COMPA_vect) {
	static BaseType_t xTaskWoken = pdFALSE;
	static unsigned msCounter = 0;
	msCounter += 8 ; // interval 8ms
	TCNT0 = 0; // to the start of the counter
	//timer0 interrupt handler
	if ( msCounter == 1000)
	{
		secondsFromMidNight++;
		xSemaphoreGiveFromISR( xClock, &xTaskWoken ); // notify the change in time
		msCounter = 0;
	}
}
