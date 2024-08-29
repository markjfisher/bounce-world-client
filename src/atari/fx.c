#include <stdlib.h>

#include "data.h"
#include "fx.h"

bool is_flashing_screen = false;
uint8_t current_flash_time = 0;

void screen_flash() {
	current_flash_time = 0;
	is_flashing_screen = true;
}