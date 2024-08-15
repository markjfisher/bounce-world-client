#ifndef SHAPES_H
#define SHAPES_H

#include <stdint.h>

void getShapes();

typedef struct {
    uint8_t shape_id;
    uint8_t shape_width;
	uint8_t shape_data_len;
    uint8_t *shape_data;
} ShapeRecord;

extern ShapeRecord shapes[50];

#endif // SHAPES_H