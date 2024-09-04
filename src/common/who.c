#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "app_errors.h"
#include "data.h"
#include "fujinet-network.h"
#include "who.h"
#include "world.h"

#ifdef __APPLE2__
#include <apple2enh.h>
#include "conio_helpers.h"
#define GOTOXY gotoxy_buffer
#define CPUTSXY cputsxy_buffer
#else
#define GOTOXY gotoxy
#define CPUTSXY cputsxy
#endif

// for the corner chars
#if defined(__ATARI__)
#include <atari.h>
#elif defined(__CBM__)
#include <cbm.h>
#endif

static char grid[2][10] = {
	{CH_ULCORNER, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_URCORNER },
	{CH_LLCORNER, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_HLINE, CH_LRCORNER }
};

void show_clients() {
	uint8_t i, j;

	// show a fake box on the screen, it will be displayed on the buffer screen, so should just overlay bodies

	// top of box
	GOTOXY(29, 2);
	for (i = 0; i < 10; i++) {
		cputc(grid[0][i]);
	}

	// all the clients, these are space buffered for us by server
	for (i = 0; i < num_clients; i++) {
		GOTOXY(29, 3+i);
		cputc(CH_VLINE);
		for (j = 0; j < 8; j++) {
			cputc(clients_buffer[(i << 3) + j]);
		}
		cputc(CH_VLINE);
	}

	// bottom of box
	GOTOXY(29, 3+num_clients);
	for (i = 0; i < 10; i++) {
		cputc(grid[1][i]);
	}

}
