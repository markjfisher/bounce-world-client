#include <conio.h>
// #include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "broadcast.h"
#include "data.h"
#include "debug.h"
#include "delay.h"
#include "display.h"
#include "double_buffer.h"
#include "hex_dump.h"
#include "keyboard.h"
#include "screen.h"
#include "shapes.h"
#include "target_clr.h"
#include "world.h"
#include "who.h"

#ifdef __ATARI__
#include <atari.h>
#include "dlist.h"
#include "vbi.h"
#include "fx.h"
#endif

#ifdef BWC_CUSTOM_CPUTC
void cputc_fast(char c);
void cincx();
void gotoxy_fast (uint8_t x, uint8_t y);
#endif

extern void debug();
// extern void hd(void* data, unsigned int size);

void init_screen() {
	clrscr();

#ifdef __ATARI__
	OS.color2 = 0;
	init_vbi();

	wait_vsync();
	setup_dli();
	wait_vsync();

	// turn off key clicking
	OS.noclik = 0xFF;
#endif
	if (is_darkmode) {
		do_darkmode();
	}

}

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

	shape = shapes[shape_id];
	width = shape.shape_width;
	data = (char *) shape.shape_data;

	start_x = center_x - (width >> 1) - 1;
	start_y = center_y - (width >> 1) - 1;

	// cater for even width shapes which are slightly off centre.
	// this is actually a "mod 2" calc, but that's the same as checking last bit set or not.
	if ((width & 1) == 0) {
		start_x++;
		start_y++;
	}

	for (i = 0; i < width; ++i) {
		y = start_y + i;
		first_x_in_row = false; // Reset for each row

		// Only proceed if the row is within the vertical screen bounds
		if (y >= 0 && y < SCREEN_HEIGHT) {
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
						gotoxy(x, y);
#endif

#ifdef __APPLE2__
						// handle double buffer on apple2, always done after a gotoxy.
						// this ensures we are writing to the correct location
						check_text_buffer_location();
#endif
					}
					c = data[iw + j];

#ifdef BWC_CUSTOM_CPUTC
					// handles space char as just moving x directly
					cputc_fast(c);
#else

#ifdef __APPLE2__
					// no optimization for the space on apple2
					cputc(c);
#else

					if (c != ' ') {
						cputc(c);  // Print and move to the next position
					} else {
						// don't print spaces, we already cleared the screen, just advance 1 char.
						// Means no "rubout" on slightly overlapping shapes
						gotox(wherex() + 1);
					}
#endif

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

	char tmp[10];
	uint8_t i, shape_id;
	int8_t x, y;
	// Number of bytes to skip before starting to read shapes data
	uint8_t index = 3;
	uint8_t number_of_shapes = app_data[2];

	// make all writes go to the other screen/memory
	swap_buffer();

	// displays world stats. allow for double buffering. To redisplay this, reset info_display_count to 0
	if (info_display_count < 2) {
		// do a full clear screen to clear the text area

#ifdef __APPLE2__
		target_clr();
#else
		clrscr();
#endif

		show_info();
		info_display_count++;
	} else {
		// just a partial clear, the text display is now setup correctly.
		target_clr();

#ifdef __APPLE2__
		// target_clr() clears whole screen on apple2, so need to redo the info part every frame. TODO: fix
		show_info();
#endif

	}

	for (i = 0; i < number_of_shapes; ++i) {
		shape_id = app_data[index++];
		x = (int8_t)app_data[index++];
		y = (int8_t)app_data[index++];

		if (shape_id < shape_count) {
			show_shape(shape_id, x, y);
		} else {
			// crash out
			show_other_screen();
			clrscr();
			cputsxy(0, 0, "error in app_data - bad shape id");
			cputsxy(0, 1, "shape id: ");
			itoa(shape_id, tmp, 10);
			cputsxy(10, 1, tmp);
			cputsxy(0, 2, "num shapes: ");
			itoa(number_of_shapes, tmp, 10);
			cputsxy(12, 2, tmp);

			gotoxy(0, 4);
			hd(app_data, 64);
			wait_vsync();
			debug();
			while(1) ;
		}
	}

	if (is_showing_clients) {
		show_clients();
	}

	if (is_showing_broadcast) {
		show_broadcast();
	}

	// show the other screen
	wait_vsync();
	show_other_screen();

}