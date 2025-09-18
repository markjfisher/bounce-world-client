#include "double_buffer.h"
#include "playfield_clr.h"
#include <video/tms99x8.h>


uint8_t is_alt_screen;
extern uint16_t _tms9918_pattern_name;

void swap_buffer()
{
  is_alt_screen = !is_alt_screen;
  if (is_alt_screen)
    _tms9918_pattern_name == 0x1c00;
  else
    _tms9918_pattern_name == 0x1800;
}

void show_other_screen()
{
  if (is_alt_screen)
    vdp_set_reg(2,7); //1c00
  else
    vdp_set_reg(2,6); //1800
}

void playfield_clr()
{
  clrscr();
}
