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

#include "main.h"
#include "shapes.h"

extern uint16_t fn_network_bw;
extern uint8_t fn_network_conn;
extern uint8_t fn_network_error;

int main(void)
{
  clrscr();
  getShapes("n:http://localhost:8080");

  cgetc();

  return 0;
}
