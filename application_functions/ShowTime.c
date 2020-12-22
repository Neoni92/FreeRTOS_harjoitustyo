#include "application_functions.h"

extern int ints[];

void ShowTime(void)
{
	char  szVariable[8];  // muuttujan arvo tulostetaan tähän

	lcd_gotoxy(0,0);
	// tunnit
	if( ints[IDD_HOUR] < 10)
	lcd_putc('0');
	itoa(ints[IDD_HOUR],szVariable,10);
	lcd_puts(szVariable);
	lcd_putc(':');
	// minuutit
	if( ints[IDD_MINUTES] < 10)
	lcd_putc('0');
	itoa(ints[IDD_MINUTES],szVariable,10);
	lcd_puts(szVariable);
	lcd_putc(':');
	// sekunnit
	if( ints[IDD_SECONDS] < 10)
	lcd_putc('0');
	itoa(ints[IDD_SECONDS],szVariable,10);
	lcd_puts(szVariable);
}
