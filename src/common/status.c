#include <conio.h>
#include <stdint.h>
#include <stdlib.h>

#include "data.h"
#include "display.h"
#include "sound.h"
#include "status.h"

void handle_app_status() {
	// if ((app_status & CLIENT_CHANGE) != 0) {
	// 	// fetch client list, and the world size as that may have increased
	// }

	// if ((app_status & OBJECT_CHANGE) != 0) {
	// 	// an object was added or removed from the world, fetch the latest 
	// }

	// if ((app_status & FROZEN_TOGGLE) != 0) {
	// 	// world frozen state changed
	// }

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