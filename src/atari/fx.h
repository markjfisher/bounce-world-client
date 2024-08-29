#ifndef BWC_FX_H
#define BWC_FX_H

#include <stdbool.h>
#include <stdint.h>

void init_flash_vbi();
void screen_flash();
extern bool is_flashing_screen;
extern uint8_t current_flash_time;

#endif