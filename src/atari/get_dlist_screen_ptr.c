#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <peekpoke.h>

#include "debug.h"

#define POINTER_AT(addr) ((uint8_t *)(PEEKW(addr)))

uint8_t *get_dlist_screen_ptr() {
	uint8_t *mem_loc;
	// uint16_t address;
	uint8_t *dlist = POINTER_AT(0x0230);

	// debug();
	// skip blank line instructions
	while ((*dlist & 0x0F) == 0) {
		dlist++;
	}

	// find the next byte that has an LMS bit
	while ((*dlist & 0x40) == 0) {
		dlist++;
	}

	// skip the LMS to start of the memory location
	dlist++;

	return dlist;

	// address = dlist[0] | (dlist[1] << 8);
	// mem_loc = (uint8_t *) address;
	// return mem_loc;

}