#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "get_line.h"

void get_info() {
  char hxp = 4;
  char txp = 3;
  char yps = 3;

  clrscr();

  chlinexy(2, yps - 1, 36);

  revers(1);
  cputsxy(hxp, yps + 1, "                                ");
  cputsxy(hxp, yps + 2, " Welcome to Bounce World Client ");
  cputsxy(hxp, yps + 3, "        By Mark Fisher          ");
  cputsxy(hxp, yps + 4, "                                ");
  revers(0);

  chlinexy(2, yps + 6, 36);

  cputsxy(txp, yps +  9, "Please enter the URL of the");
  cputsxy(txp, yps + 10, "Bounce Service:");
  cursor(1);
  cputsxy(txp, yps + 12, "> ");

  // while the user is reading the message, create and clear the memory for the url
  endpoint = malloc(128);
  heartbeat = malloc(128);
  name = malloc(64);
  memset(endpoint, 0, 128);
  memset(heartbeat, 0, 128);
  memset(name, 0, 64);

  get_line(endpoint, 100); // allow some space for the endpoint part

  cputsxy(txp, yps + 15, "Please enter your name:");
  cputsxy(txp, yps + 17, "> ");  
  get_line(name, 63); // 1 short for the nul
  cursor(0);

  // create the heartbeat endpoint, prepend n2:
  memcpy(heartbeat + 3, endpoint, 100);
  heartbeat[0] = 'n';
  heartbeat[1] = '2';
  heartbeat[2] = ':';
  
  // move it forward 3 bytes, and prepend n1:
  memmove(endpoint + 3, endpoint, 100);
  endpoint[0] = 'n';
  endpoint[1] = '1';
  endpoint[2] = ':';
}