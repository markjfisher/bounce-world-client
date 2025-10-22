#ifndef COCO_CONIO_WRAPPER_H
#define COCO_CONIO_WRAPPER_H

#include <cmoc.h>
#include <coco.h>
#include <hirestxt.h>
#include "chardef.h"
#include "stdbool.h"

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
#undef isprint
#undef kbhit
#undef cgetc

unsigned char wherex(void);
unsigned char wherey(void);

#define gotoxy(x, y) moveCursor(x, y)
#define gotox(x) moveCursor(x, wherey())
#define gotoy(y) moveCursor(wherex(), y)
 
#define cputc(c) putchar(c)
#define cputs(s) putstr(s, strlen(s))

#define isprint(c) (c>=0x20 && c<=0x8E)

#define kbhit() inkey()

byte cgetc(void);

byte cgetc_cursor(void);

unsigned char revers(unsigned char onoff);
unsigned char cursor(unsigned char onoff);

void cputcxy(unsigned char x, unsigned char y, char c);
void cputsxy(unsigned char x, unsigned char y, const char* s);

void chlinexy(unsigned char x, unsigned char y, unsigned char length);

unsigned char doesclrscrafterexit (void);

void hirestxt_init(void);

void switch_colorset(void);

#endif // COCO_CONIO_WRAPPER_H
