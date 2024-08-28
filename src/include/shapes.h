#ifndef BWC_SHAPES_H
#define BWC_SHAPES_H

#include <stdint.h>

void get_shapes();

typedef struct {
    uint8_t shape_id;
    uint8_t shape_width;
	uint8_t shape_data_len;
    uint8_t *shape_data;
} ShapeRecord;

typedef struct {
    uint8_t shapeId;
    int8_t x;
    int8_t y;
    uint8_t width;
    // bool active;  // Indicates if the shape is active in the current frame
} ShapeLocation;

extern ShapeRecord shapes[50];
// extern ShapeLocation previous_locs[50];

#endif // BWC_SHAPES_H