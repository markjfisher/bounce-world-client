#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "data.h"
#include "fujinet-network.h"
#include "heartbeat.h"
#include "press_key.h"
#include "shapes.h"

// platform specific values will be supplied here:
#include "screen.h"

char *client_url = "/client";
char *comma_str  = ",";
char *world_str  = "/w/";
char *hb_str     = "/hb/";

extern void itoa_byte(char *s, uint8_t v);


void connect_service() {
	char *post_data;
	char tmp[4]; // for the itoa string
	int n;

	// send a POST to <endpoint>/client with "name,version,screenX,screenY", and get the client id back
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, client_url);

	post_data = malloc(80);
	memset(post_data, 0, 80);
	strcpy(post_data, name);
	strcat(post_data, comma_str);
	strcat(post_data, "1");
	strcat(post_data, comma_str);
	itoa_byte(tmp, SCREEN_WIDTH);
	strcat(post_data, tmp);
	strcat(post_data, comma_str);
	itoa_byte(tmp, SCREEN_HEIGHT);
	strcat(post_data, tmp);

    err = network_open(url_buffer, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
	handle_err("post:open");

    network_http_start_add_headers(url_buffer);
    network_http_add_header(url_buffer, "Accept: */*");
    network_http_add_header(url_buffer, "Content-Type: text/plain");
    network_http_end_add_headers(url_buffer);

	err = network_http_post(url_buffer, post_data);
	handle_err("post:data");

	// finally read the client id in the response. client ids are 8 chars, we have 9 char buffer, with 0 at end so can treat it as a string
	memset(client_id, 0, sizeof(client_id));
	n = network_read(url_buffer, (uint8_t *)client_id, 8);
	if (n < 0) {
		err = -n;
		handle_err("client_id");
	}

	cputsxy(10, 19, "Client ID: ");
	cputsxy(21, 19, client_id);

	// now append this to the 2 strings we have:
	// <endpoint>/w/<client> on n1: for data
	// <heartbeat>/hb/<client> on n2: for heartbeat

	strcat(endpoint, world_str);
	strcat(endpoint, client_id);

	strcat(heartbeat, hb_str);
	strcat(heartbeat, client_id);

	// open the endpoint to begin data flow
	err = network_open(endpoint, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("open data channel");

	send_heartbeat();

	press_key();

	free(post_data);
}