#if !defined (_CMOC_VERSION_)
#if !defined (__ADAM__)
#include <conio.h>
#endif
// #include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#include "conio_wrapper.h"
#endif
#include "broadcast.h"
#include "data.h"
#include "debug.h"
#include "delay.h"
#include "display.h"
#include "double_buffer.h"
#include "full_clr.h"
#include "hex_dump.h"
#include "keyboard.h"
#include "screen.h"
#include "shapes.h"
#include "playfield_clr.h"
#include "world.h"
#include "who.h"

#ifdef __ADAM__
#include <video/tms99x8.h>
#endif

#ifdef __ATARI__
#include <atari.h>
#include "dlist.h"
#include "vbi.h"
#include "fx.h"
#endif

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "screen_util.h"
#include "text_buffer.h"
#include "shape_util.h"

#include "itoa_wrapper.h"
#include "timer.h"
static unsigned int t1;
static unsigned int t2;
static char tmp[4];
// #elif defined __ADAM__
// #include "conio_wrapper.h"

#endif

#ifdef BWC_CUSTOM_CPUTC
void cputc_fast(char c);
void cincx();
void gotoxy_fast (uint8_t x, uint8_t y);
#endif

extern void debug();
// extern void hd(void* data, unsigned int size);

void set_screen_colours() {

#ifdef __ATARI__
	if (is_darkmode || !is_showing_info) {
		txt_c1 = 0;
		txt_c2 = 0;
		txt_c3 = 0;
	} else {
		txt_c1 = INIT_COLOUR_1;
		txt_c2 = INIT_COLOUR_2;
		txt_c3 = INIT_COLOUR_3;
	}
#endif

#ifdef __PMD85__
	draw_border();
#endif
}

void init_screen() {
	clrscr();

#ifdef __APPLE2__
	// clear the 2nd text area as we don't fully clear it on every screen swap to try and reduce blink in the info bar
	memset((void *) 0x0800, 0xA0, 0x0400);
#endif

#ifdef __ATARI__
	OS.color2 = 0;
	set_screen_colours();
	init_vbi();

	wait_vsync();
	setup_dli();
	wait_vsync();

	// turn off key clicking
	OS.noclik = 0xFF;

	// setup double buffering for atari
	dlist_scr_ptr = get_dlist_screen_ptr();
	screen_mem_orig = (uint8_t *) ((uint16_t)(dlist_scr_ptr[0]) | ((uint16_t)(dlist_scr_ptr[1]) << 8));
#endif

#ifdef __PMD85__
	set_screen_colours();
#endif
}

#ifdef __PMD85__
void draw_border() {
	long pat;

	// print border using top|bottom hline and left|right vline chars
	if (!is_darkmode) {
		// do some low level character drawing
		char i;
		// horizontal "lines"
		video_ptr = (unsigned char *)(0xC000 + 64 * (5 * 8 -2) + 4);
		for(i = 40; i--;) {
			blit_char(14);
			video_ptr++;
		}
		video_ptr = (unsigned char *)(0xC000 + 64 * (5 * 8 + 26 * 6 -2) + 4);
		for(i = 40; i--;) {
			blit_char(13);
			video_ptr++;
		}
		// vertical "lines"
		video_ptr = (unsigned char *)(0xC000 + 64 * (5 * 8 + 1 * 6 -2) + 3);
		for(i = 25; i--;) {
			blit_char(2);
			video_ptr += 6*64;
		}
		video_ptr = (unsigned char *)(0xC000 + 64 * (5 * 8 + 1 * 6 -2) + 44);
		for(i = 25; i--;) {
			blit_char(22);
			video_ptr += 6*64;
		}
		pat = MASK_KEEP_BITMAP | ACE_RED;
	}
	else {
		pat = PATTERN_BLANK;
	}

	// apply color (MASK_KEEP_BITMAP | ACE_...) or erase previous border (PATTERN_BLANK)
	screen_set_region(3, 5, 42, 1);
	screen_fill_region(pat);
	screen_set_region(3, 24, 42, 1);
	screen_fill_region(pat);

	screen_set_region(3, 6, 1, 18);
	screen_fill_region(pat);
	screen_set_region(44, 6, 1, 18);
	screen_fill_region(pat);
}
#endif // __PMD85__

void show_shape(uint8_t shape_id, int8_t center_x, int8_t center_y) {
	uint8_t i, j;
	int8_t x, y;
	int8_t start_x, start_y;
	ShapeRecord shape;
	uint8_t width;
	uint8_t iw = 0;
	char *data;
	bool first_x_in_row;
	char c;
	uint8_t max_y = SCREEN_HEIGHT;

	if (is_showing_info) {
		max_y = SCREEN_HEIGHT - 2;
	}

	shape = shapes[shape_id];
	width = shape.shape_width;
	data = (char *) shape.shape_data;

	// find location of top left corner of the box (square) this shape needs to display
	start_x = center_x - (width >> 1) - 1;
	start_y = center_y - (width >> 1) - 1;

	// cater for even width shapes which are slightly off centre.
	// this is actually a "mod 2" calc, but that's the same as checking last bit set or not.
	if ((width & 1) == 0) {
		start_x++;
		start_y++;
	}

	// print each char in the box for this shape
	for (i = 0; i < width; ++i) {
		y = start_y + i;
		first_x_in_row = false; // Reset for each row

		// Only proceed if the row is within the vertical screen bounds
		if (y >= 0 && y < max_y) {
			for (j = 0; j < width; ++j) {
				x = start_x + j;

				// Check if the current character is within the horizontal screen bounds
				if (x >= 0 && x < SCREEN_WIDTH) {
					// only jump to location if it's the first character on the row and it needs to be displayed
					if (!first_x_in_row) {
						first_x_in_row = true;
#ifdef BWC_CUSTOM_CPUTC
						gotoxy_fast(x, y);
#else
						GOTOXY(x, y);
#endif
					}
					c = data[iw + j];

#ifdef BWC_CUSTOM_CPUTC
					// handles space char as just moving x directly
					cputc_fast(c);
#elif defined(_CMOC_VERSION_)
					cputc(c);  // Print and move to the next position
#else
					if (c != ' ') {
						cputc(c);  // Print and move to the next position
					} else {
						// don't print spaces, we already cleared the screen, just advance 1 char.
						// Means no "rubout" on slightly overlapping shapes, which looks is better
						gotox(wherex() + 1);
					}
#endif // BWC_CUSTOM_CPUTC

				} else if (first_x_in_row) {
					// If we were printing characters and have now gone out of bounds, break
					break;
				}
			}
		}
		// iw = i * width, but as i starts at 0, we want to accumulate at the end. this saves a multiplication.
		iw += width;
	}
}

void show_screen() {
	// positions to display are in app_data
	// byte 0: world step number (0-255, looping)
	// byte 1: status byte (used elsewhere)
	// byte 2: number of shapes to display for client, 3 bytes per shape follow
	// byte 3-5: {shape_id, x, y}
	// byte 6-8: ...

#ifdef __PMD85__
	char tmp[10];
#endif
	uint8_t i, shape_id;
	int8_t x, y;
	// Number of bytes to skip before starting to read shapes data, and then indexes into data for shapes
	// must be 16 bit as we can have 240 shapes in buffer, which is up to 722 bytes to index
	uint16_t index = 3;
	uint8_t number_of_shapes = app_data[2];

	// make all writes go to the other screen/memory
	swap_buffer();

#ifdef __PMD85__
	if (info_display_count++ == 0) { // one shot
		if (is_showing_info) {
			show_info();
		} else {
			clear_info();
		}
	}
#else
	// only do a full clear when we want to show changes in the info bar, which is at start, or if any state changes from server
	if (info_display_count < 2) {
		full_clr();
		if (is_showing_info) {
			show_info();
		}
		// this increments to 2 so that we can clear/print info in both buffers
		info_display_count++;
	} else {
#ifdef _CMOC_VERSION_
		if (is_showing_info)
		{
			// just a partial clear, the info display is now setup correctly.
			playfield_clr();
		}
		else
		{
			full_clr();
		}
#else 
		// just a partial clear, the info display is now setup correctly.
		playfield_clr();
#endif // _CMOC_VERSION
	}
#endif // __PMD85__

	// if (!is_showing_broadcast) {
		for (i = 0; i < number_of_shapes; ++i) {
			shape_id = app_data[index++];
			x = (int8_t)app_data[index++];
			y = (int8_t)app_data[index++];
#ifdef __PMD85__
			// t1 = read_timer(1);
			shape_to_buffer(shape_id, x, y);
			// t2 = read_timer(1);
			// uint8_to_a16((t1-t2)>>8, tmp);
			// cputsxy(i*3, 0, tmp);
#else
			show_shape(shape_id, x, y);
#endif
		}
	// }

	if (is_showing_clients) {
		show_clients();
	}

	if (is_showing_broadcast) {
		broadcast();
	}

	// show the other screen
	show_other_screen();

}
