#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "connection.h"
#include "data.h"
#include "debug.h"
#include "keyboard.h"
#include "fujinet-network.h"
#include "who.h"
#include "world.h"

char *world_state = "/ws";
char *who_endpoint = "/who";
char *cmd_endpoint = "/cmd/get/";
char *msg_endpoint = "/msg";

void get_world_state() {
	int n;

	create_command("x-ws");
	send_command();
	read_response((uint8_t *) &world_width, 14);
}

// get up to 240 bytes for all connected clients. we live in hope
void get_world_clients() {
	memset(clients_buffer, 0, 240);
	create_command("x-who");
	send_command();
	read_response(clients_buffer, 240);
}

void get_broadcast() {
	int n;
	create_command("x-msg");
	send_command();
	n = read_response(broadcast_message, 119);

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
	n = read_response(app_data, 240);

/*
    ENABLE_DARK_MODE(1, "enableDarkMode"),
    DISABLE_DARK_MODE(2, "disableDarkMode"),
    ENABLE_WHO(3, "enableWho"),
    DISABLE_WHO(4, "disableWho");
	ENABLE_BROADCAST(5, "enableBroadcast"),
    DISABLE_BROADCAST(6, "disableBroadcast"),
 */

	if (n > 0) {
		for (i = 0; i < n; i++) {
			cmd = app_data[i];
			switch (cmd) {
				case 1: // enable dark mode
					is_darkmode = true;
					do_darkmode();
					break;
				case 2: // disable dark mode
					is_darkmode = false;
					do_darkmode();
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
				default:
					is_showing_broadcast = false;
					break;
			}
		}
	}

}