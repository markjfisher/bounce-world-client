#ifndef BWC_ADAM_CONIO_HELPERS_H
#define BWC_ADAM_CONIO_HELPERS_H

#ifdef __ADAM__

#include <stdint.h>

void init_vdp();

void cputsxy(uint8_t x, uint8_t y, char *s);
void cputcxy(uint8_t x, uint8_t y, char c);
int cputc(char c);

void gotox(unsigned char x);
unsigned char doesclrscrafterexit(void);
void chlinexy_blt(unsigned char x, unsigned char y, unsigned char length);

unsigned char revers(unsigned char onoff);
unsigned int wherex(void);
unsigned int wherey(void);
void gotoxy(unsigned int x, unsigned int y);
void putch(char c);

void clrscr(void);

#endif

#endif // BWC_ADAM_CONIO_HELPERS_H