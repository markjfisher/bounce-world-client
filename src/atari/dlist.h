#ifndef BWC_DLIST_H
#define BWC_DLIST_H

#include <stdint.h>

uint8_t *get_dlist_screen_ptr();
void setup_dli();
void dli(void);
void enable_dli(void);

extern uint8_t txt_c1;
extern uint8_t txt_c2;

#define INIT_COLOUR_1   (0x94)
#define INIT_COLOUR_2   (0x28)

#endif // BWC_DLIST_H