#include <conio.h>
#include <stdint.h>

#ifdef __APPLE2__

#include "double_buffer.h"

void gotoxy_buffer(uint8_t x, uint8_t y) {
	gotoxy(x, y);
	check_text_buffer_location();
}

void cputsxy_buffer(uint8_t x, uint8_t y, char *s) {
	gotoxy_buffer(x, y);
	cputs(s);
}

void cputcxy_buffer(uint8_t x, uint8_t y, char c) {
	gotoxy_buffer(x, y);
	cputc(c);
}


#endif
