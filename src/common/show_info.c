#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "double_buffer.h"
#include "debug.h"
#include "world.h"

// double buffering macros, apple2 needs a hand.
#ifdef __APPLE2__
#include "conio_helpers.h"
#define GOTOXY gotoxy_buffer
#define CPUTSXY cputsxy_buffer
#else
#define GOTOXY gotoxy
#define CPUTSXY cputsxy
#endif


// spaces are printed on the right
void print_justified(uint16_t v, uint8_t len) {
	char tmp[8];
	int i, extra;

	memset(tmp, 0, 8);
	itoa(v, tmp, 10);
	extra = len - strlen(tmp);

	cputs(tmp);
	for (i = 0; i < extra; i++) cputc(' ');
}

void print_reverse(char *s) {
	revers(1); cputs(s); revers(0);
}


void show_info() {
	uint8_t xtra;

	CPUTSXY(0, 22, name);

	revers(1); CPUTSXY(9, 22, "C:"); revers(0); print_justified(num_clients, 2);

	revers(1); cputs("1:"); revers(0); print_justified(body_1, 2);
	revers(1); cputs("2:"); revers(0); print_justified(body_2, 2);
	revers(1); cputs("3:"); revers(0); print_justified(body_3, 2);
	revers(1); cputs("4:"); revers(0); print_justified(body_4, 2);
	revers(1); cputs("5:"); revers(0); print_justified(body_5, 2);


	xtra = (world_height > 99) ? 0 : 1;
	GOTOXY(32 + xtra, 22);
	if (world_is_frozen) {
		revers(1);
	}
	print_justified(world_width, 0);
	cputc('x');
	print_justified(world_height, 0);
	if (world_is_frozen) {
		revers(0);
	}

	GOTOXY(0, 23);
	print_reverse("F");    cputs("reeze ");
	print_reverse("R");    cputs("eset ");
	print_reverse("+-");  cputs("Spd ");
	print_reverse("1-5");  cputs("Add ");
	print_reverse("W");  cputs("ho ");
	print_reverse("Q");    cputs("uit ");
#ifdef __ATARI__
	print_reverse("D");    cputs("ark");
#endif
}