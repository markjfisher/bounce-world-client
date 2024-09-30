#include <conio.h>
#include <string.h>
#include <stdlib.h>

#include "double_buffer.h"

void full_clr() {
	if (is_alt_screen) {
		memset((void *) 0x0800, 0xA0, 0x0400);
	} else {
		clrscr();
	}
}
