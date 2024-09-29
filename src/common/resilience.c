#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "app_errors.h"
#include "data.h"
#include "delay.h"
#include "fujinet-network.h"
#include "resilience.h"

void try_open(char *msg, char *url, uint8_t mode) {
	uint8_t err_count = 0;
	bool has_open = false;

	while (err_count < 5) {
		err = network_open(url, mode, OPEN_TRANS_NONE);
		if (err == FN_ERR_OK) {
			has_open = true;
			break;
		} else {
			err_count++;
			pause(40);
		}
	}
	if (!has_open) handle_err(msg);
}