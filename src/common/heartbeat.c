#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "data.h"
#include "fujinet-network.h"

void send_heartbeat() {
	char tmp[2];
	err = network_open(heartbeat, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
	handle_err("open heartbeat channel");
	network_read(heartbeat, (uint8_t *)tmp, 2);
	network_close(heartbeat);
}