#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "data.h"
#include "debug.h"
#include "fujinet-network.h"
#include "world.h"

char *world_state = "/ws";

void get_world_state() {
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, world_state);

    err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("ws_get:open");

	// read directly into the 14 bytes of memory starting at world_width
	network_read(url_buffer, (uint8_t *) &world_width, 14);
	network_close(url_buffer);
}