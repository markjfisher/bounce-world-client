#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "debug.h"
#include "delay.h"
#include "dlist.h"

// void main_dlist = {
// 	DL_DLI(DL_BLK6),	// drop 2 lines - is it worth it?
// 	DL_BLK8,
// 	DL_BLK8,
// 	DL_LMS(DL_GRAPHICS0), 0x00, 0x00, // this needs the address of the memory for screen
// 	DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0,
// 	DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0,
// 	DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0, DL_GRAPHICS0,
// 	// add a blank line with DLI which should help with timing the colour to the GR0 lines
// 	DL_DLI(BL_BLK1),
// 	DL_GRAPHICS0, DL_GRAPHICS0,
// 	// same
// 	DL_DLI(BL_BLK1),
// 	DL_GRAPHICS0, DL_GRAPHICS0,

// 	// and point back to the start
// 	DL_JVB, &main_dlist
// };

uint8_t txt_c1 = 0;
uint8_t txt_c2 = 0;
uint8_t txt_c3 = 0;

// retrieves the pointer to the screen location pointed to by the current DLIST
uint8_t *get_dlist_screen_ptr() {
	uint8_t *dlist = (uint8_t *) OS.sdlst;

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
}

void setup_dli() {
	// Our DLI ensures we have colour separation between printing area and the graphics area, so we don't need a border and save
	// a line we can use for more output.

	uint8_t *dlist = (uint8_t *) OS.sdlst;
	ANTIC.nmien = 0x40;						// Unset the DLI bit in nmien in case it's on while we install our routine

	// DLIs are in latter part of entire display
	dlist[26] = 0x02 + 0x80; 				// add DLI to line 22
	dlist[27] = 0x02 + 0x80; 				// add DLI to line 23

	// set the DLI routine, and then enable DLIs on display
	OS.vdslst = dli;
	enable_dli();

}
