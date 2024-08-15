#include <stdbool.h>
#include <stdint.h>

#include "data.h"
#include "display.h"
#include "fujinet-network.h"

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

		// handle_kb();

		// do we need a pause here?

	}
}