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

#include "connection.h"
#include "data.h"
#include "get_line.h"
#include "main.h"
#include "shapes.h"

extern uint16_t fn_network_bw;
extern uint8_t fn_network_conn;
extern uint8_t fn_network_error;

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

  cputsxy(txp, yps +  9, "Please enter the URL of the");
  cputsxy(txp, yps + 10, "Bounce Service:");
  cursor(1);
  cputsxy(txp, yps + 12, "> ");

  // while the user is reading the message, create and clear the memory for the url
  endpoint = malloc(128);
  heartbeat = malloc(128);
  memset(endpoint, 0, 128);
  memset(heartbeat, 0, 128);
  name = malloc(64);
  memset(name, 0, 64);

  get_line(endpoint, 100); // allow some space for the endpoint part

  cputsxy(txp, yps + 15, "Please enter your name:");
  cputsxy(txp, yps + 17, "> ");  
  get_line(name, 64);
  cursor(0);

  // create the heartbeat endpoint, prepend n2:
  memcpy(heartbeat + 3, endpoint, 100);
  heartbeat[0] = 'n';
  heartbeat[1] = '2';
  heartbeat[2] = ':';
  heartbeat[127] = '\0';
  
  // move it forward 3 bytes, and prepend n1:
  memmove(endpoint + 3, endpoint, 100);
  endpoint[0] = 'n';
  endpoint[1] = '1';
  endpoint[2] = ':';
  endpoint[127] = '\0';

  clrscr();
  getShapes();

  connect_service();


  cgetc();

  free(endpoint);
  free(heartbeat);
  free(name);
  return 0;
}
