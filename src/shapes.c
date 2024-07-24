#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fujinet-network.h"

#include "buffer.h"
#include "errors.h"
#include "shapes.h"
#include "convertChars.h"

ShapeRecord *shapes;
char *shapes_url = "/shapes";

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
		convertChars(records[i].shape_data);
		records[i].shape_data_len = dataLength;
		currentPos = currentPos + dataLength + 1; // Prepare for the next record
    }
}

void createShapeURL(char *endpoint) {
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, shapes_url);
}

uint8_t getShapeCount(char *endpoint) {
	int n = 0;
	char shapes_tmp[3];
	createShapeURL(endpoint);
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

void readAndParseShapesData(char *endpoint, uint8_t shape_count) {
	char *buffer;
	int n;

	buffer = malloc(512); // shapes json fits in 392 bytes at the moment
	memset(buffer, 0, 512);

	createShapeURL(endpoint);
	strcat(url_buffer, "/data");
	err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("shapes open");
	n = network_read(url_buffer, buffer, 512);
	if (n < 0) {
		err = -n;
		handle_err("shape data read");
	}

	parseShapeRecords(buffer, shape_count, shapes);
	free(buffer);
}

void displayShapeData(uint8_t n) {
	uint8_t i;
	uint8_t j;
	uint8_t dataLength;
	uint8_t width;
	ShapeRecord shape;
    
	shape = shapes[n];
    width = shape.shape_width;
    dataLength = shape.shape_data_len;

    for (i = 0; i < dataLength; i += width) {
        for (j = 0; j < width; ++j) {
            if (i + j < dataLength) {
                printf("%c", shape.shape_data[i + j]);
            }
        }
        printf("\n"); // New line after printing 'width' number of characters
    }
}

// fetch the shapes data and assign it to values the application can use to draw
void getShapes(char *endpoint) {
	uint8_t shape_count;
	uint8_t i;

	printf("beginning parse of shapes data\n");

	shape_count = getShapeCount(endpoint);
	shapes = (ShapeRecord *)malloc(shape_count * sizeof(ShapeRecord));
	readAndParseShapesData(endpoint, shape_count);

	printf("parsed shapes, count: %d\n", shape_count);
	for (i = 0; i < shape_count; i++) {
		displayShapeData(i);
		cgetc();
	}

    // for (i = 0; i < shape_count; i++) {
    //     printf("Record %d - ID: %u, Width: %u, Data: %s\n", i, shapes[i].shape_id, shapes[i].shape_width, shapes[i].shape_data);
    // }

}