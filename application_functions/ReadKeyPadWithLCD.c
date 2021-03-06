#include "application_functions.h"

int  ReadKeyPadWithLCD(char *szPrompt, int nMax)
{
	char text[10]={0};
	int nValue=0;
	int key;
	char *p = szPrompt;

	/* show prompt */
	lcd_clrscr();
	while(*p )
	{
		if(*p == '\n')
		lcd_gotoxy(0,1);
		else
		lcd_putc(*p);
		p++;
	}
	do
	{
		while( (key = GetKey()) == NO_KEY); /* wait for button */
		if (key == IDK_UP)
		{
			if(nValue < nMax)
			nValue++;
			
			else if (nValue == nMax)
			nValue = 0;
		}
		else if(key == IDK_DOWN)
		{
			if(nValue)
			nValue--;
			
			else if(!nValue)
			nValue = nMax;
		}
		else if(key == IDK_SELECT)
		{
			while( (key = GetKey()) != NO_KEY); /* wait for button to be free */
			return nValue;
		}
		/* show the value */
		itoa(nValue,text,10);
		
		if (nValue<10)
		{
			lcd_gotoxy(10,1);
			lcd_putc('0');
			lcd_gotoxy(11,1);
		}
		else
		{
			lcd_gotoxy(10,1);
		}
			
		lcd_puts(text);
		
		while( (key = GetKey()) != NO_KEY); /* wait for button to be free */

	}
	while(1);

}
