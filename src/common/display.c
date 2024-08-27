#include <conio.h>
// #include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "data.h"
#include "debug.h"
#include "delay.h"
#include "display.h"
#include "double_buffer.h"
#include "screen.h"
#include "shapes.h"
#include "target_clr.h"

#ifdef __ATARI__
#include <atari.h>
#include "dlist.h"
#endif

extern void debug();

void init_screen() {

#ifdef __ATARI__
	debug();
	setup_dli();
#endif

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

	shape = shapes[shape_id];
	width = shape.shape_width;
	data = (char *) shape.shape_data;

	start_x = center_x - (width >> 1);
	start_y = center_y - (width >> 1);

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
						gotoxy(x, y);
#ifdef __APPLE2__
						// handle double buffer on apple2, always done after a gotoxy.
						// this ensures we are writing to the correct location
						check_text_buffer_location();
#endif
					}

					cputc(data[iw + j]);  // Print and move to the next position
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
	// byte 1: status byte
	// byte 2: number of shapes to display for client, 3 bytes per shape follow
	// byte 3-5: {shape_id, x, y}
	// byte 6-8: ...

	uint8_t i, shape_id;
	int8_t x, y;
	// Number of bytes to skip before starting to read shapes data
	uint8_t index = 3;
	uint8_t number_of_shapes = app_data[2];

	// make all writes go to the other screen/memory
	swap_buffer();
	target_clr();
	show_info();

	for (i = 0; i < number_of_shapes; ++i) {
		shape_id = app_data[index++];
		x = (int8_t)app_data[index++];
		y = (int8_t)app_data[index++];

		show_shape(shape_id, x, y);
	}

	// show the other screen
	// wait_vsync();
	show_other_screen();

}