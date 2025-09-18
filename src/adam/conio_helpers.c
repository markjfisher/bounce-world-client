#include <stdint.h>
#include "chardef.h"
#include <video/tms99x8.h>
#include "conio_helpers.h"
#include <sys/ioctl.h>
#include <interrupt.h>

#ifdef __ADAM__

extern uint8_t is_alt_screen;

//reaching around the stdio/conio info in z88dk here...
extern char _console_x;
extern char _console_y;

unsigned int currentTime;

void counterInt(void);

void init_vdp()
{
  //set up the vdp vram with tile data, both tile and color... change to mode 1 first thing.
  //change background to black...
  int mode = 1;
  uint8_t chr = 0;
  console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
  void *param = &bounce_udgs;
  console_ioctl(IOCTL_GENCON_SET_UDGS,&param);
  vdp_color(VDP_INK_WHITE,VDP_INK_BLACK,VDP_INK_BLACK);
  add_raster_int(counterInt);

  //doesn't have to do with the video in any case, but a helpful place to stash it...
  initPlatformKeyboardInput();
}

void cputsxy(uint8_t x, uint8_t y, char *s) {
  uint16_t vramaddr;
  uint16_t count = strlen(s);
  _console_x = x;
  _console_y = y;

  if (count == 0 ) return;

  if (is_alt_screen)
    vramaddr = 0x1c00;
  else
    vramaddr = 0x1800; //base address for name table in mode 1
  
  vramaddr += (_console_y << 5) + _console_x;

  if ((vramaddr + count) > 0x1800+768) return;
  
  vdp_vwrite(s, vramaddr,count);
  //update x/y
  _console_y += (count + _console_x) / 32;
  _console_x = (_console_x + count) % 32;
}


void cputcxy(uint8_t x, uint8_t y, char c) {
  _console_x = x;
  _console_y = y;
  cputc(c);
}

int cputc(char c)
{
  uint16_t vramaddr;
  char temp = c;

  if (is_alt_screen)
    vramaddr = 0x1c00;
  else
    vramaddr = 0x1800; //base address for name table in mode 1

  vramaddr += (_console_y << 5) + _console_x;
  vdp_vwrite(&temp, vramaddr,1);

  //update x/y
  if(_console_x == 31)
  {
    _console_x = 0;
    _console_y++;
  } else 
    _console_x++;
  if (_console_y > 23) _console_y = 0; //wrap the display


  return 1;
}

void gotox(unsigned char x)
{
  _console_x = x;
}

unsigned int wherex(void)
{
  return _console_x;
}

unsigned int wherey(void)
{
  return _console_y;
}

void gotoxy(unsigned int x, unsigned int y)
{
  _console_x = x;
  _console_y = y;
}

void putch(char c)
{
  cputc(c);
}

unsigned char doesclrscrafterexit(void) {
	return 1;
}

void chlinexy(unsigned char x, unsigned char y, unsigned char length) {
  _console_x = x;
  _console_y = y;
  while(length--)
		cputc(CH_HLINE);
}

unsigned char revers(unsigned char onoff)
{

	return 0;
}

void clrscr(void)
{
  uint16_t vramaddr;

  if (is_alt_screen)
    vramaddr = 0x1c00;
  else
    vramaddr = 0x1800; //base address for name table in mode 1

  vdp_vfill(vramaddr,0,0x300);
  _console_x = 0;
  _console_y = 0;

}

void counterInt(void)
{
    currentTime++;
}

#endif
