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
#include "who.h"
#include "world.h"

#ifdef __ATARI__
#include "dlist.h"
#endif

char *freeze_endpoint = "/freeze";
char *reset_endpoint = "/reset";
char *inc_endpoint = "/inc";
char *dec_endpoint = "/dec";
char *add_endpoint = "/add/";

void do_command(char *command) {
	char tmp[1];
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, command);

    err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("get:open:command");
	network_read(url_buffer, tmp, 1);

	get_world_state();
	info_display_count = 0;
	
}

void add_body(uint8_t size) {
	char tmp[1];
	char size_string[2];

	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, add_endpoint);
	itoa(size, size_string, 10);
	strcat(url_buffer, size_string);

    err = network_open(url_buffer, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("get:open:add");
	network_read(url_buffer, tmp, 1);

	get_world_state();
	info_display_count = 0;

}

void do_darkmode() {
#ifdef __ATARI__
	if (is_darkmode) {
		txt_c1 = 0;
		txt_c2 = 0;
		txt_c3 = 0;
	} else {
		txt_c1 = INIT_COLOUR_1;
		txt_c2 = INIT_COLOUR_2;
		txt_c3 = INIT_COLOUR_3;
	}
#endif

}

void toggle_darkmode() {
	is_darkmode = !is_darkmode;
	do_darkmode();
}


void handle_kb() {
	char c;
	if (kbhit() == 0) return;

	c = cgetc();
	switch (c) {
		case '+': do_command(inc_endpoint); break;
		case '-': do_command(dec_endpoint); break;
		case 'f': do_command(freeze_endpoint); break;
		
		case '1':
		case '2':
		case '3':
		case '4':
		case '5': add_body(c - '0'); break;

		case 'r': do_command(reset_endpoint); break;
		case 'i': is_showing_info = !is_showing_info; break;
		case 'w': is_showing_clients = !is_showing_clients; break;
		case 'q': is_running_sim = false; break;

#ifdef __ATARI__
		case 'd': toggle_darkmode(); break;
#endif

		default: break;
	}
}