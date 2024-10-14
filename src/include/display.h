#ifndef BWC_DISPLAY_H
#define BWC_DISPLAY_H

#include <stdint.h>

void show_screen();
void show_info();
#ifdef __PMD85__
void clear_info();
void draw_border();
#endif
void init_screen();
void set_screen_colours();

#endif // BWC_DISPLAY_H