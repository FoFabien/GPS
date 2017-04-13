// liblcd.c

#include	<pic.h>
#include "LCD.h"
#include "Delay.h"
unsigned char bfx;



/* initialisation de l'afficheur en mode 8 bits */

 void lcd_init(void)
{
	TRISD = 0xFF; 	//   DATAS en entree 
	/*RD7 = 1; 		alimentation 5V*/
	ADCON1=0b00000110;  // tout le PORTE en mode Digital
	ADON=0;		//CAN OFF
	TRISE =0x00;	// commandes en sortie



	DelayMs(40);		// tempo 40 ms
	lcd_write_instr(0x30);	// ecriture de 3 dans MSB de IR	
	DelayBigUs(4100);			// tempo 4,1 ms
	lcd_write_instr(0x30);	// ecriture de 3 dans MSB de IR	
	DelayUs(200);			// tempo 200 us
	lcd_write_instr(0x30);	// ecriture de 3 dans  MSB de IR	
	DelayUs(200);				
	lcd_write_instr(0x38);	// choix du mode : interface mode 8 bits, N=1 (2 lignes), F=0 (5x7pixels)
	busy ();		// attente BF =1
	lcd_off();
	lcd_clear ();
	busy ();			
	lcd_write_instr(0x06);	//   I/D=1 (increment), SH=0 (mode no shift),
	busy ();			
	lcd_write_instr(0x0F);	// lcd on, curseur on, clignotement on 	
	lcd_home ();	 
}



/* ecriture d'une instruction dans IR */

void lcd_write_instr (unsigned char c)
{	
	// On  charge  les 8 bits  de c dans le PORTD
	E = 0;					// horloge L interface HZ
	TRISD = 0x00; 			// PORTD :  DATAS en sortie 
	TRISE =0x00;			// PORTE : commandes en sortie
	E = 0;					// horloge L interface HZ
	R_W = 0;				// mode ecriture
	RS = 0;					// mode instruction
 	PORTD = c;				// chargement des 8 bits  de c dans le PORTD (Datas de l'afficheur)
	E = 0;					// petite attente
	E = 1 ; 				// horloge monte a H
	E = 1 ;					// petite attente
	E = 0 ; 				// horloge retombe a L donnees memorisees
	E = 0;					// petite attente
}


/* attente BF = 0  en mode 4 bits achtung ! */

	void busy (void)		
{
	bfx = 0;
	
	TRISD = 0xFF; 	// PORTD :  DATAS en entree 
	TRISE =0x00;	// PORTE : commandes en sortie

	E = 0;				// bus HZ
	R_W = 1; 				// mode lecture
	RS = 0;  				// mode instrucion et BF 
	E = 0;
	E=0;

	do 
	{
	E=0;
	E=0;
	E = 1;
	E = 1;				
	bfx = BF;				// echantillonnage de BF
	E = 0;
	E = 0;
 		}
	 while(bfx==1);
}




/* display off*/

void lcd_off (void)
{	
	busy ();	
	lcd_write_instr(0x08);	
}

void lcd_puts(const unsigned char * s)
{
	int i = 0;
	while(s[i] != '\0')
	{
		lcd_putch(s[i]);
		i++;
	}
}


/* effacement lcd */

void lcd_clear (void)	
{
	busy ();	
	lcd_write_instr(0x01);
	DelayMs(3);
}


/* curseur a l'origine*/

void lcd_home (void)	
{	
	busy ();	
	lcd_write_instr(0x02);
}




/* envoi un caractere */

void lcd_putch (unsigned char d)

	{
	busy();
	E = 0;						// horloge L interface HZ
	TRISD = 0x00 ;				// DATAS en sortie
	TRISE=0x00;					//commandes en sortie
	E = 0;						// petite attente
	R_W = 0;					// mode ecriture
	RS = 1;						// mode donnee
	PORTD=d;					// chargement des 8 bits de d
	E = 0;						// petite attente
	E = 1 ; 					// horloge monte a H
	E = 1 ;						// petite attente
	E = 0 ; 					// horloge retombe a L donnees memorisees
	E = 0;						// petite attente
	}

/* choix de la ligne et de la position */

void lcd_pos (unsigned char ligne, unsigned char pos) // 0 = 1ère ligne, 1 = 2ème ligne
{
	char c = 0x80;
	if(ligne == 1) c = c | 0x40;

	c = c | pos;

	busy();
	lcd_write_instr(c);
}
