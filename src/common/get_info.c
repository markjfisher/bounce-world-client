// #include <atari.h>
#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "delay.h"
#include "get_line.h"
#include "sound.h"

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

  cputsxy(txp, yps +  9, "Bounce Server URL:");
  cursor(1);
  cputsxy(txp, yps + 10, "> ");

  // while the user is reading the message, create and clear various memory buffers
  memset(endpoint, 0, 80);
  memset(client_data_url, 0, 96);
  memset(name, 0, 9);

  get_line(endpoint, 60);

  cputsxy(txp, yps + 12, "Your name (max 8):");
  cputsxy(txp, yps + 13, "> ");  
  get_line(name, 9);
  cursor(0);

  // move it forward 3 bytes, and prepend n1:
  memmove(endpoint + 3, endpoint, 100);
  endpoint[0] = 'n';
  endpoint[1] = '1';
  endpoint[2] = ':';

}