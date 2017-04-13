#include <pic.h>
#include "LCD.h"
#include "GPS.h"
#include "display.h"
#include "Delay.h"
__CONFIG (0x3FF2);


void main(void)
{
	unsigned char c = 0;
	lcd_init();
	gps_init();
	TRISC = 0x07;

	busy();
	lcd_puts("Initialisation");
	DelayMs(10);

	while(1)
	{
		receive();
		if(RC0 == 1) c = 0;
		else if(RC1 == 1) c = 1;
		else if(RC2 == 1) c = 2;

		lcd_clear();

		switch(c)
		{
			case 0:
				affichageClassique();
				break;
			case 1: // UTM
				//affichageUTM();
				break;
			case 2: // Lambert III
				//affichageLambert();
				break;
		}
	}
}
