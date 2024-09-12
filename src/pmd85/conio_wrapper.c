#ifdef __PMD85__

#include <stdint.h>

#include "chardef.h"
#include "screen_util.h"
#include "conio_wrapper.h"

#include <conio.h>

#define DISPLAY			0xC000

#define BYTES_PER_LINE			64
#define BYTES_PER_LINE_VISIBLE	48
#define LINES_PER_CHAR			6
#define LINES_PER_CHAR_ROW		8

#define BLITTER_WIDTH	40
#define BLITTER_HEIGHT	24
#define BLITTER_X0		4
#define BLITTER_Y0		4

static char blitter_x = 0;
static char blitter_y = 0;

/* 
 * cc65 conio like, text console functions
 * using software blitter on pmd85
 *
 * TODO: asm versions
 *
 */

void clrscr_blt(void)
{
	screen_set_region(BLITTER_X0, BLITTER_Y0, BLITTER_WIDTH, BLITTER_HEIGHT);
	screen_fill_region(PATTERN_BLANK);
	blitter_x = blitter_y = 0;
	video_ptr = (unsigned char *) DISPLAY + BLITTER_X0 + BLITTER_Y0 * LINES_PER_CHAR_ROW * BYTES_PER_LINE;
}

static void _update_video_ptr(void) {
	video_ptr = (unsigned char *) DISPLAY + (BLITTER_X0 + blitter_x) + (BLITTER_Y0 + blitter_y) * LINES_PER_CHAR_ROW * BYTES_PER_LINE;
}

void gotoxy_blt(unsigned char x, unsigned char y) {
	blitter_x = x;
	blitter_y = y;
	_update_video_ptr();
}

void gotox_blt(unsigned char x) {
	blitter_x = x;
	_update_video_ptr();
}

void gotoy_blt(unsigned char y) {
	blitter_y = y;
	_update_video_ptr();
}

unsigned char wherex_blt(void) {
	return blitter_x;
}

unsigned char wherey_blt(void) {
	return blitter_y;
}

unsigned char cursor_blt(unsigned char onoff) {
	return 0;
}

unsigned char revers_blt(unsigned char onoff) {
	return 0;
}

void cputc_blt(char c) {
	if (blitter_x >= BLITTER_WIDTH) gotox_blt(0);  // wrap the line
	if (blitter_y >= BLITTER_HEIGHT) gotoy_blt(0); // wrap the screen
	(c == ' ') ? clear_char() : blit_char_upper(c);
	video_ptr++;
	blitter_x++;
}

void cputs_blt(const char* s)
{
	char c;
	while((c = *s++))
		cputc_blt(c);
}

void chlinexy_blt(unsigned char x, unsigned char y, unsigned char length) {
	gotoxy_blt(x, y);
	while(length--)
		cputc_blt(CH_HLINE);
}

// void cvlinexy_blt(unsigned char x, unsigned char y, unsigned char height) {
// 	gotoxy_blt(x, y);
// 	while(height--) {
// 		cputc_blt(CH_VLINE);
// 		gotoy_blt(blitter_y+1);
// 	}
// }

void cputcxy_blt(unsigned char x, unsigned char y, char c)
{
    gotoxy_blt(x, y);
	cputc_blt(c);
}

void cputsxy_blt(unsigned char x, unsigned char y, const char *s)
{
    gotoxy_blt(x, y);
	cputs_blt(s);
}

/* this is from cc65.h */

unsigned char doesclrscrafterexit(void) {
	return 1;
}

// /* (double) buffer drawing functions */

// void gotoxy_buffer(uint8_t x, uint8_t y) {
// 	gotoxy_blt(x, y);
// 	//check_text_buffer_location();
// }

// void cputsxy_buffer(uint8_t x, uint8_t y, char *s) {
// 	gotoxy_buffer(x, y);
// 	cputs_blt(s);
// }

// void cputcxy_buffer(uint8_t x, uint8_t y, char c) {
// 	gotoxy_buffer(x, y);
// 	cputc_blt(c);
// }

#endif
