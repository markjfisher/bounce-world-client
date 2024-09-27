// #include <atari.h>
#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "delay.h"
#include "get_line.h"
#include "sound.h"

#ifndef ENDPOINT_URL
#define ENDPOINT_URL ""
#endif

char endpoint_input[61];
char *protocol = "n1:";

void get_info() {
  char hxp = 4;
  char txp = 3;
  char yps = 3;

  clrscr();
  init_sound();
  chlinexy(2, yps - 1, 36);

  revers(1);
  cputsxy(hxp, yps + 1, "                                ");
  cputsxy(hxp, yps + 2, " Welcome to Bounce World Client ");
  cputsxy(hxp, yps + 3, "        By Mark Fisher          ");
  cputsxy(hxp, yps + 4, "                                ");
  revers(0);

  chlinexy(2, yps + 6, 36);

  memset(endpoint, 0, 80);
  memset(client_data_url, 0, 96);
  memset(name, 0, 9);

  cputsxy(txp, yps +  9, "Bounce Server URL:");

  cursor(1);
  if (strlen(ENDPOINT_URL) == 0) {
    cputsxy(txp, yps + 10, "> ");
    memset(endpoint_input, 0, 60);
    get_line(endpoint_input, 60);
    if (strncasecmp(endpoint_input, "http", 4) != 0) {
      strcat(endpoint, "http://");
    }
    strcat(endpoint, endpoint_input);
  } else {
    strcpy(endpoint, ENDPOINT_URL);
    cputsxy(txp, yps + 10, "> ");
    cputs(endpoint);
  }

  cputsxy(txp, yps + 12, "Your name (max 8):");
  cputsxy(txp, yps + 13, "> ");  
  get_line(name, 9);
  cursor(0);

  // move it forward 3 bytes, and prepend n1:
  memmove(endpoint + 3, endpoint, 76);
  memcpy(endpoint, protocol, 3);
}