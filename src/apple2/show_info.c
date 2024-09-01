#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "double_buffer.h"
#include "debug.h"
#include "world.h"


/*
 * APPLE2 version of the text area. Needs special double buffer handling around goto commands for text.
 */

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

void gotoxy_buffer(uint8_t x, uint8_t y) {
	gotoxy(x, y);
	check_text_buffer_location();
}

void cputsxy_buffer(uint8_t x, uint8_t y, char *s) {
	gotoxy_buffer(x, y);
	cputs(s);
}

void show_info() {
	uint8_t xtra;

	cputsxy_buffer(0, 20, name);

	cputsxy_buffer(16, 20, "1:");
	print_justified(body_1, 2);
	cputs("|2:");
	print_justified(body_2, 2);
	cputs("|3:");
	print_justified(body_3, 2);
	cputs("|4:");
	print_justified(body_4, 2);
	cputs("|5:");
	print_justified(body_5, 2);

	cputsxy_buffer(0, 21, "Clients:");
	print_justified(num_clients, 2);

	xtra = (world_height > 99) ? 0 : 1;
	gotoxy_buffer(33 + xtra, 21);
	if (world_is_frozen) {
		revers(1);
	}
	print_justified(world_width, 0);
	cputc('x');
	print_justified(world_height, 0);
	if (world_is_frozen) {
		revers(0);
	}

	gotoxy_buffer(2, 22);
	print_reverse("F");    cputs("reeze ");
	print_reverse("C");    cputs("lients ");
	print_reverse("W");    cputs("rap ");
	print_reverse("R");    cputs("eset ");
	print_reverse("+/-");  cputs(" Speed ");

	gotoxy_buffer(4, 23);
	print_reverse("1-5");  cputs(" Add   ");
	print_reverse("SH 1-5");  cputs(" Del   ");
	print_reverse("Q");    cputs("uit");
}