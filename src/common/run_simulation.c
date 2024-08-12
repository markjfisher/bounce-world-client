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

		if (!is_connected) {
			err = network_open(endpoint, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
			handle_err("open data");
			is_connected = true;
		}

		n = network_read_nb(endpoint, location_data, 256);

		// gotoxy(0,22);
		// printf("%u\n", n);
		// pause(5);
		// debug();

		if (fn_network_error == 136) {
			is_connected = false;
			continue;
		}

		if (n > 0) {
			display_positions();
		}

		current_frame++;
		if (current_frame == 100) {
			send_heartbeat();
			current_frame = 0;
		}

		// handle_kb();

		// do we need a pause here?

	}
}