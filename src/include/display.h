#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_positions();

#ifdef __ATARI__
extern char *dlist_scr_ptr;
extern char *screen_mem_orig;
// extern char *current_screen_mem;
extern uint8_t is_orig_screen_mem;


#endif


#endif // DISPLAY_H