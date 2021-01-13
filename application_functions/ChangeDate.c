#include "application_functions.h"
#include "FreeRTOS.h"
#include "task.h"
extern int ints[];
extern void vTaskDelay();
extern bool mainScreenTimerStopped;

void ChangeDate(void)
{
	unsigned int year, month, day;
	
	/* stop the mainScreenTimer so the user can set date */
	mainScreenTimerStopped = true;
	year = ReadKeyPadWithLCD("DATE SET\nYear:   2021", 99);
	vTaskDelay(20);
	month = ReadKeyPadWithLCD("DATE SET\nMonth: ", 12);
	if (month == 0)
	{
		month = 1;
	}
	vTaskDelay(20);
	day = ReadKeyPadWithLCD("DATE SET\nDay: ", 31);
	if (day == 0)
	{
		day = 1;
	}
	vTaskDelay(20);
	
	/* set date */
	ints[IDD_DAY] = day;
	ints[IDD_MONTH] = month;
	ints[IDD_YEAR] = 2000 + year;
	vTaskDelay(20);
	/* put on the timer again when date is set */
	mainScreenTimerStopped = false;
}
