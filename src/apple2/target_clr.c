#include <conio.h>
#include <string.h>
#include <stdlib.h>

#include "double_buffer.h"

extern void wait_vsync();

void target_clr() {
	wait_vsync();
	if (is_orig_screen_mem) {
		clrscr();
	} else {
		memset((void *) 0x0800, 0xA0, 0x0400);
	}
}
