#if defined(__CC65__)

#include <conio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "get_line.h"

void get_line(char* buf, uint8_t max_len) {
	uint8_t c;
	uint8_t i = 0;
	uint8_t start_x = wherex();
	memset(buf, 0, max_len);

	cursor(1);

	do {
		c = cgetc();

		if (isprint(c)) {
			gotox(start_x + i);
			if (i == (max_len - 1)) {
				// we're at the end, set reverse and invisible cursor
				// so it looks like the cursor is editing the current last character
				revers(1);
				cursor(0);
			} else {
				revers(0);
				cursor(1);
			}

			cputc(c);

			buf[i] = c;
			if (i < max_len - 1) {
				i++;
			}
		}
		else if ((c == CH_CURS_LEFT) || (c == CH_DEL)) {
			if (i) {
				uint8_t cur_x = wherex();

				// if we're at the end, i is max_len - 1 but we need to check if the
				// buffer has a char there before deciding which char to remove
				if (i == (max_len - 1) && buf[i] != '\0') {
					// we need to erase the last char from the buffer but not "move" the cursor
					buf[i] = '\0';
					gotox(cur_x - 1);
					revers(0);
					cursor(1);
					cputc(' ');
					gotox(cur_x - 1);
				} else {
					// we weren't on a full buffer, so do simple erase
					gotox(cur_x - 1);
					cputc(' ');
					gotox(cur_x - 1);
					--i;
					buf[i] = '\0';
				}

			}
		}

	} while (c != CH_ENTER);

	// in case last character printed left us in revers mode, undo it, and write out the final string so the inverse char is removed.
	cursor(0);
	revers(0);
	gotox(start_x);
	cputs(buf);

	cursor(0);
}
#endif