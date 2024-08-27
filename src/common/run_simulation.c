#include <conio.h>
#include <stdint.h>

#include "data.h"
#include "display.h"
#include "delay.h"
#include "double_buffer.h"
#include "fujinet-network.h"
#include "keyboard.h"
#include "sound.h"
#include "status.h"

#ifdef __ATARI__
#include "dlist.h"
#endif

extern bool is_playing_collision;

void run_simulation() {
	uint8_t new_step_id = 0;
	clrscr();
	init_screen();

#ifdef __ATARI__
	// setup double buffering for atari
	dlist_scr_ptr = get_dlist_screen_ptr();
	screen_mem_orig = (uint8_t *) ((uint16_t)(dlist_scr_ptr[0]) | ((uint16_t)(dlist_scr_ptr[1]) << 8));
#endif

	// flag we are on initial screen for double buffering across the platforms
	is_orig_screen_mem = 1;

	while(is_running_sim) {
		// I originally used a continuous open channel that received data from server as it was pushed, but it
		// became clear it's easier to pull from client, so it didn't fall behind, and fetched current when it can.
		// I thought the push model would provide more consistency across views for the client, but it turns out
		// there's negligible delay between platforms constantly polling and the screens being updated, so pull model is better.
		if (is_playing_collision) {
			pause(1);
			continue;
		}

		network_open(endpoint, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
		// this is a huge buffer, received data is only 3 bytes per body displayed, plus 2 overhead bytes, e.g. 10 bodies on screen would be 32 bytes only.
		network_read(endpoint, app_data, APP_DATA_SIZE);
		network_close(endpoint);

		new_step_id = app_data[0];
		if (new_step_id != current_step) {
			current_step = new_step_id;
			show_screen();
		}

		app_status = app_data[1];
		if (app_status != 0) {
			handle_app_status();
		}

		handle_kb();
	}
}