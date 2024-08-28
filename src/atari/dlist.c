#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <peekpoke.h>

#include "debug.h"
#include "delay.h"
#include "dlist.h"

extern void dli(void);
extern void enable_dli(void);

// retrieves the pointer to the screen location pointed to by the current DLIST
uint8_t *get_dlist_screen_ptr() {
	uint8_t *mem_loc;
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

	// make the colour change before the first lines of the display, and in the text area
	dlist[0]  = 0x70 + 0x80; 				// add DLI to the LMS on first line
	dlist[24] = 0x02 + 0x80; 				// add DLI to the 20th
	dlist[26] = 0x02 + 0x80; 				// add DLI to the 22nd

	// set the DLI routine, and then enable DLIs on display
	OS.vdslst = dli;
	enable_dli();

}
