#include <cmoc.h>
#include <coco.h>
#include "double_buffer.h"
#include "conio_wrapper.h"
#include "screen.h"

extern void wait_vsync();

void playfield_clr() {

	for (int y = 0; y < SCREEN_HEIGHT - 2; y++)
	{
		gotoxy(0, y);
		clrtoeol();
	}
}
