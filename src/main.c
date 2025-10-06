/**
 * bounce world client
 * 
 * by Mark Fisher (c) 2024
 *
 */

#if !defined(__PMD85__) && !defined(_CMOC_VERSION_)
#include <cc65.h>
#endif
#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#include <conio_wrapper.h>
#else
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif //_CMOC_VERSION

#include "fujinet-network.h"

#include "connection.h"
#include "data.h"
#include "hex_dump.h"
#include "get_info.h"
#include "run_simulation.h"
#include "shapes.h"
#include "shutdown.h"
#include "world.h"

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "timer.h"
#endif

extern uint16_t fn_network_bw;
extern uint8_t fn_network_conn;
extern uint8_t fn_network_error;

int main(void)
{
#ifdef __PMD85__
  reset_timer(1);
#endif
#ifdef _CMOC_VERSION_
  hirestxt_init();
#endif  

  // print app information and get the URL/name
  get_info();
  clrscr();  

  // make a persistent connection to the server
  connect_service();

  // fetch data from server for shapes, create the client and get the current world state for display
  get_shapes();
  send_client_data();
  get_world_state();

  // run it!
  run_simulation();

  // when simulation ends, clean up any screen/sound etc
  cleanup_client();

  return 0;
}
