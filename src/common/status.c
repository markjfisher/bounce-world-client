#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "collision.h"
#include "data.h"
#include "debug.h"
#include "display.h"
#include "status.h"
#include "world.h"

void handle_app_status() {
	bool is_client_change = (app_status & CLIENT_CHANGE) != 0;
	bool is_frozen_change = (app_status & FROZEN_TOGGLE) != 0;
	bool is_object_change = (app_status & OBJECT_CHANGE) != 0;
	bool is_collision     = (app_status & COLLISION) != 0;

	if (is_client_change || is_frozen_change || is_object_change) {
		get_world_state();
		info_display_count = 0;
	}

	if (is_client_change) {
		get_world_clients();
	}

	if (is_collision) {
		collision_fx();
	}

}