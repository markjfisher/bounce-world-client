#include <conio.h>
#include <stdint.h>
#include <stdlib.h>

#include "data.h"
#include "debug.h"
#include "display.h"
#include "sound.h"
#include "status.h"
#include "world.h"

void handle_app_status() {
	// if ((app_status & CLIENT_CHANGE) != 0) {
	// 	// fetch client list, and the world size as that may have increased
	// }

	// if ((app_status & OBJECT_CHANGE) != 0) {
	// 	// an object was added or removed from the world, fetch the latest 
	// }

	// someone else may freeze the world, so we have to react to it from an event
	if ((app_status & FROZEN_TOGGLE) != 0) {
		// world frozen state changed
		get_world_state();

		// redisplay the state
		info_display_count = 0;
	}

	// if ((app_status & WRAPPING_TOGGLE) != 0) {
	// 	// world wrapping state changed
	// }

	// if ((app_status & SPEED_CHANGE) != 0) {
	// 	// all objects had speed change
	// }

	// if ((app_status && COLLISION) != 0) {
	// 	// there was a collision in our screen, start the noise going
	// 	sound_collision();
	// }
}