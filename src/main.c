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
#include "hex_dump.h"
#include "get_info.h"
#include "run_simulation.h"
#include "shapes.h"
#include "shutdown.h"
#include "simulator.h"
#include "world.h"

extern uint16_t fn_network_bw;
extern uint8_t fn_network_conn;
extern uint8_t fn_network_error;

int main(void)
{
  // print app information and get the URL/name
  get_info();

  clrscr();

  // fetch data from server for shapes, create the client and get the current world state for display
  get_shapes();

#ifndef SIMULATE_FN
  connect_service();
#endif

  get_world_state();

  // run it!
  run_simulation();

  // when simulation ends, clean up any screen/sound etc
  cleanup_client();

  return 0;
}
