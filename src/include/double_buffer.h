#ifndef BWC_DOUBLE_BUFFER_H
#define BWC_DOUBLE_BUFFER_H

#include <stdint.h>

extern void swap_buffer();
extern uint8_t is_orig_screen_mem;
extern void show_other_screen();

#ifdef __ATARI__
#define SAVMSC_P ((uint8_t*) 0x0058)
extern char d1[960];
extern char *dlist_scr_ptr;
extern char *screen_mem_orig;
#endif

#ifdef __APPLE2__
extern void check_text_buffer_location();
#endif


#endif // BWC_DOUBLE_BUFFER_H