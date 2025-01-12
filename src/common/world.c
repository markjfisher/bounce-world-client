#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
	int16_t n;
	uint8_t i;
	uint16_t error_delay = 50;
	uint8_t attempts = 0;
	bool got_state = false;

	while (!got_state && attempts < 60) {
		memset(app_data, 0, APP_DATA_SIZE);
		request_client_data();
		n = read_response_with_error((uint8_t *) app_data, APP_DATA_SIZE);
		if (n < 0) {
			// there was an error, so don't process this round. try again after a small pause
			// We use some resillience here, backing off until the error delay is 255 (about 5 seconds)
			// TODO: check if we errored too many times maybe? Or just let the user decide to reboot etc.

			for (i = 0; i < 20; i++) {
				// Flash an E to indicate an error
				cputcxy(39, 23, 'E');
				pause(error_delay);
				cputcxy(39, 23, ' ');
				pause(error_delay);
			}

			error_delay += error_delay >> 1;
			if (error_delay > 255) error_delay = 255;
			attempts++;
			continue;
		} else {
			got_state = true;
		}
		if (n == 0) {
			debug();
		}
	}

	return n;
}

void get_world_state() {
	create_command("x-ws");
	send_command();
	read_response((uint8_t *) &world_width, 14);
}

// get up to 512 bytes for all connected clients. we live in hope
void get_world_clients() {
	memset(clients_buffer, 0, 512);
	create_command("x-who");
	send_command();
	read_response((uint8_t *) clients_buffer, 512);
}

void get_broadcast() {
	int n;
	create_command("x-msg");
	send_command();
	n = read_response((uint8_t *) broadcast_message, 119);

	if (n > 0) {
		// terminate the string. We only read 119 bytes to not overrun the buffer, but always add a nul terminator.
		broadcast_message[n] = '\0';
	}
}

// this client has some commands to process
void get_world_cmd() {
	int n;
	uint8_t i, cmd;

	create_command("x-cmd-get");
	append_command(client_str);
	send_command();
	n = read_response((uint8_t *) app_data, 240);

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