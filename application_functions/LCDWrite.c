#include "application_functions.h"
#include "FreeRTOS.h"
#include "task.h"
extern int ints[];
extern float temperatures[];

void LCDWrite(char *text)
{
	char *pDisplay[] = {text};
	volatile char *pChDisplay = 0, /* pointer for *pDisplay, showing the current character */
	*pChVariable = 0;	/* pointer used to print variable value */
	int i;
	char szVariable[8];	/* variable value is printed here */
	i = *text;
	pChDisplay = pDisplay[i];
	
	/* go to the beginning of lcd */
	lcd_gotoxy(0,0);
	
	while(*pChDisplay != 0)
	{
		/* if it is variable printing time */
		if(*pChDisplay == '%')
		{
			/* go past '%' */
			pChDisplay++;
			
			/* what kind of data? */
			switch(*pChDisplay)
			{
				case 'i': pChDisplay++;
				/* index in form of 09, 10... */
				i = (*pChDisplay - '0')*10;
				pChDisplay++;
				i += (*pChDisplay - '0'); /* index to ints array */
				taskENTER_CRITICAL();
				itoa(ints[i], szVariable, 10);
				if (ints[i] < 10)
				{
					taskEXIT_CRITICAL();
					lcd_putc('0');
				}
				else
				taskEXIT_CRITICAL();
				break;
				
				case 't': pChDisplay++;
				i = (*pChDisplay - '0')*10;
				pChDisplay++;
				i += (*pChDisplay - '0');
				taskENTER_CRITICAL();
				ftoa(temperatures[i], szVariable, 1);
				taskEXIT_CRITICAL();
				break;
			}
			/* print the variable */
			pChVariable = szVariable;
			while(*pChVariable != 0)
			{
				lcd_putc(*pChVariable); /* print the char */
				pChVariable++; /* next char */
			}
		}
		else if (*pChDisplay == '\n')
		{
			lcd_gotoxy(0,1); /* 2nd line */
		}
		else
		/* screen *pChDisplay text */
		lcd_putc(*pChDisplay);
		
		pChDisplay++;
	}
}
