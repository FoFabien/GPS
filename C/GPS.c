#include <pic.h>
#include <math.h>
#include "GPS.h"
#include "Delay.h"
#include "LCD.h"

#define TAILLE_BUFFER 42

struct buffer buf;
GPS gps;

void gps_init(void)
{
	gps = &buf;
	gps->lat.deg[0] = '0';
	gps->lat.deg[1] = '0';
	gps->lat.min[0] = '0';
	gps->lat.min[1] = '0';
	gps->lat.sec[0] = '0';
	gps->lat.sec[1] = '0';
	gps->lat.o = ' ';

	gps->lon.deg[0] = '0';
	gps->lon.deg[1] = '0';
	gps->lon.min[0] = '0';
	gps->lon.min[1] = '0';
	gps->lon.sec[0] = '0';
	gps->lon.sec[1] = '0';
	gps->lon.o = ' ';
	TRISA = 0xFB;
}

void receive(void)
{
	char c[TAILLE_BUFFER] = "\0";
	int i = 0, j = 0, k = 0;
	volatile long tmp;

	do
	{
		do
		{
			c[0] = gps_getc();
		}while(c[0] != '$'); // premier caractère
		for(i = 1; i < 6; i++)
			c[i] = gps_getc();
	}while(c[3] != 'G' || c[4] != 'G' || c[5] != 'A');

	for(i = 6; i < TAILLE_BUFFER; i++)
		c[i] = gps_getc();

	gps->lat.deg[0] = c[18];
	gps->lat.deg[1] = c[19];
	gps->lat.min[0] = c[20];
	gps->lat.min[1] = c[21];

	tmp = ((long)(c[23] - '0')) * 1000 // Conversion en seconde
		+ ((long)(c[24] - '0')) * 100
		+ ((long)(c[25] - '0')) * 10
		+ ((long)(c[26] - '0'));
	tmp *= 60;
	tmp /= 10000;

	gps->lat.sec[0] = (char)(tmp / 10) + '0';
	gps->lat.sec[1] = (char)(tmp % 10) + '0';

	gps->lat.o = c[28];

	gps->lon.deg[0] = c[31];
	gps->lon.deg[1] = c[32];
	gps->lon.min[0] = c[33];
	gps->lon.min[1] = c[34];

	tmp = ((long)(c[36] - '0')) * 1000 // Conversion en seconde
		+ ((long)(c[37] - '0')) * 100
		+ ((long)(c[38] - '0')) * 10
		+ ((long)(c[39] - '0'));
	tmp *= 60;
	tmp /= 10000;

	gps->lon.sec[0] = (char)(tmp / 10) + '0';
	gps->lon.sec[1] = (char)(tmp % 10) + '0';

	gps->lon.o = c[41];
}

char gps_getc(void)
{
	char c = 0;
	signed int i = 0;
	char ok = 1;

	while(RA0 == 1);

	for(i = 0; i < 8; i++)
	{
		DelayUs(200);
		c = c | (RA0 << i);
	}

	DelayUs(200);

	return c;
}
