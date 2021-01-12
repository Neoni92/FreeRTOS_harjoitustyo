/* 
Simple FreeRTOS project using: Arduino Mega 2560 + DfRobot LCD Keypad Shield + DS1820.
LCD shows the time, date and current temperature as the default screen. User can change
the date and time, search for max and min temperatures recorded through the button presses
on the shield. 
*/

#define F_CPU 16000000L
/* avr needed for timer */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* c functions */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* local devices */
#include "../ds1820/sensor.h"   /* temperature functions */
#include "..\devices\device.h"  /* lcd shield */

#include "applications_globals.h" /* globals defines and data */
#include "../application_functions/application_functions.h"

/* -- main starts -- */
/* main creates the tasks and starts the system */
int main( void )
{
	/* create queueu for lcd task */
	xDisplay = xQueueCreate( 2, sizeof(DISPLAY_MESSAGE));

	/* create semaphores */
	vSemaphoreCreateBinary( xADC );
	vSemaphoreCreateBinary( xDisplaySemaphore);

	/* init serial port */
    xSerialPort = xSerialPortInitMinimal(0, mainCOM_BAUD_RATE, comBUFFER_LEN ,10);
	
	/* init lcd */
	lcd_init(LCD_DISP_ON);

	/* create tasks */
	xTaskCreate( vLcdHandler, 0, configMINIMAL_STACK_SIZE, 0,  (tskIDLE_PRIORITY + 2), NULL );
	xTaskCreate( vDoMeasurements, 0, configMINIMAL_STACK_SIZE, 0,  (tskIDLE_PRIORITY + 2), NULL );
	xTaskCreate( vKeyPadHandler, 0, configMINIMAL_STACK_SIZE, 0,  (tskIDLE_PRIORITY + 2), NULL );
	xTaskCreate( vClock, 0, configMINIMAL_STACK_SIZE, 0, (tskIDLE_PRIORITY + 2), NULL);
	xTaskCreate( vTerminal, 0, configMINIMAL_STACK_SIZE, 0, (tskIDLE_PRIORITY + 2), NULL);
	
	/* start scheduler */
	vTaskStartScheduler();
	
	/* ! - should never get here - ! */
	return 0;
} /* main ends */

/* ------------------------ */
/* -- APPLICATION TASKS -- */

/* task vDoMeasurements starts */
/* this task measures temperature from our DS1820 sensor, connected to A7 on Arduino Mega,
temperature value is kept in ints[IDD_TEMPERATURE] also keeping track of max and min values */
static void vDoMeasurements( void *pvParameters )
{
	( void ) pvParameters; /* just to stop compiler warnings */
	
	float value ; /* temperature value */

		static unsigned char  numberOfSensors;
		
		/* number of sensors on the same wire (on this case only 1) */
		numberOfSensors = GetSensorCount();
		
		/* max and min temperature values, temperature times 10x because integer */
		temperatures[IDD_MAXTEMPERATURE] = -600;
		temperatures[IDD_MINTEMPERATURE] = 1300;

		for( ;; )
		{

			if (numberOfSensors)
			{
				value  = (float)(GetTemperature(0)/10000.0); /* convert long value to float */
		        taskENTER_CRITICAL();
		        temperatures[IDD_TEMPERATURE] = value;
		        taskEXIT_CRITICAL();
				
				/* save as a max or min temperature depending on value */
				if (value > temperatures[IDD_MAXTEMPERATURE])
				{
					temperatures[IDD_MAXTEMPERATURE] = value;
				}
				if (value < temperatures[IDD_MINTEMPERATURE])
				{
					temperatures[IDD_MINTEMPERATURE] = value;
				}
			}

			vTaskDelay(100);
		}
	
} /* task vDoMeasurements ends */

/* task vLcdHandler starts */
/* this task handles the lcd, showing the needed and required information on the screen */
static void vLcdHandler( void *pvParameters )
{
	static DISPLAY_MESSAGE message;

	( void ) pvParameters;	/* just to stop compiler warnings */

	for( ;; )
	{
		xQueueReceive(xDisplay,&message, portMAX_DELAY); /* wait until message received */
		
		/* check if the screen is the same as the last one, to reduce the screen interrupt */
		if (previousScreen != message.idMessage)
		{
			/* clear screen */
			lcd_clrscr();
			previousScreen = message.idMessage;
		}
		
		
		switch( message.idMessage)
		{	
			/* main screen showing time and date + current temperature */
			case IDM_DISPLAY_MAIN:
			xSemaphoreTake(xDisplaySemaphore, portMAX_DELAY);
			taskENTER_CRITICAL();
			LCDWrite(message.data);
			taskEXIT_CRITICAL();
			xSemaphoreGive(xDisplaySemaphore);
			break;
			
			/* the screen where user selects whether to update date or time */
			case IDM_UPDATE_DISPLAY:
			xSemaphoreTake(xDisplaySemaphore, portMAX_DELAY);
			taskENTER_CRITICAL();
			LCDWrite(message.data);
			taskEXIT_CRITICAL();
			xSemaphoreGive(xDisplaySemaphore);
			break;
			
			case IDM_DISPLAY_MINMAX:
			xSemaphoreTake(xDisplaySemaphore, portMAX_DELAY);
			taskENTER_CRITICAL();
			LCDWrite(message.data);
			taskEXIT_CRITICAL();
			xSemaphoreGive(xDisplaySemaphore);
			break;
		}
	}
} /* task vLcdHandler ends */

/* task vKeyPadHandler starts */
/* this task handles the button presses on the shield from the user */
static void vKeyPadHandler( void *pvParameters )
{
	static unsigned char ch = 0;
	static DISPLAY_MESSAGE message;
	
	( void ) pvParameters; /* just to stop compiler warnings */

	for( ;; )
	{
		do
		{
			xSemaphoreTake( xADC, portMAX_DELAY );
			ch =GetKey();vTaskDelay(1); /* get the key value through ADC */
			xSemaphoreGive( xADC );

		}while (ch == NO_KEY);
		
		mainScreenTimer = 0;

		switch( ch )
		{
			case IDK_SELECT:
			/* send message to lcd task to update the screen */
			message.idMessage = IDM_UPDATE_DISPLAY;
			message.data = "Up=Set Time\nDown=Set Date";
			xQueueSend( xDisplay, (void*)&message,0);
			break;
			
			case IDK_DOWN: case IDK_UP:
			if (previousScreen == IDM_UPDATE_DISPLAY)
			{
				if (ch == IDK_UP)
				{
					xSemaphoreTake(xADC, portMAX_DELAY);
					ChangeTime();
					vTaskDelay(20);
					xSemaphoreGive(xADC);
					message.idMessage = IDM_DISPLAY_MAIN;
				}
				else if (ch == IDK_DOWN)
				{
					xSemaphoreTake(xADC, portMAX_DELAY);
					ChangeDate();
					vTaskDelay(20);
					xSemaphoreGive(xADC);
					message.idMessage = IDM_DISPLAY_MAIN;
				}
			}
			else if (previousScreen != IDM_DISPLAY_UPDATE_DATE && previousScreen != IDM_DISPLAY_UPDATE_TIME)
			{
				message.idMessage = IDM_DISPLAY_MINMAX;
				message.data = "Max:%t01C \nMin:%t02C";
			}
			xQueueSend(xDisplay, (void*)&message,0);
			break;
			
			case IDK_RIGHT: case IDK_LEFT:
			
			break;
			taskENTER_CRITICAL();
			//ints[IDD_LASTKEY] = ch;
			taskEXIT_CRITICAL();
			default:; /* other buttons */
		}
	}
} /* task vKeyPadHandler ends */

/* task vClock starts */
/* this task keeps track of time in the background */
static void vClock( void *pvParameters )
{
	static DISPLAY_MESSAGE message;
	( void ) pvParameters; /* just to stop compiler warnings */

	vSemaphoreCreateBinary( xClock ); /* create semaphore */

	StartTimer(125); /* = interrupt every 8ms, 8*125 = 1000ms = 1s */
	
	if (!mainScreenTimerStopped)
	{
		mainScreenTimer++;
	}
	
	/* message for lcd handler */
	message.idMessage = IDM_DISPLAY_MAIN;
	
	/* default data for date */
	ints[IDD_DAY] = 10;
	ints[IDD_MONTH] = 1;
	ints[IDD_YEAR] = 2021;
	
	/* default data for time */
	ints[IDD_HOUR] = 12;
	ints[IDD_MINUTES] = 59;
	ints[IDD_SECONDS] = 59;
	
	for( ;; )
	{
		xSemaphoreTake( xClock, portMAX_DELAY ); /* wait for interrupt */

		taskENTER_CRITICAL();
		ints[ IDD_HOUR ]   =  secondsFromMidNight / 3600L;
		ints[ IDD_MINUTES ]= (secondsFromMidNight % 3600L) / 60L ;
		ints[ IDD_SECONDS ]=  secondsFromMidNight % 60L;
		taskEXIT_CRITICAL();
		
		/* whole day gone by */
		if (secondsFromMidNight > (24 * 3600L))
		{
			secondsFromMidNight = 0;
			ints[IDD_DAY]++;
		}
		
		/* whole month gone by */
		if (ints[IDD_DAY] > 31)
		{
			ints[IDD_DAY] = 1;
			ints[IDD_MONTH]++;
		}
		
		/* whole year gone by */
		if (ints[IDD_MONTH] > 12)
		{
			ints[IDD_MONTH] = 1;
			ints[IDD_YEAR]++;
		}
		
		/* prepare message data for lcd */
		message.data = "%i00:%i01 %i03.%i04.%i05\n%t00 C";
		
		if (mainScreenTimer >= WAIT_BEFORE_MAIN_DISPLAY)
		{
			/* show the time */
			xQueueSend( xDisplay, (void*)&message, 0);
		}
		
	}
} /* task vClock ends */

/* task vTerminal starts */
/* use this task to print data to the terminal, currently send temperature every second*/
static void vTerminal( void *pvParameters )
{
	char szVariable [20];
	
	const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

	( void ) pvParameters; /* just to stop compiler warnings */
	
	for( ;; )
	{
		/* print temperature to serial every second */
		ftoa(temperatures[IDD_TEMPERATURE], szVariable, 1);
		xSerialxPrintf(&xSerialPort, szVariable);
		xSerialxPrintf(&xSerialPort, "\n\r");
		vTaskDelay( xDelay );
	}
} /* task vTerminal ends */

/* - APPLICATION TASKS END - */
/* ------------------------- */

/* timer0 */
SIGNAL(TIMER0_COMPA_vect) {
	static BaseType_t xTaskWoken = pdFALSE;
	static unsigned msCounter = 0;
	msCounter += 8 ; /* interval 8ms */
	TCNT0 = 0; /* to the start of the counter */
	/* timer0 interrupt handler */
	if ( msCounter == 1000)
	{
		secondsFromMidNight++;
		xSemaphoreGiveFromISR( xClock, &xTaskWoken ); /* notify the change in time via semaphore */
		msCounter = 0;
	}
}
