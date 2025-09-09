#include <coco.h>
#include <cmoc.h>

#include "chardef.h"
#include "conio_wrapper.h"

/* 
 * cc65 conio like, text console functions
 * on coco 
 *
 */

unsigned char cursor(unsigned char onoff) {
	return 0;
}

unsigned char revers(unsigned char onoff) {
	return 0;
}

void chlinexy(unsigned char x, unsigned char y, unsigned char length) {
	gotoxy(x, y);
	while(length--)
	{
		cputc(CH_HLINE);
	}
}

void cputcxy(unsigned char x, unsigned char y, char c)
{
    gotoxy(x, y);
    cputc(c);
}

void cputsxy(unsigned char x, unsigned char y, const char *s)
{
    gotoxy(x, y);
	cputs(s);
}

/* this is from cc65.h */

unsigned char doesclrscrafterexit(void) {
	return 1;
}

