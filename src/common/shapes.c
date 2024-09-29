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
#include "resilience.h"
#include "shapes.h"

char *shapes_url = "/shapes";

void parse_shape_records(const uint8_t *input) {
	uint8_t i, j;
	uint8_t dataLength;
	uint8_t w;
	uint16_t current_offset = 0;
	const uint8_t *currentPos = input;

	memset(shapes_buffer, 0, SHAPES_BUFFER_SIZE);

	for (i = 0; i < shape_count; i++) {
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

void create_shape_url() {
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, shapes_url);
}

uint8_t get_shape_count() {
	int n = 0;
	uint8_t shapes_tmp[1];
	create_shape_url();
	strcat(url_buffer, "/count");

	try_open("shape count open", url_buffer, OPEN_MODE_HTTP_GET);
	n = network_read(url_buffer, shapes_tmp, 1);
	network_close(url_buffer);
	if (n < 0) {
		err = -n;
		handle_err("shape count read");
	}
	return shapes_tmp[0];
}

void read_and_parse_shapes_data() {
	int n;

	// use the app_data buffer as a scratch buffer, as it's only needed when initially parsing
	memset(app_data, 0, 512);

	create_shape_url();
	strcat(url_buffer, "/data");
	try_open("shapes open", url_buffer, OPEN_MODE_HTTP_GET);
	n = network_read(url_buffer, app_data, 512);
	network_close(url_buffer);
	if (n < 0) {
		err = -n;
		handle_err("shape data read");
	}

	parse_shape_records(app_data);
}

void display_shape_data(uint8_t n, uint8_t x, uint8_t y) {
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
	uint8_t i;
	uint8_t x;
	uint8_t y;
	char tmp[4];

	cputsxy(0, 0, "Beginning parse of shapes data...");

	shape_count = get_shape_count();
	memset(shapes, 0, 250); // room for 50 shapes
	// shapes = (ShapeRecord *)malloc(shape_count * sizeof(ShapeRecord));
	read_and_parse_shapes_data();

	gotoxy(0, 1);
	cputs("Parsed shapes, count: ");
	itoa(shape_count, tmp, 10);
	cputs(tmp);

	for (i = 0; i < shape_count; i++) {
		x = (i % 7) * 6;
		y = ((uint8_t) (i / 7)) * 6 + 3;
		display_shape_data(i, x, y);
	}

}