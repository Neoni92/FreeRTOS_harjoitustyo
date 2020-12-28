#include "application_functions.h"

extern int ints[];

void ShowDate(void)
{
	char  szVariable[8];  // variable value will be printed here

	// day
	if(ints[IDD_DAY]<10)
	lcd_putc('0');
	itoa(ints[IDD_DAY],szVariable,10);
	lcd_puts(szVariable);
	lcd_putc('.');
	// month
	if(ints[IDD_MONTH]<10)
	lcd_putc('0');
	itoa(ints[IDD_MONTH],szVariable,10);
	lcd_puts(szVariable);
	lcd_putc('.');
	// year
	itoa(ints[IDD_YEAR],szVariable,10);
	lcd_puts(szVariable);
	
}