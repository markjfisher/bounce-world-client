#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fujinet-network.h"

#include "app_errors.h"
#include "convert_chars.h"
#include "data.h"
#include "debug.h"
#include "shapes.h"

char *shapes_url = "/shapes";

extern void itoa_byte(char *s, uint8_t v);

void parseShapeRecords(const uint8_t *input, uint8_t count) {
	uint8_t i, j;
	uint8_t dataLength;
	uint8_t w;
	uint16_t current_offset = 0;
	const uint8_t *currentPos = input;

	memset(shapes_buffer, 0, SHAPES_BUFFER_SIZE);

	for (i = 0; i < count; i++) {
		shapes[i].shape_id = *currentPos++;

		w = *currentPos++;
		shapes[i].shape_width = w;

		// allocate space in shapes_buffer to each shape_data
		dataLength = w * w;
		if (current_offset + dataLength <= SHAPES_BUFFER_SIZE) {
            // Point shape_data to the current position in the buffer
            shapes[i].shape_data = &shapes_buffer[current_offset];

            // Copy the data into the shapes_buffer
            for (j = 0; j < dataLength; j++) {
                shapes_buffer[current_offset + j] = *currentPos++;
            }

            current_offset += dataLength;

            convert_chars(shapes[i].shape_data, dataLength);
            shapes[i].shape_data_len = dataLength;
        } else {
            // Handle the error for insufficient buffer space
            err = 1;
            handle_err("Insufficient buffer space");
            break;
        }

	}
}

void createShapeURL() {
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, shapes_url);
}

uint8_t getShapeCount() {
	int n = 0;
	uint8_t shapes_tmp[1];
	createShapeURL();
	strcat(url_buffer, "/count");

	err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("shape count open");
	n = network_read(url_buffer, shapes_tmp, 1);
	network_close(url_buffer);
	if (n < 0) {
		err = -n;
		handle_err("shape count read");
	}
	return shapes_tmp[0];
}

void readAndParseShapesData(uint8_t shape_count) {
	int n;

	// use the app_data buffer as a scratch buffer, as it's only needed when initially parsing
	memset(app_data, 0, 512);

	createShapeURL();
	strcat(url_buffer, "/data");
	err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("shapes open");
	n = network_read(url_buffer, app_data, 512);
	network_close(url_buffer);
	if (n < 0) {
		err = -n;
		handle_err("shape data read");
	}

	parseShapeRecords(app_data, shape_count);
}

void displayShapeData(uint8_t n, uint8_t x, uint8_t y) {
	uint8_t i;
	uint8_t j;
	uint8_t dataLength;
	uint8_t width;
	ShapeRecord shape;
	char *c;
	uint8_t actX;
	uint8_t actY;

	shape = shapes[n];
	width = shape.shape_width;
	dataLength = shape.shape_data_len;
	c = (char *) &shape.shape_data[0];

	for (i = 0; i < dataLength; i += width) {
		for (j = 0; j < width; ++j) {
			if (i + j < dataLength) {
				actX = x + j;
				actY = y + (i / width);
				cputcxy(actX, actY, *c);
				c++;
			}
		}
	}
}

// fetch the shapes data and assign it to values the application can use to draw
void get_shapes() {
	uint8_t shape_count;
	uint8_t i;
	uint8_t x;
	uint8_t y;
	char tmp[4];

	cputsxy(0, 0, "Beginning parse of shapes data...");

	shape_count = getShapeCount();
	memset(shapes, 0, 250); // room for 50 shapes
	// shapes = (ShapeRecord *)malloc(shape_count * sizeof(ShapeRecord));
	readAndParseShapesData(shape_count);

	gotoxy(0, 1);
	cputs("Parsed shapes, count: ");
	itoa_byte(tmp, shape_count);
	cputs(tmp);

	for (i = 0; i < shape_count; i++) {
		x = (i % 7) * 6;
		y = ((uint8_t) (i / 7)) * 6 + 3;
		displayShapeData(i, x, y);
	}

}