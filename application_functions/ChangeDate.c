#include "application_functions.h"

extern int ints[];
extern void vTaskDelay();
extern bool mainScreenTimerStopped;

void ChangeDate(void)
{
	unsigned int year, month, day;
	
	mainScreenTimerStopped = true;
	year = ReadKeyPadWithLCD("DATE SET\nYear: 2000", 99);
	vTaskDelay(20);
	month = ReadKeyPadWithLCD("DATE SET\nMonth: ", 12);
	vTaskDelay(20);
	day = ReadKeyPadWithLCD("DATE SET\nDay: ", 1);
	vTaskDelay(20);
	
	/* set date */
	ints[IDD_DAY] = day;
	ints[IDD_MONTH] = month;
	ints[IDD_YEAR] = 2000 + year;
	
	vTaskDelay(20);
	mainScreenTimerStopped = false;
}
