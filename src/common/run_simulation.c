#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "data.h"
#include "display.h"
#include "delay.h"
#include "double_buffer.h"
#include "fujinet-network.h"
#include "keyboard.h"
#include "resilience.h"
#include "sound.h"
#include "status.h"
#include "world.h"

#ifdef __ATARI__
#include "dlist.h"
extern bool is_playing_collision;
#endif

#ifdef __PMD85__
#include "conio_wrapper.h"
#endif

void run_simulation() {
	int n;
	uint16_t error_delay = 30;
	uint8_t new_step_id;
	init_screen();

	// flag we are on initial screen for double buffering across the platforms
	is_alt_screen = 0;

	get_world_clients();

	while(is_running_sim) {
		// I originally used a continuous open channel that received data from server as it was pushed, but it
		// became clear it's easier to pull from client, so it didn't fall behind, and fetched current when it can.
		// I thought the push model would provide more consistency across views for the client, but it turns out
		// there's negligible delay between platforms constantly polling and the screens being updated, so pull model is better.

		memset(app_data, 0, APP_DATA_SIZE);
		try_open("loop:open", client_data_url, OPEN_MODE_HTTP_GET);

		n = network_read(client_data_url, app_data, APP_DATA_SIZE);
		network_close(client_data_url);

		if (n < 0) {
			// there was an error, so don't process this round. try again after a small pause
			// TODO: add some resillience here, backoff exponentially for max number of attempts.
			pause(error_delay);
			error_delay = error_delay * 14 / 10; // roughly sqrt(2) times previous delay if keep getting errors
			if (error_delay > 255) error_delay = 255;
			continue;
		}

		// reset error delay when we have a good read.
		error_delay = 30;

		// check there was some data
		if (n == 0) continue;

		app_status = app_data[1];
		if (app_status != 0) {
			handle_app_status();
		}

		new_step_id = app_data[0];
		if (new_step_id != current_step) {
			current_step = new_step_id;
			show_screen();
		}

		handle_kb();
	}
}