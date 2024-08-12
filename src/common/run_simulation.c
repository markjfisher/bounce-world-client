#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "connection.h"
#include "data.h"
#include "display.h"
#include "heartbeat.h"
#include "fujinet-network.h"
#include "get_line.h"

extern void debug();
extern void wait_vsync();
extern void pause(uint8_t t);

void run_simulation() {
	int n = 0;
	uint8_t current_frame = 0;

	// the core loop for grabbing data from FN and then displaying it

	while(is_running_sim) {
		network_open(endpoint, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
		n = network_read(endpoint, location_data, 256);
		network_close(endpoint);

		if (n > 0) {
			display_positions();
		}

		current_frame++;
		if (current_frame == 40) {
			send_heartbeat();
			current_frame = 0;
		}

		// handle_kb();

		// do we need a pause here?

	}
}