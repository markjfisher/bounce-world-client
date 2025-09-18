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
#if defined (USE_PLATFORM_SPECIFIC_INPUT)
    while(c == 0) 
    {
      c = getPlatformKey();
    }
#else
		c = cgetc();
#endif


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

				gotox(cur_x - 1);
				if (i == (max_len - 1) && buf[i] != '\0') {
					// don't "move" the cursor, but do set back to normal text
					revers(0);
					cursor(1);
				} else {
					// we weren't on a full buffer, ok to move
					--i;
				}
				cputc(' ');
				gotox(cur_x - 1);
				
				buf[i] = '\0';
			}
		}

	} while (c != CH_ENTER);

	// in case last character printed left us in revers mode, undo it, and write out the final string so the inverse char is removed.
	cursor(0);
	revers(0);
	gotox(start_x);
	cputs(buf);

	cursor(0);

	//// Alternate version just using cgets() with Oliver's changes to handle CH_DEL:
	// cgets(buf, max_len+1);
}
#endif