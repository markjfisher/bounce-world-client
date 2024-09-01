#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "debug.h"
#include "world.h"


void print_justified(uint16_t v, uint8_t len) {
	char tmp[8];
	int i, extra;

	memset(tmp, 0, 8);
	itoa(v, tmp, 10);
	extra = len - strlen(tmp);

	for (i = 0; i < extra; i++) cputc(' ');
	cputs(tmp);
}

void print_reverse(char *s) {
	revers(1); cputs(s); revers(0);
}

void show_info() {
	uint8_t xtra;
	// show the current frame
	// show number of bodies by size
	// show all names
	// show is wrapping
	// show is frozen

	cputsxy(0, 20, name);

	cputsxy(16, 20, "1:");
	print_justified(body_1, 2);
	cputs("|2:");
	print_justified(body_2, 2);
	cputs("|3:");
	print_justified(body_3, 2);
	cputs("|4:");
	print_justified(body_4, 2);
	cputs("|5:");
	print_justified(body_5, 2);

	cputsxy(0, 21, "Clients:");
	print_justified(num_clients, 2);

	xtra = (world_height > 99) ? 0 : 1;
	gotoxy(33 + xtra, 21);
	if (world_is_frozen) {
		revers(1);
	}
	print_justified(world_width, 0);
	cputc('x');
	print_justified(world_height, 0);
	if (world_is_frozen) {
		revers(0);
	}

	gotoxy(2, 22);
	print_reverse("F");    cputs("reeze ");
	print_reverse("C");    cputs("lients ");
	print_reverse("W");    cputs("rap ");
	print_reverse("R");    cputs("eset ");
	print_reverse("+/-");  cputs(" Speed ");

	gotoxy(4, 23);
	print_reverse("1-5");  cputs(" Add   ");
	print_reverse("SH 1-5");  cputs(" Del   ");
	print_reverse("Q");    cputs("uit");
}