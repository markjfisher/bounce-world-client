#include <conio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "get_line.h"

// This function adds 900 bytes to the app. The atari CIO routine is much cleaner and handles cursor movement better, so this
// code is only used for systems without an OS based read line ability that we want to use

void get_line(char* buf, uint8_t max_len) {
	uint8_t c;
	uint16_t i = 0;

	cursor(1);

	do {
		c = cgetc();

		if (isprint(c)) {
			putchar(c);
			buf[i] = c;
			if (i < max_len - 1) i++;
		}
		else if ((c == CH_CURS_LEFT) || (c == CH_DEL)) {
			if (i) {
				putchar(CH_CURS_LEFT);
				putchar(' ');
				putchar(CH_CURS_LEFT);
				--i;
			}
		}
	} while (c != CH_ENTER);
	putchar('\n');
	buf[i] = '\0';

	cursor(0);
}
