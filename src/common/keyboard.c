#ifdef __ATARI__
#include <atari.h>
#endif

#include <conio.h>
#include <screen.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "app_errors.h"
#include "data.h"
#include "debug.h"
#include "display.h"
#include "fujinet-network.h"
#include "world.h"

char *freeze_endpoint = "/freeze";

void change_speed(bool is_increase) {

}

void toggle_freeze() {
	char tmp[1];
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, freeze_endpoint);

    err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("get:open:freeze");
	network_read(url_buffer, tmp, 1);

	get_world_state();
	info_display_count = 0;
}

void add_body(uint8_t size) {
}

void toggle_wrapping() {

}

void handle_kb() {
	char c;
	if (kbhit() == 0) return;

	c = cgetc();
	switch (c) {
		case '+': change_speed(true); break;
		case '-': change_speed(false); break;
		case 'f': toggle_freeze(); break;
		
		case '1':
		case '2':
		case '3':
		case '4':
		case '5': add_body(c - '0'); break;

		case 'w': toggle_wrapping(); break;
		case 'q': is_running_sim = false; break;

		default: break;
	}
}