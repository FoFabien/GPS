#ifndef _LCD_H_
#define _LCD_H_

// liblcd.h
#define E RE2 		// horloge de l'afficheur
#define RS RE0  	// donnees ou instructions en mode ecriture
					// 
#define R_W  RE1  	// ecriture ou lecture
#define DB7  RD7	 // en mode 4 bits les donnees 
#define DB6  RD6	 
#define DB5  RD5	 
#define DB4  RD4
#define BF   RD7  // le busy flag de l'afficheur

void lcd_init(void);
void busy (void);
void lcd_write_instr (unsigned char c);
void lcd_putch (unsigned char d);
void lcd_puts (const unsigned char *s);
void lcd_pos (unsigned char ligne, unsigned char pos);
void lcd_clear (void);
void lcd_home (void);
void lcd_off (void);

#endif
