#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "data.h"
#include "debug.h"
#include "screen.h"
#include "shapes.h"

#ifdef __ATARI__
#include <atari.h>
#include "fast_clr.h"
#include "get_dlist_screen_ptr.h"

#define SAVMSC_P ((uint8_t*) 0x0058)

char d1[960];
char *dlist_scr_ptr;
char *screen_mem_orig;
uint8_t is_orig_screen_mem;

extern void swap_buffer();
extern void set_dlist();

#endif

extern void itoa_byte(char *s, uint8_t v);
extern void debug();
extern void wait_vsync();

void show_shape(uint8_t shapeId, int8_t centerX, int8_t centerY) {
	uint8_t i, j;
	int8_t x, y;
	int8_t startX, startY;
	ShapeRecord shape;
	uint8_t width;
	char *data;
	bool firstXInRow;

	shape = shapes[shapeId];
	width = shape.shape_width;
	data = (char *) shape.shape_data;

	startX = centerX - width / 2;
	startY = centerY - width / 2;

	// cater for even width shapes, mod 2 is same as testing last bit
	if ((width & 1) == 0) {
		startX++;
		startY++;
	}

	for (i = 0; i < width; ++i) {
		y = startY + i;
		firstXInRow = false; // Reset for each row

		// Only proceed if the row is within the vertical screen bounds
		if (y >= 0 && y < SCREEN_HEIGHT) {
			for (j = 0; j < width; ++j) {
				x = startX + j;

				// Check if the current character is within the horizontal screen bounds
				if (x >= 0 && x < SCREEN_WIDTH) {
					if (!firstXInRow) {
						firstXInRow = true;  // Found the first x position in this row within bounds
						gotoxy(x, y);
					}
					cputc(data[i * width + j]);  // Print and move to the next position
				} else if (firstXInRow) {
					// If we were printing characters and have now gone out of bounds, break
					break;
				}
			}
		}
	}
}

void display_positions() {
	// positions to display are in location_data
	// byte 0: world step number (0-255, looping)
	// byte 1: number of shapes to display for client, 3 bytes per shape follow
	// byte 2-4: {shapeId, x, y}
	// byte 5-7: ...

	uint8_t i, shapeId;
	int8_t x, y;
	uint8_t stepNumber = location_data[0]; // TODO: show this somewhere
	uint8_t numberOfShapes = location_data[1];
	uint8_t index = 2;  // Start reading shapes data after the first two bytes

	// wait_vsync();
#ifdef __ATARI__
	swap_buffer();
	fast_clr(); // slightly faster on atari, only 40x20, and no set cursor, so 160 fewer iterations
#else
	clrscr();
#endif

	for (i = 0; i < numberOfShapes; ++i) {
		shapeId = location_data[index++];
		x = (int8_t)location_data[index++];  // Cast to signed int8_t
		y = (int8_t)location_data[index++];  // Cast to signed int8_t

		show_shape(shapeId, x, y);
	}

#ifdef __ATARI__
	wait_vsync();
	set_dlist();
#endif

}