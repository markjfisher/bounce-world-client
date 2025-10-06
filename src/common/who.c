#ifndef _CMOC_VERSION_
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#else
#include <cmoc.h>
#include <coco.h>
#include <chardef.h>
#include <conio_wrapper.h>
#endif
#include "app_errors.h"
#include "data.h"
#include "fujinet-network.h"
#include "screen.h"
#include "who.h"
#include "world.h"

#ifdef __APPLE2__
#include "conio_helpers.h"
#define GOTOXY gotoxy_buffer
#define CPUTSXY cputsxy_buffer
#define CPUTC(c) cputc(c)
#elif __PMD85__
#include "text_buffer.h"
#include "chardef.h"
#include "double_buffer.h"
#undef GOTOXY
#undef CPUTC
#define GOTOXY(x, y) gotoxy_tb(x, y)
#define CPUTC(c) putc_upper_tb(c)
#else
#define GOTOXY gotoxy
#define CPUTSXY cputsxy
#define CPUTC(c) cputc(c)
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
	uint8_t max_show = SCREEN_HEIGHT - 4;
	uint8_t to_show = num_clients;
	if (to_show > max_show) {
		to_show = max_show;
	}

	// show a fake box on the screen, it will be displayed on the buffer screen, so should just overlay bodies

	// top of box
	GOTOXY(SCREEN_WIDTH-11, 2);
	for (i = 0; i < 10; i++) {
		CPUTC(grid[0][i]);
	}

	// all the clients, these are space buffered for us by server
	for (i = 0; i < to_show; i++) {
		GOTOXY(SCREEN_WIDTH-11, 3+i);
		CPUTC(CH_VLINE);
		for (j = 0; j < 8; j++) {
			CPUTC(clients_buffer[(i << 3) + j]);
		}

		CPUTC(CH_VLINE);
	}

	// bottom of box
	GOTOXY(SCREEN_WIDTH-11, 3+num_clients);
	for (i = 0; i < 10; i++) {
		CPUTC(grid[1][i]);
	}

#ifdef __PMD85__
	add_dirty_rect(29, 2, 10, num_clients+2);
#endif
}
