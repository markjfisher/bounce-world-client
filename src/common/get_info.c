// #include <atari.h>
#if !defined (_CMOC_VERSION_)
#if !defined(__ADAM__)
#include <conio.h>
#endif
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
#elif defined __ADAM__
#include "conio_helpers.h"
#endif

char endpoint_input[61];
char *protocol = "n1:";

char *version = "2.0.3";

#ifdef _CMOC_VERSION_
char hxp = 5;
char txp = 4;
#elif defined(__ADAM__)
char hxp = 0;
char txp = 2;
#else
char hxp = 4;
char txp = 3;
#endif 
char yps = 3;

void clear_cursor() {
  // a touch hacky, but forces cursor off after printing a char
  cursor(0);
  cputcxy(0, 0, ' ');
}

void get_input(uint8_t x, uint8_t y, uint8_t len, char *s) {
  memset(s, ' ', len);
  cputsxy(x, y, s);
  *s = '\0';
  gotoxy(x, y);
  cursor(1);
#ifdef __PMD85__
  get_line(s, 33);
#else  
  get_line(s, len);
#endif
  clear_cursor();

}

void get_info_changes()
{
  char c;

  // keep looping until they hit non NnSs key
  while (true) {
    c = 0;

#ifdef _CMOC_VERSION_
    while (c == 0)
    {
      c = (char)kbhit();
    }
#elif defined (USE_PLATFORM_SPECIFIC_INPUT)
    while(c <= 0) 
    {
      c = getPlatformKey();
    }
#else
    while (kbhit() == 0) ;
    c = cgetc();
#endif
    switch (c) {

      case 'S':
      case 's':
        get_input(txp + 2, yps + 11, ENDPOINT_LEN, endpoint_input);
        write_appkey(endpoint_input, APP_KEY_ENDPOINT);
        break;

      case 'N':
      case 'n':
        get_input(txp + 2, yps + 14, NAME_LEN, name);
        write_appkey(name, APP_KEY_NAME);
        break;

      default:
        // only exit if both server and name are set
        if (strlen(endpoint_input) > 0 && strlen(name) > 0) {
          return;
        }
        break;

    }
  }
}

void show_header() {
  clrscr();
  init_sound();
#ifdef __ADAM__
  chlinexy(hxp+1, yps - 1, 30);
#else
  chlinexy(hxp-2, yps - 1, 36);
#endif
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
#elif defined __ADAM__
  cputsxy(hxp, yps + 4, " ADAM version by Geoff Oltmans" );
  revers(0);
  cputsxy(hxp, yps + 5, "        Version: 0.0.0          ");
  cputsxy(hxp + 17, yps + 5, version);

  chlinexy(hxp+1, yps + 7, 30);

#else
  cputsxy(hxp, yps + 4, "                                ");
  revers(0);
  cputsxy(hxp, yps + 5, "                Version: 0.0.0  ");
  cputsxy(hxp + 25, yps + 5, version);

  chlinexy(hxp-2, yps + 7, 36);
#endif

}

void get_default_server(char *s) {
  bool r = read_appkey(s, ENDPOINT_LEN, APP_KEY_ENDPOINT);
  if (!r) {
    *s = '\0';
  }
}

void get_default_name(char *s) {
  bool r = read_appkey(s, NAME_LEN, APP_KEY_NAME);
  if (!r) {
    *s = '\0';
  }
}

void show_server(char *s) {
  cputsxy(txp, yps + 10, "Bounce Server URL:");
  cputsxy(txp, yps + 11, "> ");
  cputsxy(txp + 2, yps + 11, s);
}

void show_name(char *s) {
  cputsxy(txp, yps + 13, "Your name (max 8):");
  cputsxy(txp, yps + 14, "> ");
  cputsxy(txp +2, yps + 14, s);
}

void cput_rev1(char *s) {
  revers(1);
  cputc(s[0]);
  revers(0);
  cputs(&s[1]);  
}

void show_menu() {
#if defined (__ADAM__)
  chlinexy(txp, 20, 28);
#else
  chlinexy(txp + 3, 20, 28);
#endif
  cputsxy(txp + 4, 21, "Change ");
  cput_rev1("Server ");

  cputs("Change ");
  cput_rev1("Name");

  revers(1);
  gotoxy(txp + 5, 22);
  cputs("Press a key to continue");
  revers(0);

#if defined (__ADAM__)
  chlinexy(txp, 23, 28);
#else
  chlinexy(txp + 3, 23, 28);
#endif
}

void get_info()
{
  memset(app_data, 0, 80);
  memset(name, 0, 9);

  show_header();
  
  get_default_server(endpoint_input);
  show_server(endpoint_input);
  
  get_default_name(name);
  show_name(name);

  show_menu();

  get_info_changes();
  clear_cursor();

  if (strncasecmp(endpoint_input, "tcp", 3) != 0)
  {
    strcat(app_data, "tcp://");
  }
  strcat(app_data, endpoint_input);

  // pre-calculate this so it isn't constantly done in loops
  name_pad = 9 - strlen(name);

  // move it forward 3 bytes, and prepend n1:
  memmove(app_data + 3, app_data, 76);
  memcpy(app_data, protocol, 3);

  memset(server_url, 0, 80);
	// copy from the app_data buffer into the server_url
	strcpy(server_url, (char *) app_data);
}
