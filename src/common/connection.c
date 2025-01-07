#include <conio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "data.h"
#include "fujinet-network.h"
#include "press_key.h"
#include "shapes.h"

// platform specific values will be supplied here:
#include "screen.h"

char *comma_str  = ",";

void create_command(char *cmd) {
	memset(cmd_tmp, 0, sizeof(cmd_tmp));
	strcat(cmd_tmp, cmd);
}

void append_command(char *cmd) {
	strcat(cmd_tmp, " ");
	strcat(cmd_tmp, cmd);
}

void send_command() {
	network_write(server_url, cmd_tmp, strlen(cmd_tmp));
}

void connect_service() {
	memset(server_url, 0, 128);
	strcat(server_url, endpoint);
	err = network_open(server_url, 0x0C, 0);
	handle_err("connect");
}

// This uses network_read_nb() which is a one shot read, grabbing whatever is in bytes-waiting.
// We cannot use network_read() as it may ask for a larger size than would be returned.
// Normally this is ok, but in TCP where we do not close the connection, it will loop in FN getting BW of 0, and never be able to fetch the rest as there isn't any
int16_t read_response(uint8_t *buf, int16_t len) {
	int16_t n;
	n = network_read_nb(server_url, buf, len);
	if (n < 0) {
		err = -n;
		handle_err("read_response");
	}
	return n;
}

// As above but without error handling, as we need to handle errors in the caller
int16_t read_response_with_error(uint8_t *buf, int16_t len) {
	return network_read_nb(server_url, buf, len);
}

void send_client_data() {
	char tmp[6]; // for the itoa string
	int n;
	memset(tmp, 0, sizeof(tmp));

	// send x-add-client with "name,version,screenX,screenY", and get the client id back

	memset((char *) app_data, 0, 64);
	strcat((char *) app_data, name);
	strcat((char *) app_data, comma_str);
	strcat((char *) app_data, "2"); // version
	strcat((char *) app_data, comma_str);
	itoa(SCREEN_WIDTH, tmp, 10);
	strcat((char *) app_data, tmp);
	strcat((char *) app_data, comma_str);
	itoa(SCREEN_HEIGHT, tmp, 10);
	strcat((char *) app_data, tmp);

	create_command("x-add-client");
	append_command(app_data);
	send_command();
	n = read_response((uint8_t *) &client_id, 1);
	if (n < 0) {
		err = -n;
		handle_err("client_id");
	}

	memset(client_str, 0, 10);
	itoa(client_id, client_str, 10);

	cputsxy(10, 19, "Client ID: ");
	cputsxy(21, 19, client_str);

	press_key();

}
