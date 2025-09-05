#include <coco.h>
#include <cmoc.h>

#include "chardef.h"
#include "conio_wrapper.h"

#define DISPLAY			0xC000

#define BYTES_PER_LINE		32
#define BYTES_PER_LINE_VISIBLE	32
#define LINES_PER_CHAR		6
#define LINES_PER_CHAR_ROW	8

#define BLITTER_WIDTH	40
#define BLITTER_HEIGHT	24
#define BLITTER_X0		4
#define BLITTER_Y0		4

static char blitter_x = 0;
static char blitter_y = 0;

/* 
 * cc65 conio like, text console functions
 * on coco 
 *
 */

unsigned char wherex(void) {
	return blitter_x;
}

unsigned char wherey(void) {
	return blitter_y;
}

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

