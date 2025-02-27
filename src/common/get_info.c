// #include <atari.h>
#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "delay.h"
#include "get_line.h"
#include "sound.h"

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

void get_info() {
  char hxp = 4;
  char txp = 3;
  char yps = 3;

  clrscr();
  init_sound();
  chlinexy(2, yps - 1, 36);

  revers(1);
  cputsxy(hxp, yps + 1, "                                ");

#ifdef __APPLE2__
  // can't easily have apple2 target with lower case inverse text without doing hacks, so just do upper case
  cputsxy(hxp, yps + 2, " WELCOME TO BOUNCY WORLD CLIENT ");
  cputsxy(hxp, yps + 3, "        BY MARK FISHER          ");
#else
  cputsxy(hxp, yps + 2, " Welcome to Bouncy World Client ");
  cputsxy(hxp, yps + 3, "        By Mark Fisher          ");
#endif

#ifdef __PMD85__
  cputsxy(hxp, yps + 4, "   PMD 85 version by Jan Krupa  ");
  cputsxy(hxp, yps + 5, "                                ");
  revers(0);
  cputsxy(hxp, yps + 6, "                 Version 0.0.0  ");
  cputsxy(hxp + 25, yps + 6, version);

  chlinexy(2, yps + 8, 36);
#else
  cputsxy(hxp, yps + 4, "                                ");
  revers(0);
  cputsxy(hxp, yps + 5, "                  Version: 0.0.0");
  cputsxy(hxp + 27, yps + 5, version);

  chlinexy(2, yps + 7, 36);
#endif

  memset(app_data, 0, 80);
  memset(name, 0, 9);

  cputsxy(txp, yps +  10, "Bounce Server URL:");

  cursor(1);
  if (strlen(ENDPOINT_URL) == 0) {
    cputsxy(txp, yps + 11, "> ");
    memset(app_data, 0, 60);
#ifdef __PMD85__
    get_line(endpoint_input, 33);
#else
    get_line(endpoint_input, 60);
#endif
    if (strncasecmp(endpoint_input, "tcp", 3) != 0) {
      strcat(app_data, "tcp://");
    }
    strcat(app_data, endpoint_input);
  } else {
    memset(app_data, 0, 256);
    strcpy(app_data, ENDPOINT_URL);
    cputsxy(txp, yps + 11, "> ");
    cputs(app_data);
  }

  cputsxy(txp, yps + 13, "Your name (max 8):");
  cputsxy(txp, yps + 14, "> ");  
  get_line(name, 9);
  name_pad = 9 - strlen(name);    // pre-calculate this so it isn't constantly done in loops
  cursor(0);

  // move it forward 3 bytes, and prepend n1:
  memmove(app_data + 3, app_data, 76);
  memcpy(app_data, protocol, 3);

  memset(server_url, 0, 80);
	// copy from the app_data buffer into the server_url
	strcpy(server_url, (char *) app_data);

}