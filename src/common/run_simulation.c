#include <conio.h>
#include <stdbool.h>
#include <stdint.h>

#include "data.h"
#include "display.h"
#include "double_buffer.h"
#include "fujinet-network.h"

#ifdef __ATARI__
#include "get_dlist_screen_ptr.h"
#endif

void run_simulation() {
	int n = 0;
	uint8_t current_frame = 0;

	clrscr();
#ifdef __ATARI__
	dlist_scr_ptr = get_dlist_screen_ptr();
	screen_mem_orig = (uint8_t *) ((uint16_t)(dlist_scr_ptr[0]) | ((uint16_t)(dlist_scr_ptr[1]) << 8));
#endif
	is_orig_screen_mem = 1;

	// the core loop for grabbing data from FN and then displaying it
	while(is_running_sim) {
		network_open(endpoint, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
		n = network_read(endpoint, location_data, 256);
		network_close(endpoint);

		display_positions();
		// if (n > 0) {
		// 	display_positions();
		// }

		// handle_kb();

		// do we need a pause here?

	}
}