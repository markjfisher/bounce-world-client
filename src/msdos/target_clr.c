#include <stdint.h>

#include <conio.h>
#include "data.h"
#include "screen.h"
#include "playfield_clr.h"

void playfield_clr(void)
{
    uint8_t bottom_row;

    if (is_showing_info) {
        bottom_row = (uint8_t)(SCREEN_HEIGHT - 3);
    } else {
        bottom_row = (uint8_t)(SCREEN_HEIGHT - 1);
    }

    fill_rows(0, bottom_row, bwc_normal_attr());
    gotoxy(0, 0);
}
