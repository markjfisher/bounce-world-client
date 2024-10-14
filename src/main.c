/**
 * bounce world client
 * 
 * by Mark Fisher (c) 2024
 *
 */

#ifndef __PMD85__
#include <cc65.h>
#endif
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-network.h"

#include "connection.h"
#include "data.h"
#include "hex_dump.h"
#include "get_info.h"
#include "run_simulation.h"
#include "shapes.h"
#include "shutdown.h"
#include "world.h"

#ifdef __APPLE2__
#include <peekpoke.h>
#endif

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "timer.h"
#endif

extern uint16_t fn_network_bw;
extern uint8_t fn_network_conn;
extern uint8_t fn_network_error;

int main(void)
{
#ifdef __APPLE2__
  // get lowercase and mouse text in early
	allow_lowercase(true);
	POKE(0xC00F, 0);
#endif

#ifdef __PMD85__
  reset_timer(1);
#endif

  // print app information and get the URL/name
  get_info();
  clrscr();

  // fetch data from server for shapes, create the client and get the current world state for display
  get_shapes();
  connect_service();
  get_world_state();

  // run it!
  run_simulation();

  // when simulation ends, clean up any screen/sound etc
  cleanup_client();

  return 0;
}
