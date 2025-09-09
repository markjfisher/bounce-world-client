#ifndef _CMOC_VERSION_
#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#include "conio_wrapper.h"
#endif
#include "connection.h"
#include "data.h"
#include "display.h"
#include "debug.h"
#include "delay.h"
#include "double_buffer.h"
#include "fujinet-network.h"
#include "keyboard.h"
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
	uint8_t new_step_id;

	init_screen();

	// flag we are on initial screen for double buffering across the platforms
	is_alt_screen = 0;
	// Ensure that the is_running_sim flag is set if it was turned off previously
	is_running_sim = 1;

	get_world_clients();

	while(is_running_sim) {
		n = fetch_client_state();

		// Nothing to show this round, so reloop and check again
		if (n == 1) {
			// debug();
			continue;
		}

		// handle any app events
		app_status = app_data[1];
		if (app_status != 0) {
			handle_app_status();
		}

		// check if the server is frozen or not, by checking the step id
		new_step_id = app_data[0];
		if (new_step_id != current_step) {
			current_step = new_step_id;
			show_screen();
		}

		// handle any keyboard events
		handle_kb();

		// debug: slow things down so we can monitor the logs and catch them when it errors.
		// pause(60);
	}

	// either errored, or quit by user, so deregister from the server
	disconnect_service();

}
