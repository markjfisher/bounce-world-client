#ifndef BWC_DOUBLE_BUFFER_H
#define BWC_DOUBLE_BUFFER_H

#include <stdint.h>

// double buffering macros, apple2 needs a hand.
#ifdef __APPLE2__
#include "conio_helpers.h"
#define GOTOXY gotoxy_buffer
#define CPUTSXY cputsxy_buffer
#else
#define GOTOXY gotoxy
#define CPUTSXY cputsxy
#endif

extern void swap_buffer();
extern uint8_t is_orig_screen_mem;
extern void show_other_screen();

#ifdef __ATARI__
// extern char display_buffer[960];
extern char *dlist_scr_ptr;
extern char *screen_mem_orig;
#endif

#ifdef __APPLE2__
extern void check_text_buffer_location();
#endif


#endif // BWC_DOUBLE_BUFFER_H