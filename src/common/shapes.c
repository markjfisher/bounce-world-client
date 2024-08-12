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
#include "main.h"
#include "shapes.h"

char *shapes_url = "/shapes";

extern void itoa_byte(char *s, uint8_t v);

void parseShapeRecords(const char *input, uint8_t count, ShapeRecord *records) {
    uint8_t i;
	uint8_t dataLength;
	uint8_t w;
	const char *currentPos = input;
    char *end;

	// printf("data: %s\n", input);

    for (i = 0; i < count; i++) {
        // Initialize shape_data to NULL for each record
        records[i].shape_data = NULL;

        // Parse shape_id
        records[i].shape_id = (uint8_t)strtol(currentPos, &end, 10);
        if (currentPos == end) break; // No conversion happened
        currentPos = end + 1; // Move past the comma

        // Parse shape_width
        w = (uint8_t)strtol(currentPos, &end, 10);
        records[i].shape_width = w;
        if (currentPos == end) break; // No conversion happened
        currentPos = end + 1; // Move past the comma

        // Calculate the length of the string based on shape_width squared (lookup less time/memory than multiplication)
        dataLength = w * w;

        // Allocate memory for shape_data
        records[i].shape_data = (char *)malloc(dataLength + 1); // +1 for the null terminator
        if (records[i].shape_data == NULL) {
			err = 1;
			handle_err("malloc shapes");
        }

		strncpy(records[i].shape_data, currentPos, dataLength);
		records[i].shape_data[dataLength] = '\0'; // Ensure null-termination
		convert_chars(records[i].shape_data);
		records[i].shape_data_len = dataLength;
		currentPos = currentPos + dataLength + 1; // Prepare for the next record
    }
}

void createShapeURL() {
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, shapes_url);
}

uint8_t getShapeCount() {
	int n = 0;
	char shapes_tmp[3];
	createShapeURL();
	strcat(url_buffer, "/count");

	err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("shape count open");
	n = network_read(url_buffer, (uint8_t *)shapes_tmp, 2);
	if (n < 0) {
		err = -n;
		handle_err("shape count read");
	}
	return atoi(shapes_tmp);
}

void readAndParseShapesData(uint8_t shape_count) {
	char *buffer;
	int n;

	buffer = malloc(512); // shapes json fits in 392 bytes at the moment
	memset(buffer, 0, 512);

	createShapeURL();
	strcat(url_buffer, "/data");
	err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("shapes open");
	n = network_read(url_buffer, (uint8_t *) buffer, 512);
	if (n < 0) {
		err = -n;
		handle_err("shape data read");
	}

	parseShapeRecords(buffer, shape_count, shapes);
	free(buffer);
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
	c = &shape.shape_data[0];

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
void getShapes() {
	uint8_t shape_count;
	uint8_t i;
	uint8_t x;
	uint8_t y;
	char tmp[4];

	cputsxy(0, 0, "Beginning parse of shapes data...");

	shape_count = getShapeCount();
	shapes = (ShapeRecord *)malloc(shape_count * sizeof(ShapeRecord));
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