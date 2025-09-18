#ifndef _CMOC_VERSION_
#include <conio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#include "conio_wrapper.h"
#endif
#include "app_errors.h"
#include "data.h"
#include "delay.h"
#include "fujinet-network.h"
#include "hex_dump.h"
#include "press_key.h"
#include "shapes.h"
#include "world.h"

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "itoa_wrapper.h"
// #elif defined __ADAM__
// #include "conio_wrapper.h"
#endif

// platform specific values will be supplied here:
#include "screen.h"

extern uint8_t fn_default_timeout;

// extern void debug();

void create_command(char *cmd) {
	memset(cmd_tmp, 0, 64);
	strcpy((char *) cmd_tmp, cmd);
	// cmd_tmp[strlen(cmd)] = '\0';
}

void append_command(char *cmd) {
	strcat((char *) cmd_tmp, " ");
	strcat((char *) cmd_tmp, cmd);
	// cmd_tmp[strlen((char *) cmd_tmp)] = '\0';
}

void send_command() {
	// gotoxy(0, 0);
	// hd(cmd_tmp, 64);
	// cgetc();
	err = network_write(server_url, (uint8_t *) cmd_tmp, strlen((char *) cmd_tmp));
	handle_err("send_command"); 
}

// just send the cached client data command
void request_client_data() {
	err = network_write(server_url, (uint8_t *) client_data_cmd, client_data_cmd_len);
	handle_err("request_client_data");
}

void connect_service() {
	fn_default_timeout = 2;
	err = network_open(server_url, 0x0C, 0);
	fn_default_timeout = 15;
	handle_err("connect");
}

void disconnect_service() {
	create_command("close");
	append_command(client_str);
	send_command();
	network_close(server_url);
}

// read fully until we get len bytes
// Used for commands where we must receive all the data
int16_t read_response_wait(uint8_t *buf, int16_t len) {
	int16_t n;
	n = network_read(server_url, buf, len);
	if (n < 0) {
		err = -n;
		handle_err("read_response_wait");
	}
	return n;
}

// read at least min bytes. Forces something to be read, not skip if there's no bytes waiting
int16_t read_response_min(uint8_t *buf, int16_t min, int16_t max) {
	int16_t n = 0;
	int16_t total = 0;
	while (total < min) {
		n = network_read_nb(server_url, buf + total, max - total);
		if (n < 0) {
			err = -n;
			handle_err("read_response_min");
		}
		// if we got no data, pause slightly and try again. this compensates for network latency
		if (n == 0) {
			pause(3); // about 50ms
			continue;
		}

		total += n;
	}
	return total;
}


void send_client_data() {
	char tmp[6]; // for the itoa string
	memset(tmp, 0, sizeof(tmp));

	// send x-add-client with "name,version,screenX,screenY", and get the client id back

	memset((char *) app_data, 0, 64);
	strcat((char *) app_data, name);
	strcat((char *) app_data, ",2,"); // version
	itoa(SCREEN_WIDTH, tmp, 10);
	strcat((char *) app_data, tmp);
	strcat((char *) app_data, ",");
	itoa(SCREEN_HEIGHT, tmp, 10);
	strcat((char *) app_data, tmp);

	create_command("x-add-client");
	append_command((char *) app_data);
	send_command();
	read_response_wait((uint8_t *) &client_id, 1);
	if (client_id == 0) {
		err = -1;
		handle_err("bad client id");
	}

	memset(client_str, 0, 8);
	itoa(client_id, client_str, 10);

	cputsxy(10, 19, "Client ID: ");
	cputsxy(21, 19, client_str);

	// create the cached client data command like "x-w <id>"
	create_client_data_command();

	press_key();

}
