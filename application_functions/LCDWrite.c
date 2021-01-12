#include "application_functions.h"
extern int ints[];
extern float temperatures[];

void LCDWrite(char *text)
{
	char *pDisplay[] = {text};
	volatile char *pChDisplay = 0,
	*pChVariable = 0;
	int i;
	char szVariable[8];
	pChDisplay = pDisplay[0];
	
	lcd_gotoxy(0,0);
	
	while(*pChDisplay != 0)
	{
		/* if it is variable printing time */
		if(*pChDisplay == '%')
		{
			pChDisplay++;
			
			switch(*pChDisplay)
			{
				case 'i': pChDisplay++;
				
				i = (*pChDisplay - '0')*10;
				pChDisplay++;
				i += (*pChDisplay - '0');
				itoa(ints[i], szVariable, 10);
				break;
				
				case 't': pChDisplay++;
				i = (*pChDisplay - '0')*10;
				pChDisplay++;
				i += (*pChDisplay - '0');
				ftoa(temperatures[i], szVariable, 1);
				break;
			}
			
			pChVariable = szVariable;
			while(*pChVariable != 0)
			{
				lcd_putc(*pChVariable);
				pChVariable++;
			}
		}
		else if (*pChDisplay == '\n')
		{
			lcd_gotoxy(0,1);
		}
		else
		
		lcd_putc(*pChDisplay);
		
		pChDisplay++;
	}
}
