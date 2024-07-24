#ifndef SHAPES_H
#define SHAPES_H

#include <stdint.h>

void getShapes();

typedef struct {
    uint8_t shape_id;
    uint8_t shape_width;
	uint8_t shape_data_len;
    char *shape_data;
} ShapeRecord;

extern ShapeRecord *shapes;

#endif // SHAPES_H