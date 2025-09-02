#ifndef BWC_DISPLAY_H
#define BWC_DISPLAY_H

#ifndef _CMOC_VERSION_
#include <stdint.h>
#endif

void show_screen();
void show_info();
#ifdef __PMD85__
void clear_info();
void draw_border();
#endif
void init_screen();
void set_screen_colours();

#endif // BWC_DISPLAY_H
