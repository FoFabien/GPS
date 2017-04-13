#include "display.h"
#include "GPS.h"
#include "LCD.h"
#include <math.h>
#include <stdio.h>

extern GPS gps;

#define pi 3.141593f

// DEFINE UTM
#define E0 500 // kilomètres
#define K0 1
#define N0 10000 // kilomètres
#define UTM_a 6378 // kilomètres
#define UTM_e 0.0818192f
#define UTM_e2 (UTM_e*UTM_e)
#define UTM_e4 (UTM_e2*UTM_e2)
#define UTM_e6 (UTM_e4*UTM_e4)

// DEFINE LAMBERT III
#define LAM_a 6378137.f //demi grand axe de l'ellipsoide (m)
#define LAM_e 0.08181919106f //première excentricité de l'ellipsoide
#define LAM_e2 (LAM_e*LAM_e)
#define l0 (3.f*pi/180.f)
#define phi0 (46.5f*pi/180.f) //latitude d'origine en radian
#define phi1 (44.f*pi/180.f) //1er parallele automécoïque
#define phi2 (49.f*pi/180.f) //2eme parallele automécoïque
#define x0 700000 //coordonnées à l'origine
#define y0 6600000 //coordonnées à l'origine


void affichageClassique()
{
	lcd_putch('L');
	lcd_putch('A');
	lcd_putch('T');
	lcd_putch(' ');
	lcd_putch(gps->lat.deg[0]);
	lcd_putch(gps->lat.deg[1]);
	lcd_putch('d');
	lcd_putch(gps->lat.min[0]);
	lcd_putch(gps->lat.min[1]);
	lcd_putch('m');
	lcd_putch(gps->lat.sec[0]);
	lcd_putch(gps->lat.sec[0]);
	lcd_putch('s');
	lcd_putch(gps->lat.o);

	lcd_pos(1, 0);

	lcd_putch('L');
	lcd_putch('O');
	lcd_putch('N');
	lcd_putch(' ');
	lcd_putch(gps->lon.deg[0]);
	lcd_putch(gps->lon.deg[1]);
	lcd_putch('d');
	lcd_putch(gps->lon.min[0]);
	lcd_putch(gps->lon.min[1]);
	lcd_putch('m');
	lcd_putch(gps->lon.sec[0]);
	lcd_putch(gps->lon.sec[0]);
	lcd_putch('s');
	lcd_putch(gps->lon.o);
}

/*void affichageUTM()
{
	float lat = 0.f;
	float lon = 0.f;

	float v = 0.f, A = 0.f, s = 0.f, T = 0.f, C = 0.f, E = 0.f, N = 0.f;
	char line[16];
	char i = 2;

	// CONVERSION DONNEES EN FLOAT ---------------------------------------------------------
	lat = ((gps->lat.sec[0] - '0') * 10.f + (gps->lat.sec[1] - '0') * 1.f) / 60.f;
	lat = ((gps->lat.min[0] - '0') * 10.f + (gps->lat.min[1] - '0') * 1.f + lat) / 60.f;

	lat = (gps->lat.deg[0] - '0') * 10.f
		+ (gps->lat.deg[1] - '0') * 1.f
		+ lat;

	lon = ((gps->lon.sec[0] - '0') * 10.f + (gps->lon.sec[1] - '0') * 1.f) / 60.f;
	lon = ((gps->lon.min[0] - '0') * 10.f + (gps->lon.min[1] - '0') * 1.f + lat) / 60.f;

	lon = (gps->lon.deg[0] - '0') * 10.f
		+ (gps->lon.deg[1] - '0') * 1.f
		+ lon;

	// CALCUL INTERMEDIAIRE ----------------------------------------------------------------
	v = 1.f / sqrt(1.f - UTM_e2 * pow(sin(lat), 2));
	A = lon * cos(lat);
	s = (1.f - UTM_e2 / 4.f) - (3.f * UTM_e4 / 64.f) - (5.f * UTM_e6 / 256.f) * lat
		- ((3.f * UTM_e2 / 8.f) + (3.f * UTM_e4 / 32.f) + (45.f * UTM_e6 / 1024.f)) * sin(2.f * lat)
		+ ((15.f * UTM_e4 / 256.f) + (45.f * UTM_e6 / 1024.f)) * sin(4.f * lat)
		- (35.f * UTM_e6 / 3072.f) * sin(6.f * lat);
	T = pow(tan(lat), 2);
	C = pow(cos(lat), 2) * UTM_e2 / (1 - UTM_e2);

	// CALCUL FINAL -------------------------------------------------------------------------
	E = E0 + K0 * UTM_a * v * (A + ((1.f - T + C) * pow(A, 3) / 6.f) + ((5.f - 18.f * T + pow(T, 2) * pow(A, 5)) / 120.f));
	N = N0 + K0 * UTM_a * (s + v * tan(lat) * ((pow(A, 2) / 2.f)) + (5.f - T + 9.f * C + 4.f * pow(C, 2)) * (pow(A, 4) / 24.f) + (61.f - 58.f * T + pow(T, 2)) * (pow(A, 6) / 720.f));


	// AFFICHAGE
	sprintf(line, "E %.3f", E);
	lcd_puts(line);
	lcd_pos(1, 0);
	sprintf(line, "N %.3f", N);
	lcd_puts(line);
}*/

/*void affichageLambert()
{
	float lat = 0.f;
	float lon = 0.f;

	float tmp = 0.f;

	float phi, l, gN1, gN2;
	float gl0, gl1, gl2, gl;
	float n, c, ys, x93, y93;

	char line[16];

	// CONVERSION DONNEES EN FLOAT ---------------------------------------------------------
	tmp = ((gps->lat.sec[0] - '0') * 10.f + (gps->lat.sec[1] - '0') * 1.f) / 60.f;
	tmp = ((gps->lat.min[0] - '0') * 10.f + (gps->lat.min[1] - '0') * 1.f + tmp) / 60.f;

	lat = (gps->lat.deg[0] - '0') * 10.f
		+ (gps->lat.deg[1] - '0') * 1.f
		+ tmp;

	tmp = ((gps->lon.sec[0] - '0') * 10.f + (gps->lon.sec[1] - '0') * 1.f) / 60.f;
	tmp = ((gps->lon.min[0] - '0') * 10.f + (gps->lon.min[1] - '0') * 1.f + tmp) / 60.f;

	lon = (gps->lon.deg[0] - '0') * 10.f
		+ (gps->lon.deg[1] - '0') * 1.f
		+ tmp;

	// CALCULS

	phi = lat * pi / 180.f;
	l = lon * pi / 180.f;

	//calcul des grandes normales
	gN1 = LAM_a / sqrt(1.f - LAM_e2 * sin(phi1) * sin(phi1));
	gN2 = LAM_a / sqrt(1.f - LAM_e2 * sin(phi2) * sin(phi2));

	//calculs des latitudes isométriques
	gl1 = log(tan(pi/4.f+phi1/2.f)*pow((1-LAM_e*sin(phi1))/(1.f+LAM_e*sin(phi1)),LAM_e/2.f));
	gl2 = log(tan(pi/4.f+phi2/2.f)*pow((1-LAM_e*sin(phi2))/(1.f+LAM_e*sin(phi2)),LAM_e/2.f));
	gl0 = log(tan(pi/4.f+phi0/2.f)*pow((1-LAM_e*sin(phi0))/(1.f+LAM_e*sin(phi0)),LAM_e/2.f));
	gl = log(tan(pi/4.f+phi/2.f)*pow((1-LAM_e*sin(phi))/(1.f+LAM_e*sin(phi)),LAM_e/2.f));

	//calcul de l'exposant de la projection
	n = (log((gN2 * cos(phi2)) / (gN1 * cos(phi1)))) / (gl1 - gl2);

	//calcul de la constante de projection
	c = ((gN1 * cos(phi1)) / n) * exp(n * gl1);

	//calcul des coordonnées
	ys = y0 + c * exp(-1.f * n * gl0);

	x93 = x0 + c * exp(-1.f * n * gl) * sin(n * (l - l0));
	y93 = ys - c * exp(-1.f * n * gl) * cos(n * (l - l0));


	// AFFICHAGE
	sprintf(line, "X %.3f", x93);
	lcd_puts(line);
	lcd_pos(1, 0);
	sprintf(line, "Y %.3f", y93);
	lcd_puts(line);
}
*/
