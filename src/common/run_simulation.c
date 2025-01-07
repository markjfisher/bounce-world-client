#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "connection.h"
#include "data.h"
#include "display.h"
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


void run_simulation() {
	int n;
	uint16_t error_delay = 30;
	uint8_t new_step_id;
	init_screen();

#ifdef __ATARI__
	// setup double buffering for atari
	dlist_scr_ptr = get_dlist_screen_ptr();
	screen_mem_orig = (uint8_t *) ((uint16_t)(dlist_scr_ptr[0]) | ((uint16_t)(dlist_scr_ptr[1]) << 8));
#endif

	// flag we are on initial screen for double buffering across the platforms
	is_orig_screen_mem = 1;

	get_world_clients();

	while(is_running_sim) {
		create_command("x-w");
		append_command(client_str);
		send_command();
		n = read_response_with_error(app_data, APP_DATA_SIZE);
		if (n < 0) {
			// there was an error, so don't process this round. try again after a small pause
			// We use some resillience here, backing off until the error delay is 255 (about 5 seconds)
			// TODO: check if we errored too many times maybe? Or just let the user decide to reboot etc.
			// cputcxy(39, 23, 'E');
			pause(error_delay);
			error_delay = error_delay * 14 / 10; // roughly sqrt(2) times previous delay if keep getting errors
			if (error_delay > 255) error_delay = 255;
			continue;
		}
		// reset error delay when we have a good read.
		error_delay = 30;
		// cputcxy(39, 23, ' ');

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