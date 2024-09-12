#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "double_buffer.h"
#include "debug.h"
#include "world.h"

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "itoa_wrapper.h"
#include "screen_util.h"
#endif

#ifdef __PMD85__

void show_info() {
	//                    0123456789012345678901234567890
	static char info[] = "C:                             ";
//	static char info[] = "C:    1   2   3   4   5     x  ";
	static char tmp[4];
	static uint8_t n;

	cputsxy(20 - strlen(name) / 2, 0, name);
	cputsxy(5, 21, info);

	// TODO add convenient functions into conio_wrapper to print lowercase
	gotox_blt(5+6); blit_char('p');
	gotox_blt(5+10); blit_char('q');
	gotox_blt(5+14); blit_char('r');
	gotox_blt(5+18); blit_char('s');
	gotox_blt(5+22); blit_char('t');

	uint8_to_a10(num_clients, tmp); cputsxy(5+2, 21, tmp);
	uint8_to_a10(body_1, tmp); cputsxy(5+7, 21, tmp);
	uint8_to_a10(body_2, tmp); cputsxy(5+11, 21, tmp);
	uint8_to_a10(body_3, tmp); cputsxy(5+15, 21, tmp);
	uint8_to_a10(body_4, tmp); cputsxy(5+19, 21, tmp);
	uint8_to_a10(body_5, tmp); cputsxy(5+23, 21, tmp);

	// TODO uint16_to_a10, make the numbers smaller as a workaround ;-)
	n = world_width / 160;
	gotoxy(5+26, 21);
	if (n < 10) cputc(' ');
	uint8_to_a10(n, tmp);
	cputs(tmp);
	cputc('x');
	n = world_height / 88;
	uint8_to_a10(n, tmp); cputs(tmp);

	cputsxy(0, 22, "F:Freeze R:Reset +-:Speed 1-5:Add Q:Quit");
	// TODO "W:Who"
}

#else

// print a uint8_t into 2 spaces
void printu8j2(uint8_t v) {
	if (v < 10) {
		cputc(v + '0');
		cputc(' ');
	} else {
		cputc(v / 10 + '0');
		cputc(v % 10 + '0');
	}
}

// print a uint16
void printu16(uint16_t v) {
	char tmp[6];

	utoa(v, tmp, 10);
	cputs(tmp);
}

void print_reverse(char *s) {
	revers(1); cputs(s); revers(0);
}

void show_info() {
	uint8_t xtra, i;

	CPUTSXY(0, 22, name);
	for (i = 0; i < name_pad; i++) {
		cputc(' ');
	}

	// cputs("C:"); printu8j2(num_clients);
	// cputs("1:"); printu8j2(body_1);
	// cputs("2:"); printu8j2(body_2);
	// cputs("3:"); printu8j2(body_3);
	// cputs("4:"); printu8j2(body_4);
	// cputs("5:"); printu8j2(body_5);
	revers(1); cputs("C:"); revers(0); printu8j2(num_clients);
	revers(1); cputs("1:"); revers(0); printu8j2(body_1);
	revers(1); cputs("2:"); revers(0); printu8j2(body_2);
	revers(1); cputs("3:"); revers(0); printu8j2(body_3);
	revers(1); cputs("4:"); revers(0); printu8j2(body_4);
	revers(1); cputs("5:"); revers(0); printu8j2(body_5);

	if (world_height > 99) {
		cputc(' ');
	}
	if (world_is_frozen) {
		revers(1);
	}
	printu16(world_width);
	cputc('x');
	printu16(world_height);
	if (world_is_frozen) {
		revers(0);
	}

	GOTOXY(0, 23);
	print_reverse("F"); cputs("rz ");
	print_reverse("R"); cputs("st ");
	print_reverse("+"); cputc('/'); print_reverse("-"); cputc(' ');
	print_reverse("1"); cputc('-'); print_reverse("5"); cputs("Add ");
	print_reverse("W"); cputs("ho ");
	print_reverse("I"); cputs("nf ");
	print_reverse("Q"); cputs("uit ");

#ifdef __ATARI__
	print_reverse("D"); cputs("rk ");
	cputc('f'); print_reverse("L");    cputs("sh");
#endif

#ifndef __APPLE2__
#endif
}
#endif // __PMD85__
