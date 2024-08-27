#ifndef BWC_DATA_H
#define BWC_DATA_H

#include <stdbool.h>
#include <stdint.h>
#include "shapes.h"

// WARNING: if these change, need to change data.s too where the data is reserved!
#define SHAPES_BUFFER_SIZE 512
#define APP_DATA_SIZE      512

extern char    url_buffer[128];
extern uint8_t app_data[APP_DATA_SIZE];
extern uint8_t shapes_buffer[SHAPES_BUFFER_SIZE];
extern char    post_data[64];
extern char    name[9];
extern char    endpoint[64];

extern char client_id[1];
extern ShapeRecord shapes[50];
extern bool is_running_sim;
extern uint8_t current_step;
extern uint8_t app_status;

#endif // BWC_DATA_H