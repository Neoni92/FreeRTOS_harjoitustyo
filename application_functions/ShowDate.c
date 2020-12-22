#include "application_functions.h"

extern int intsTime[];

void ShowDate(void)
{
	char  szVariable[8];  // variable value will be printed here

	// day
	if(intsTime[IDD_DAY]<10)
	lcd_putc('0');
	itoa(intsTime[IDD_DAY],szVariable,10);
	lcd_puts(szVariable);
	lcd_putc('.');
	// month
	if(intsTime[IDD_MONTH]<10)
	lcd_putc('0');
	itoa(intsTime[IDD_MONTH],szVariable,10);
	lcd_puts(szVariable);
	lcd_putc('.');
	// year
	itoa(intsTime[IDD_YEAR],szVariable,10);
	lcd_puts(szVariable);
	
}