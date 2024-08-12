#include <stdbool.h>
#include <stdint.h>

#include "shapes.h"

char url_buffer[128];
uint8_t location_data[256];

char *endpoint;
char client_id[9]; // 8 chars + 1 for nul terminator
char *heartbeat;
char *name;
ShapeRecord *shapes;
bool is_running_sim = true;
bool is_connected = false;
