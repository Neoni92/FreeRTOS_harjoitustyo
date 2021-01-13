#include "application_functions.h"
#include "FreeRTOS.h"
#include "task.h"
extern int ints[];
extern bool mainScreenTimerStopped;
extern int secondsFromMidNight;
extern void vTaskDelay();

void ChangeTime (void)
{
	unsigned int hour, minutes, seconds;
	
	/* stop the mainScreenTimer so the user can set time */
	mainScreenTimerStopped = true;
	
	hour = ReadKeyPadWithLCD("TIME SET\nHours: ", 23);
	vTaskDelay(20);
	minutes = ReadKeyPadWithLCD("TIME SET\nMinutes: ", 59);
	vTaskDelay(20);
	seconds = ReadKeyPadWithLCD("TIME SET\nSeconds: ", 59);
	vTaskDelay(20);
	
	/* set the time */
	secondsFromMidNight = hour*3600L+minutes*60L+seconds;
	vTaskDelay(20);
	
	/* put on the timer again when time is set */
	mainScreenTimerStopped = false;
}
