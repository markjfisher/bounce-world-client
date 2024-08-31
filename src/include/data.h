#ifndef BWC_DATA_H
#define BWC_DATA_H

#include <stdbool.h>
#include <stdint.h>
#include "shapes.h"

// WARNING: if these change, need to change data.s too where the data is reserved!
#define SHAPES_BUFFER_SIZE 512
#define APP_DATA_SIZE      512

// scratch buffer when creating full url from concat of endpoint and service being called
extern char    url_buffer[128];

// scratch buffer for general network data (get/post etc)
extern uint8_t app_data[APP_DATA_SIZE];

// This memory is carved up for the shapes data strings so we don't need to malloc them, and instead used slices of this memory
extern uint8_t shapes_buffer[SHAPES_BUFFER_SIZE];
extern char    name[9];

// this is the captured URL of the BW server without any controller endpoints added
extern char    endpoint[80];

// this is the endpoint for fetching world data for this client, effectively endpoint with extra controller strings
extern char    client_data_url[96];

extern char    client_id[1];

// each record is 5 bytes, so this requires 250 bytes
extern ShapeRecord shapes[50];
extern uint8_t shape_count;

extern bool is_running_sim;
extern uint8_t current_step;
extern uint8_t app_status;

extern uint8_t info_display_count;


#endif // BWC_DATA_H