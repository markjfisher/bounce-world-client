#include <cmoc.h>
#include <coco.h>
#include "double_buffer.h"
#include "conio_wrapper.h"
#include "screen.h"

extern void wait_vsync();

void playfield_clr() {
	clrscrn(SCREEN_HEIGHT -2);
}
