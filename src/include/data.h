#ifndef DATA_H
#define DATA_H

#include <stdbool.h>
#include <stdint.h>
#include "shapes.h"

extern char url_buffer[128];
extern uint8_t location_data[256];

extern char *endpoint;
extern char client_id[9];
extern char *heartbeat;
extern char *name;
extern ShapeRecord *shapes;
extern bool is_running_sim;

extern bool is_connected;

#endif // DATA_H