#ifndef _CMOC_VERSION_
#include <conio.h>
#include <screen.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#include "stdbool.h"
#include "conio_wrapper.h"
#endif

#include "app_errors.h"
#include "connection.h"
#include "data.h"
#include "debug.h"
#include "delay.h"
#include "display.h"
#include "fujinet-network.h"
#include "who.h"
#include "world.h"

#ifdef __ATARI__
#include "dlist.h"
#endif

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "itoa_wrapper.h"
#endif

void do_command(char *command) {
	create_command(command);
	send_command();

	// the response is discarded as it's not needed
	read_response_wait((uint8_t *) cmd_tmp, 1);

	get_world_state();
	info_display_count = 0;
}

void add_body(uint8_t size) {
	char size_string[4];

	create_command("x-add-body");
	itoa(size, size_string, 10);
	append_command(size_string);
	send_command();

	// the response is discarded as it's not needed, but must be read to ensure TCP reading is aligned
	read_response_wait((uint8_t *) cmd_tmp, 1);

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

#ifdef _CMOC_VERSION_
	c = (char) kbhit();

	if (c == 0) return;
#else
	if (kbhit() == 0) return;

	c = cgetc();
#endif
	switch (c) {
		case '+': do_command("x-inc"); break;
		case '-': do_command("x-dec"); break;
		case 'F':
		case 'f': do_command("x-freeze"); break;
		
		case '1':
		case '2':
		case '3':
		case '4':
		case '5': add_body(c - '0'); break;

		case 'R':
		case 'r': do_command("x-reset"); break;
		case 'I':
		case 'i': toggle_info(); break;
		case 'W':
		case 'w': is_showing_clients = !is_showing_clients; break;
		case 'Q':
		case 'q': is_running_sim = false; break;

#ifdef _CMOC_VERSION_
		case 'C':
		case 'c':
			switch_colorset();
#endif

#if defined(__ATARI__) || defined(__PMD85__)
		case 'd': toggle_darkmode(); break;
		case 'l': flash_on_collision = !flash_on_collision; break;
#endif

		default: break;
	}
}
