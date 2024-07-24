/**
 * bounce world client
 * 
 * by Mark Fisher (c) 2024
 *
 */

#include <cc65.h>
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-network.h"

#include "buffer.h"
#include "getLine.h"
#include "main.h"
#include "shapes.h"

extern uint16_t fn_network_bw;
extern uint8_t fn_network_conn;
extern uint8_t fn_network_error;

char *endpoint;

int main(void)
{
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

  cputsxy(txp, yps +  9, " Please enter the URL of the");
  cputsxy(txp, yps + 10, " Bounce Service:");
  cursor(1);
  cputsxy(txp, yps + 14, "> ");

  // while the user is reading the message, create and clear the memory for the url
  endpoint = malloc(128);
  memset(endpoint, 0, 128);

  get_line(endpoint, 128);
  cursor(0);

  // move it forward 2 bytes, and shove in the N:
  memmove(endpoint+2, endpoint, 125);
  endpoint[0] = 'n';
  endpoint[1] = ':';
  endpoint[127] = '\0';

  clrscr();
  getShapes(endpoint);
  free(endpoint);

  cgetc();
  return 0;
}
