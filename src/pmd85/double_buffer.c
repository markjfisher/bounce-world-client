#include "shapes.h"
#include "text_buffer.h"
#include "shape_util.h"
#include "double_buffer.h"

uint8_t is_alt_screen;

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;
} DirtyRect;

#define MAX_DIRTY_RECTS		52 // 50 shapes + who (+ breadcast?)
static uint8_t dirty_rectangles[1 + MAX_DIRTY_RECTS * sizeof(DirtyRect)] = {0};
static uint8_t *dirty_ptr = dirty_rectangles + 1;


void swap_buffer() {
	reset_dirty();
}

void show_other_screen() {

	// Render text buffer to screen
	show_text_buffer();

	// Get ready for next frame
	clear_dirty();
}

void reset_dirty() {
	// Reset aux buffer
	dirty_rectangles[0] = 0;
	dirty_ptr  = dirty_rectangles + 1;
}

void add_dirty_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	DirtyRect *aux;
	aux = (DirtyRect *)dirty_ptr;

	if (dirty_rectangles[0] >= MAX_DIRTY_RECTS) return; // buffer is full

	aux->x = x;
	aux->y = y;
	aux->w = w;
	aux->h = h;
	dirty_ptr += sizeof(DirtyRect);
	dirty_rectangles[0]++;
}

void clear_dirty() {
	DirtyRect *aux;
	uint8_t count;
	// Clear dirty rectangles in text buffer
	dirty_ptr = dirty_rectangles + 1;
	count = dirty_rectangles[0];
	while(count--) {
		aux = (DirtyRect *)dirty_ptr;
		gotoxy_tb(aux->x, aux->y);
		clear_rect_tb(aux->w, aux->h);
		dirty_ptr += sizeof(DirtyRect);
	}
	reset_dirty();
}