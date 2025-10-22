#include <coco.h>
#include <cmoc.h>
#include <hirestxt.h>

#include "chardef.h"
#include "conio_wrapper.h"

/*
 * cc65 conio like, text console functions
 * on coco
 *
 */

 byte colorset = 0;

void hirestxt_init(void)
{
	byte colorset = 0x00;

	// Define a `HiResTextScreenInit` object:
	struct HiResTextScreenInit init =
		{
			42,					/* characters per row */
			writeCharAt_42cols, /* must be consistent with previous field */
			(byte *)*(byte *)0x00BC << 8,
			TRUE,		   /* redirects printf() to the 42x24 text screen */
			(word *)0x112, /* pointer to a 60 Hz async counter (Color Basic's TIMER) */
			0,			   /* default cursor blinking rate */
			NULL,		   /* use inkey(), i.e., Color Basic's INKEY$ */
			NULL,		   /* no sound on '\a' */
		};

	width(32); /* PMODE graphics will only appear from 32x16 (does nothing on CoCo 1&2) */
	pmode(4, (byte *)init.textScreenBuffer); /* hires text mode */
	pcls(255);
	screen(1, colorset); 
	initHiResTextScreen(&init);
}

void switch_colorset(void)
{
	if (colorset == 0)
	{
		colorset = 1;
	}
	else
	{
		colorset = 0;
	}

	screen(1, colorset);
}

unsigned char wherex(void)
{
	return getCursorColumn();
}

unsigned char wherey(void)
{
	return getCursorRow();
}

unsigned char cursor(unsigned char onoff)
{

	if (onoff)
	{
		animateCursor();
	}
	else
	{
		removeCursor();
	}

	return 0;
}

unsigned char revers(unsigned char onoff)
{
	setInverseVideoMode(onoff);
	return 0;
}

void chlinexy(unsigned char x, unsigned char y, unsigned char length)
{
	gotoxy(x, y);
	while (length--)
	{
		cputc(CH_HLINE);
	}
}

void cputcxy(unsigned char x, unsigned char y, char c)
{
	gotoxy(x, y);
	cputc(c);
}

void cputsxy(unsigned char x, unsigned char y, const char *s)
{
	gotoxy(x, y);
	cputs(s);
}

byte cgetc_cursor()
{
  byte shift = false;
  byte k;

  while (true)
  {
    k = waitKeyBlinkingCursor();

    if (isKeyPressed(KEY_PROBE_SHIFT, KEY_BIT_SHIFT))
    {
      shift = 0x00;
    }
    else
    {
      if (k > '@' && k < '[')
        shift = 0x20;
    }

    if (k)
      return k + shift;
  }
}

byte cgetc()
{
  byte shift = false;
  byte k;

  while (true)
  {
    k = inkey();

    if (isKeyPressed(KEY_PROBE_SHIFT, KEY_BIT_SHIFT))
    {
      shift = 0x00;
    }
    else
    {
      if (k > '@' && k < '[')
        shift = 0x20;
    }

    if (k)
      return k + shift;
  }
}

/* this is from cc65.h */

unsigned char doesclrscrafterexit(void)
{
	return 1;
}
