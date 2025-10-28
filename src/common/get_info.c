// #include <atari.h>
#ifndef _CMOC_VERSION_
#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#include "conio_wrapper.h"
#include "strncasecmp.h"
#endif

#include "data.h"
#include "delay.h"
#include "get_line.h"
#include "sound.h"
#include "appkey.h"
#include "press_key.h"

#ifdef __PMD85__
#include "conio_wrapper.h"
#endif

#ifndef ENDPOINT_URL
#define ENDPOINT_URL ""
//#define ENDPOINT_URL "tcp://localhost:9002"
#endif

char endpoint_input[61];
char *protocol = "n1:";

char *version = "2.0.0";

void get_server(unsigned char x, unsigned char y, char *serverbuf)
{
  memset(serverbuf, ' ', 60);
  serverbuf[60] = 0;
  cputsxy(x,y, serverbuf);
  memset(serverbuf, 0, 60);
  gotoxy(x,y);
  cursor(1);
#ifdef __PMD85__
    get_line(endpoint_input, 33);
#else  
  get_line(endpoint_input, 60);
#endif
  cursor(0);
}

void get_name(unsigned char x, unsigned char y, char *namebuf)
{
  memset(namebuf, ' ', 8);
  namebuf[8] = 0;
  cputsxy(x,y, namebuf);
  memset(namebuf, 0, 8);
  gotoxy(x,y);
  cursor(1);
  get_line(namebuf, 8);
  cursor(0);
  
}

void get_info_menu(char x, char y)
{
  char c = 0;

  cursor(0);
  chlinexy(x + 3, 20, 28);
  gotoxy(x + 4, 21);
  cputs("Change ");
  revers(1);
  cputs("S");
  revers(0);
  cputs("erver ");
  cputs("Change ");
  revers(1);
  cputs("N");
  revers(0);
  cputs("ame");
  revers(1);
  gotoxy(x + 5, 22);
  cputs("Press a key to continue");
  revers(0);
  chlinexy(x + 3, 23, 28);

  while (true)
  {
#ifdef _CMOC_VERSION_
    while (c == 0)
    {
      c = (char)kbhit();
    }
#else
    while (kbhit() == 0)
    {
      pause(20);
    }
    c = cgetc();
#endif

    switch (c)
    {
    case 'S':
    case 's':
      get_server(x + 2, y + 11, endpoint_input);
      if (strlen(endpoint_input) > 0) 
      {
        write_endpoint_appkey(endpoint_input);
      }
      c=0;
      break;
    case 'N':
    case 'n':
      get_name(x + 2, y + 14, name);
      if (strlen(name) > 0)
      {
        write_name_appkey(name);
      }
      c=0;
      break;
    default:
      return;
    }
  }
}

void get_info()
{

#ifdef _CMOC_VERSION_
  char hxp = 5;
  char txp = 4;
#else
  char hxp = 4;
  char txp = 3;
#endif 
  char yps = 3;

  clrscr();
  init_sound();

  chlinexy(hxp-2, yps - 1, 36);
  revers(1);
  cputsxy(hxp, yps + 1, "                                ");
  cputsxy(hxp, yps + 2, " Welcome to Bouncy World Client ");
  cputsxy(hxp, yps + 3, "        By Mark Fisher          ");

#ifdef __PMD85__
  cputsxy(hxp, yps + 4, "   PMD 85 version by Jan Krupa  ");
  cputsxy(hxp, yps + 5, "                                ");
  revers(0);
  cputsxy(hxp, yps + 6, "                Version: 0.0.0  ");
  cputsxy(hxp + 25, yps + 6, version);

  chlinexy(hxp-2, yps + 8, 36);
#elif defined(_CMOC_VERSION_)
  cputsxy(hxp, yps + 4, "  CoCo version by Rich Stephens ");
  cputsxy(hxp, yps + 5, "                                ");
  revers(0);
  cputsxy(hxp, yps + 6, "                Version: 0.0.0  ");
  cputsxy(hxp + 25, yps + 6, version);

  chlinexy(hxp-2, yps + 8, 36);
#else
  cputsxy(hxp, yps + 4, "                                ");
  revers(0);
  cputsxy(hxp, yps + 5, "                Version: 0.0.0  ");
  cputsxy(hxp + 25, yps + 5, version);

  chlinexy(hxp-2, yps + 7, 36);
#endif

  memset(app_data, 0, 80);
  memset(name, 0, 9);

  cputsxy(txp, yps + 10, "Bounce Server URL:");

#ifndef _CMOC_VERSION_
  cursor(1);
#endif

  cputsxy(txp, yps + 11, "> ");
  memset(app_data, 0, 80);

  // Try to read the endpoint from the app key
  if (read_endpoint_appkey(endpoint_input) == true)
  {
    cputsxy(txp + 2, yps + 11, endpoint_input);
  }
  else
  {
    if (strlen(ENDPOINT_URL) != 0)
    {
      strcpy(endpoint_input, ENDPOINT_URL);
    }
    else
    {
      get_server(txp + 2, yps + 11, endpoint_input);
    }
    if (strlen(endpoint_input) > 0)
    {
      write_endpoint_appkey(endpoint_input);
    }
  }

  if (strncasecmp(endpoint_input, "tcp", 3) != 0)
  {
    strcat(app_data, "tcp://");
  }
  strcat(app_data, endpoint_input);

  cputsxy(txp, yps + 13, "Your name (max 8):");
  cputsxy(txp, yps + 14, "> ");

  if (read_name_appkey(name) == true)
  {
    cputsxy(txp +2, yps + 14, name);
    get_info_menu(txp, yps);
  }
  else
  {
    get_name(txp + 2, yps + 14, name);
    if (strlen(name) > 0)
    {
      write_name_appkey(name);
    }
  }
  name_pad = 9 - strlen(name); // pre-calculate this so it isn't constantly done in loops
  cursor(0);

  // move it forward 3 bytes, and prepend n1:
  memmove(app_data + 3, app_data, 76);
  memcpy(app_data, protocol, 3);

  memset(server_url, 0, 80);
	// copy from the app_data buffer into the server_url
	strcpy(server_url, (char *) app_data);
}
