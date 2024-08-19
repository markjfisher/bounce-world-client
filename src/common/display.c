#include <conio.h>
// #include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "data.h"
#include "debug.h"
#include "double_buffer.h"
#include "screen.h"
#include "shapes.h"
#include "target_clr.h"

#ifdef __ATARI__
#include <atari.h>
#include "get_dlist_screen_ptr.h"
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
#ifdef __APPLE2__
// handle double buffer on apple2, always done after a gotoxy. this ensures we are writing to the correct location
						check_text_buffer_location();
#endif
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

	// make all writes go to the other screen/memory
	// wait_vsync();
	swap_buffer();
	// clear other screen for double buffering
	target_clr();

	// print shapes in the other screen
	for (i = 0; i < numberOfShapes; ++i) {
		shapeId = location_data[index++];
		x = (int8_t)location_data[index++];  // Cast to signed int8_t
		y = (int8_t)location_data[index++];  // Cast to signed int8_t

		show_shape(shapeId, x, y);
	}

	// show the other screen
	// wait_vsync();
	show_other_screen();

}