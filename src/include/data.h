#ifndef DATA_H
#define DATA_H

#include <stdbool.h>
#include <stdint.h>
#include "shapes.h"

// if this changes, need to change data.s too
#define SHAPES_BUFFER_SIZE 512

extern char    url_buffer[128];
extern uint8_t location_data[512];
extern uint8_t shapes_buffer[SHAPES_BUFFER_SIZE];
extern char    post_data[64];
extern char    name[64];
extern char    endpoint[64];

extern uint16_t current_offset;

extern char client_id[9];
extern ShapeRecord shapes[50];
extern bool is_running_sim;

#endif // DATA_H