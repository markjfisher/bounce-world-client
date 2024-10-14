#include <stdint.h>

#include "collision.h"
#include "data.h"
#include "screen_util.h"

static uint8_t is_playing_collision = 0;
static uint8_t current_flash_time = 0;

static uint16_t flash_patterns[] = {
    ACE_GREEN | PATTERN_SOLID, 
    ACE_GREEN | PATTERN_CHESSBOARD, 
    ACE_GREEN | PATTERN_SPARE, 
    PATTERN_BLANK
};

void play_collision() {
    long pattern = (long)flash_patterns[current_flash_time];

	// screen_set_region(1, 2, 46, 2);
	// screen_fill_region(pattern);
	// screen_set_region(1, 27, 46, 2);
	// screen_fill_region(pattern);

	screen_set_region(1, 4, 2, 23);
	screen_fill_region(pattern);
	screen_set_region(45, 4, 2, 23);
	screen_fill_region(pattern);

	if (++current_flash_time > 3) {
		current_flash_time = 0;
		is_playing_collision = 0;
	}
}

void collision_fx() {
	current_flash_time = 0;
	is_playing_collision = 1;

	if (flash_on_collision) {
		// play/animate collision now, it is slow to play it on screen refresh
		while(is_playing_collision)
			play_collision();
	}
}
