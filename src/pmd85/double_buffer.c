#include "shapes.h"
#include "text_buffer.h"
#include "shape_util.h"
#include "double_buffer.h"

#include "conio_wrapper.h"
#include "itoa_wrapper.h"
#include "timer.h"
static unsigned int t1;
static unsigned int t2;
static char tmp[4];

uint8_t is_alt_screen;

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;
} DirtyRect;

#define MAX_DIRTY_RECTS		52 // 50 shapes + who (+ breadcast?)
uint8_t dirty_rectangles[1 + MAX_DIRTY_RECTS * sizeof(DirtyRect)] = {0};
uint8_t *dirty_ptr = dirty_rectangles + 1;


void swap_buffer() {
	reset_dirty();
}

void show_other_screen() {

	// Render text buffer to screen
// t1 = read_timer(1);
	show_text_buffer();
// t2 = read_timer(1);
// uint8_to_a16((t1-t2)>>8, tmp);
// cputsxy(38, 0, tmp);

	// Get ready for next frame
// t1 = read_timer(1);
	clear_dirty();
// t2 = read_timer(1);
// uint8_to_a16((t1-t2)>>8, tmp);
// cputsxy(38, 1, tmp);
}

void reset_dirty() {
	// Reset aux buffer
	dirty_rectangles[0] = 0;
	dirty_ptr  = dirty_rectangles + 1;
}

// TODO: ASM version
void clear_dirty() {
	DirtyRect *aux;
	uint8_t count;
	// Clear dirty rectangles in text buffer
	dirty_ptr = dirty_rectangles + 1;
	count = dirty_rectangles[0];
//cputsxy(0, 23, "                                   ");
	while(count--) {
//t1 = read_timer(1);
		aux = (DirtyRect *)dirty_ptr;
		gotoxy_tb(aux->x, aux->y);
		clear_rect_tb(aux->w, aux->h);
		dirty_ptr += sizeof(DirtyRect);
//t2 = read_timer(1);
//uint8_to_a10((t1-t2)>>8, tmp);
//cputsxy(count*4, 23, tmp);
	}
	reset_dirty();
}