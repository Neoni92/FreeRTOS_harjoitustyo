#include "application_functions.h"

extern int ints[];
extern bool mainScreenTimerStopped;
extern int secondsFromMidNight;
extern void vTaskDelay();

void ChangeTime (void)
{
	unsigned int hour, minutes, seconds;
	
	mainScreenTimerStopped = true;
	
	hour = ReadKeyPadWithLCD("TIME SET\nHours: ", 23);
	vTaskDelay(20);
	minutes = ReadKeyPadWithLCD("TIME SET\nMinutes: ", 59);
	vTaskDelay(20);
	seconds = ReadKeyPadWithLCD("TIME SET\nSeconds: ", 59);
	vTaskDelay(20);
	
	/* set the time */
	secondsFromMidNight = hour*3600L+minutes*60L+seconds;
	ints[IDD_SECONDS] = seconds;
	ints[IDD_MINUTES] = minutes;
	ints[IDD_HOUR] = hour;
	
	vTaskDelay(20);
	
	mainScreenTimerStopped = false;
}
