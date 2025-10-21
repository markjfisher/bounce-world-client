#include <cmoc.h>
#include <coco.h>
#include "chardef.h"
#include "conio_wrapper.h"

#include "get_line.h"

void get_line(char *buf, uint8_t max_len)
{
	uint8_t c;
	uint16_t i = 0;
	uint8_t init_x = wherex();

	do
	{
		gotox(i + init_x);
		cursor(1);
		c = cgetc_cursor();

		if (isprint(c))
		{
			putchar(c);
			buf[i] = c;
			if (i < max_len - 1)
				i++;
		}
		else if (c == KEY_LEFT_ARROW)
		{
			if (i)
			{
				putchar(KEY_LEFT_ARROW);
				putchar(' ');
				putchar(KEY_LEFT_ARROW);
				--i;
			}
		}
	} while (c != KEY_ENTER);
	putchar('\n');
	buf[i] = '\0';

	cursor(0);
}
