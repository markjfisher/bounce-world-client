#include <conio.h>
#include <screen.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "app_errors.h"
#include "connection.h"
#include "data.h"
#include "debug.h"
#include "delay.h"
#include "display.h"
#include "fujinet-network.h"
#include "resilience.h"
#include "who.h"
#include "world.h"

#ifdef __ATARI__
#include "dlist.h"
#endif

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "itoa_wrapper.h"
#endif

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "itoa_wrapper.h"
#endif

void do_command(char *command) {
	char tmp[1];
	create_command(command);
	send_command();
	read_response(tmp, 1);

	get_world_state();
	info_display_count = 0;
}

void add_body(uint8_t size) {
	char tmp[1];
	char size_string[4];

	create_command("x-add-body");
	itoa(size, size_string, 10);
	append_command(size_string);
	send_command();
	read_response(tmp, 1);

	get_world_state();
	info_display_count = 0;
}

void toggle_darkmode() {
	is_darkmode = !is_darkmode;
	set_screen_colours();
}

void toggle_info() {
	is_showing_info = !is_showing_info;
	set_screen_colours();
	if (!is_showing_broadcast) {
		info_display_count = 0;
	}
}

void handle_kb() {
	char c;

	if (kbhit() == 0) return;

	c = cgetc();
	switch (c) {
		case '+': do_command("x-inc"); break;
		case '-': do_command("x-dec"); break;
		case 'f': do_command("x-freeze"); break;
		
		case '1':
		case '2':
		case '3':
		case '4':
		case '5': add_body(c - '0'); break;

		case 'r': do_command("x-reset"); break;
		case 'i': toggle_info(); break;
		case 'w': is_showing_clients = !is_showing_clients; break;
		case 'q': is_running_sim = false; break;

#if defined(__ATARI__) || defined(__PMD85__)
		case 'd': toggle_darkmode(); break;
		case 'l': flash_on_collision = !flash_on_collision; break;
#endif

		default: break;
	}
}