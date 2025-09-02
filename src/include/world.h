#ifndef BWC_WORLD_H
#define BWC_WORLD_H

#ifndef _CMOC_VERSION_
#include <stdint.h>
#endif

extern uint16_t world_width;
extern uint16_t world_height;
extern uint8_t  num_clients;
extern uint8_t  world_is_frozen;
extern uint8_t  world_is_wrapped;
extern uint16_t body_count;
extern uint8_t  body_1;
extern uint8_t  body_2;
extern uint8_t  body_3;
extern uint8_t  body_4;
extern uint8_t  body_5;

void get_world_state();
void get_world_clients();
void get_world_cmd();
void get_broadcast();
int16_t fetch_client_state();
void create_client_data_command();

#endif // BWC_WORLD_H
