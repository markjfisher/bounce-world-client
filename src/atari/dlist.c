#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <peekpoke.h>

#include "debug.h"
#include "delay.h"
#include "dlist.h"

extern void dli(void);

uint8_t *get_dlist_screen_ptr() {
	uint8_t *mem_loc;
	uint8_t *dlist = (uint8_t *) OS.sdlst;

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
}

void setup_dli() {
	// Our DLI ensures we have colour separation between printing area and the graphics area, so we don't need a border and save
	// a line we can use for more output.

	uint8_t *dlist = (uint8_t *) OS.sdlst;
	ANTIC.nmien = 0x40;						// Unset the DLI bit in nmien in case it's on while we install our routine

	// make the colour change before the first lines of the display, and before last 4 lines of text
	dlist[0]  = 0x70 + 0x80; 				// add DLI to the LMS on first line
	dlist[24] = 0x02 + 0x80; 				// add DLI to the 20th

	// set the DLI routine.
	OS.vdslst = dli;

	// set nmien when we're in VBI at top so the DLI colouring is correct.
	while(OS.rtclok[2] != 0) ;
	ANTIC.nmien = 0xC0;						// enable DLI

}
