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
	uint16_t len = strlen((char *) cmd_tmp);

	/* append LF (0x0A); avoid "\n" which cc65 maps to 0x9B on Atari */
	cmd_tmp[len] = 0x0A;
	err = network_write(server_url, (uint8_t *) cmd_tmp, len + 1);
	cmd_tmp[len] = '\0';
	handle_err("send_command");
}

// just send the cached client data command
void request_client_data() {
	uint16_t len = client_data_cmd_len;

	/* append LF (0x0A); avoid '\n' which cc65 maps to 0x9B on Atari */
	client_data_cmd[len] = 0x0A;
	client_data_cmd[len + 1] = '\0';
	err = network_write(server_url, (uint8_t *) client_data_cmd, len + 1);
	client_data_cmd[len] = '\0';
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

static uint16_t packet_size_from_header(const uint8_t *buf)
{
	return (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
}

static int16_t read_raw(uint8_t *buf, int16_t len)
{
	int16_t total = 0;
	int16_t n;

	while (total < len) {
		n = network_read(server_url, buf + total, len - total);
		if (n < 0) {
			err = -n;
			handle_err("read_raw");
			return total;
		}
		if (n == 0) {
			pause(3);
			continue;
		}
		total += n;
	}
	return total;
}

// read fully until we get payload_len bytes from a framed response
// Used for commands where we must receive all the data
int16_t read_response_wait(uint8_t *payload_buf, int16_t payload_len)
{
	uint8_t header[PACKET_HEADER_SIZE];
	uint16_t packet_total;
	int16_t n;

	n = read_raw(header, PACKET_HEADER_SIZE);
	if (n < PACKET_HEADER_SIZE) {
		return n;
	}

	packet_total = packet_size_from_header(header);
	if (packet_total != (uint16_t)(payload_len + PACKET_HEADER_SIZE)) {
		err = 1;
		handle_err("read_response_wait size");
		return -1;
	}

	n = read_raw(payload_buf, payload_len);
	if (n < 0) {
		return n;
	}
	return n;
}

// read a framed response into buf; payload begins at buf + PACKET_HEADER_SIZE
// (use app_payload when buf is app_data). Returns payload length, or -1 on error.
int16_t read_response_min(uint8_t *buf, int16_t min_payload, int16_t max_payload)
{
	int16_t total = 0;
	int16_t n = 0;
	int16_t max_total = (int16_t)(max_payload + PACKET_HEADER_SIZE);
	int16_t need_total = (int16_t)(min_payload + PACKET_HEADER_SIZE);
	uint16_t packet_total = 0;
	uint8_t have_header = 0;

	while (total < need_total) {
		n = network_read_nb(server_url, buf + total, max_total - total);
		if (n < 0) {
			err = -n;
			handle_err("read_response_min");
			return -1;
		}
		if (n == 0) {
			pause(3);
			continue;
		}

		total += n;

		if (total >= PACKET_HEADER_SIZE) {
			if (!have_header) {
				packet_total = packet_size_from_header(buf);
				if (packet_total < PACKET_HEADER_SIZE ||
					packet_total > (uint16_t)max_total) {
					err = 1;
					handle_err("read_response_min bad size");
					return -1;
				}
				need_total = (int16_t)packet_total;
				have_header = 1;
			}
			if (total >= need_total) {
				break;
			}
		}
	}

	if (!have_header) {
		return 0;
	}

	if (total < need_total) {
		return (int16_t)(total - PACKET_HEADER_SIZE);
	}

	if (packet_total != packet_size_from_header(buf)) {
		err = 1;
		handle_err("read_response_min mismatch");
		return -1;
	}

	return (int16_t)(packet_total - PACKET_HEADER_SIZE);
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
