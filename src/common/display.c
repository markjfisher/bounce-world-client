#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "data.h"
#include "screen.h"
#include "shapes.h"

extern void itoa_byte(char *s, uint8_t v);
extern void debug();

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

	startX = centerX - (width % 2 == 0 ? (width / 2 - 1) : (width / 2));
	startY = centerY - (width % 2 == 0 ? (width / 2 - 1) : (width / 2));

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

	clrscr();

	for (i = 0; i < numberOfShapes; ++i) {
		shapeId = location_data[index++];
		x = (int8_t)location_data[index++];  // Cast to signed int8_t
		y = (int8_t)location_data[index++];  // Cast to signed int8_t

		show_shape(shapeId, x, y);
	}

}