#ifndef BWC_DOUBLE_BUFFER_H
#define BWC_DOUBLE_BUFFER_H

#ifndef _CMOC_VERSION_
#include <stdint.h>
#endif

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
extern uint8_t is_alt_screen;
extern void show_other_screen();

#ifdef __ATARI__
// extern char display_buffer[960];
extern char *dlist_scr_ptr;
extern char *screen_mem_orig;
#endif

#ifdef __APPLE2__
extern void check_text_buffer_location();
#endif

#ifdef __PMD85__
extern void reset_dirty();
extern void clear_dirty();
extern void __CALLEE__ add_dirty_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
#endif

#endif // BWC_DOUBLE_BUFFER_H
