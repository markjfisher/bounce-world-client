#if !defined(_CMOC_VERSION_)
#if !defined(__ADAM__)
#include <conio.h>
#endif
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#include <conio_wrapper.h>
#endif

#include "fujinet-network.h"

#include "app_errors.h"
#include "connection.h"
#include "convert_chars.h"
#include "data.h"
#include "debug.h"
#include "delay.h"
#include "hex_dump.h"
#include "shapes.h"

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "text_buffer.h"
#include "shape_util.h"
#include "itoa_wrapper.h"
#include "double_buffer.h"
#elif defined __ADAM__
#include "conio_helpers.h"

#endif

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

uint8_t get_shape_count() {
	create_command("x-shape-count");
	send_command();
	read_response_wait(app_data, 1);

	// debug: show the shape count
	// gotoxy(0, 21);
	// hd(app_data, 1);

	return app_data[0];
}

void read_and_parse_shapes_data() {
	int n;

	// use the app_data buffer as a scratch buffer, as it's only needed when initially parsing
	memset(app_data, 0, APP_DATA_SIZE);
	create_command("x-shape-data");
	send_command();
	n = read_response_min(app_data, 1, APP_DATA_SIZE);
	// hd(app_data, APP_DATA_SIZE);

	if (n < 0) {
		err = -n;
		handle_err("shape data read");
	}

	parse_shape_records(app_data);
}

#ifdef __PMD85__
void display_shape_data(uint8_t n, uint8_t x, uint8_t y) {
	ShapeRecord shape;
	uint8_t w;
	char *data;

	shape = shapes[n];
	w = shape.shape_width; // h = w
	data = (char *) &shape.shape_data[0];
	gotoxy_tb(x, y);
	print_shape_tb(data, w, w, 0);
	add_dirty_rect(x, y, w, w);
}

#else
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
#endif

// fetch the shapes data and assign it to values the application can use to draw
void get_shapes() {
	uint8_t i;
	uint8_t x;
	uint8_t y;
	char tmp[6];

	cputsxy(0, 0, "Beginning parse of shapes data...");
	shape_count = get_shape_count();
	memset(shapes, 0, 250); // room for 50 shapes
	read_and_parse_shapes_data();

	gotoxy(0, 1);
	cputs("Parsed shapes, count: ");
	itoa(shape_count, tmp, 10);
	cputs(tmp);

#ifdef __PMD85__
	reset_dirty();
#endif

	for (i = 0; i < shape_count; i++) {
		x = (i % 7) * 6;
		y = ((uint8_t) (i / 7)) * 6 + 3;
		display_shape_data(i, x, y);
	}

#ifdef __PMD85__
	show_text_buffer();
	clear_dirty();
#endif

}
