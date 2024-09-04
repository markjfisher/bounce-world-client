#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "data.h"
#include "fujinet-network.h"
#include "press_key.h"
#include "shapes.h"

// platform specific values will be supplied here:
#include "screen.h"

char *client_url = "/client";
char *comma_str  = ",";
char *world_str  = "/w/";
char *hb_str     = "/hb/";

// extern void itoa_byte(char *s, uint8_t v);

void connect_service() {
	char tmp[4]; // for the itoa string
	int n;
	memset(tmp, 0, sizeof(tmp));

	// send a POST to <endpoint>/client with "name,version,screenX,screenY", and get the client id back
	memset(url_buffer, 0, sizeof(url_buffer));
	strcat(url_buffer, endpoint);
	strcat(url_buffer, client_url);

	memset((char *) app_data, 0, 64);
	strcat((char *) app_data, name);
	strcat((char *) app_data, comma_str);
	strcat((char *) app_data, "1");
	strcat((char *) app_data, comma_str);
	itoa(SCREEN_WIDTH, tmp, 10);
	strcat((char *) app_data, tmp);
	strcat((char *) app_data, comma_str);
	itoa(SCREEN_HEIGHT, tmp, 10);
	strcat((char *) app_data, tmp);

    err = network_open(url_buffer, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
	handle_err("post:open");

    network_http_start_add_headers(url_buffer);
    network_http_add_header(url_buffer, "Accept: */*");
    network_http_add_header(url_buffer, "Content-Type: text/plain");
    network_http_end_add_headers(url_buffer);

	err = network_http_post(url_buffer, app_data);
	handle_err("post:data");

	// finally read the client id in the response, this is just 1 byte
	n = network_read(url_buffer, (uint8_t *) &client_id, 1);
	if (n < 0) {
		err = -n;
		handle_err("client_id");
	}
	network_close(url_buffer);

	memset(client_str, 0, 4);
	itoa(client_id, client_str, 10);

	cputsxy(10, 19, "Client ID: ");
	cputsxy(21, 19, client_str);

	// create the world update endpoint for this client
	strcpy(client_data_url, endpoint);
	strcat(client_data_url, world_str);
	strcat(client_data_url, client_str);

	press_key();

}
