#ifndef _CMOC_VERSION_
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#endif
#include "app_errors.h"
#include "connection.h"
#include "data.h"
#include "debug.h"
#include "delay.h"
#include "display.h"
#include "keyboard.h"
#include "fujinet-network.h"
#include "who.h"
#include "world.h"

char *x_w_cmd = "x-w ";

void create_client_data_command() {
	memset(client_data_cmd, 0, 10);
	strcpy(client_data_cmd, x_w_cmd);
	strcat(client_data_cmd, client_str);
	client_data_cmd_len = strlen(client_data_cmd);
}

int16_t fetch_client_state() {
	memset(app_data, 0, APP_DATA_SIZE);
	request_client_data();
	return read_response_min((uint8_t *) app_data, 1, APP_DATA_SIZE);
}

void get_world_state() {
	create_command("x-ws");
	send_command();
#if defined(__MSDOS__) || defined(_CMOC_VERSION_)
	{
		// Wire format is little-endian uint16. cc65/z88dk LE targets read
		// directly into &world_width via BSS contiguity. Two platforms can't:
		//  - msdos (Watcom): reorders BSS, so contiguity isn't guaranteed.
		//  - coco (cmoc): 6809 is big-endian, so a raw read swaps the bytes.
		// Both unpack into a buffer and assemble the uint16s as LE.
		uint8_t buf[14];
		read_response_wait(buf, 14);
		world_width      = buf[0] | ((uint16_t)buf[1] << 8);
		world_height     = buf[2] | ((uint16_t)buf[3] << 8);
		body_count       = buf[4] | ((uint16_t)buf[5] << 8);
		body_1           = buf[6];
		body_2           = buf[7];
		body_3           = buf[8];
		body_4           = buf[9];
		body_5           = buf[10];
		num_clients      = buf[11];
		world_is_frozen  = buf[12];
		world_is_wrapped = buf[13];
	}
#else
	read_response_wait((uint8_t *) &world_width, 14);
#endif
}

// get up to 512 bytes for all connected clients. we live in hope
void get_world_clients() {
	memset(clients_buffer, 0, 512);
	create_command("x-who");
	send_command();
	read_response_min((uint8_t *) clients_buffer, 1, 512);
}

void get_broadcast() {
	int n;
	create_command("x-msg");
	send_command();
	n = read_response_min((uint8_t *) broadcast_message, 1, 119);

	broadcast_message[n] = '\0';
}

// this client has some commands to process
void get_world_cmd() {
	int n;
	uint8_t i, cmd;

	create_command("x-cmd-get");
	append_command(client_str);
	send_command();
	n = read_response_min((uint8_t *) app_data, 1, 256);

/*
    ENABLE_DARK_MODE(1, "enableDarkMode"),
    DISABLE_DARK_MODE(2, "disableDarkMode"),
    ENABLE_WHO(3, "enableWho"),
    DISABLE_WHO(4, "disableWho");
    ENABLE_BROADCAST(5, "enableBroadcast"),
    DISABLE_BROADCAST(6, "disableBroadcast"),
    ENABLE_INFO(7, "enableInfo"),
    DISABLE_INFO(8, "disableInfo"),
 */

	if (n > 0) {
		for (i = 0; i < n; i++) {
			cmd = app_data[i];
			switch (cmd) {
				case 1: // enable dark mode
					is_darkmode = true;
					set_screen_colours();
					break;
				case 2: // disable dark mode
					is_darkmode = false;
					set_screen_colours();
					break;
				case 3: // enable who
					is_showing_clients = true;
					break;
				case 4: // disable who
					is_showing_clients = false;
					break;
				case 5: // enable broadcast
					get_broadcast();
					is_showing_broadcast = true;
					break;
				case 6: // disable broadcast
					get_broadcast();
					is_showing_broadcast = false;
					break;
				case 7: // enable info. to use the toggle_info function we have to first set it to opposite state to what we eventually want
					is_showing_info = false;
					toggle_info();
					break;
				case 8: // disable info
					is_showing_info = true;
					toggle_info();
					break;
				default:
					break;
			}
		}
	}

}
