#ifndef BWC_CONIO_WRAPPER_H
#define BWC_CONIO_WRAPPER_H

#ifdef __PMD85__

#include <stdint.h>
#include <conio.h>

#include "chardef.h"


#undef clrscr
#undef gotoxy
#undef gotox
#undef gotoy
#undef wherex
#undef wherey
#undef revers
#undef cursor
#undef cputc
#undef cputs
#undef chlinexy
#undef cputsxy

#define clrscr() clrscr_blt()

#define gotoxy(x, y) gotoxy_blt(x, y)
#define gotox(x) gotox_blt(x)
#define gotoy(y) gotoy_blt(y)
#define wherex() wherex_blt()
#define wherey() wherey_blt()
 
#define revers(onoff) revers_blt(onoff)
#define cursor(onoff) cursor_blt(onoff)
 
#define cputc(c) cputc_blt(c)
#define cputs(s) cputs_blt(s)

#define cputcxy(x, y, c) cputcxy_blt(x, y, c)
#define cputsxy(x, y, s) cputsxy_blt(x, y, s)

#define chlinexy(x, y, length) chlinexy_blt(x, y, length)


void clrscr_blt(void);

void gotoxy_blt(unsigned char x, unsigned char y);
void gotox_blt(unsigned char x);
void gotoy_blt(unsigned char y);
unsigned char wherex_blt(void);
unsigned char wherey_blt(void);

unsigned char revers_blt(unsigned char onoff);
unsigned char cursor_blt(unsigned char onoff);

void cputc_blt(char c);
void cputs_blt(const char* s);

void cputcxy_blt(unsigned char x, unsigned char y, char c);
void cputsxy_blt(unsigned char x, unsigned char y, const char* s);

void chlinexy_blt(unsigned char x, unsigned char y, unsigned char length);
// void cvlinexy_blt(unsigned char x, unsigned char y, unsigned char height);

unsigned char doesclrscrafterexit (void);

#endif

#endif // BWC_CONIO_HELPERS_H