#ifndef BWC_WORLD_H
#define BWC_WORLD_H

#include <stdint.h>

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

#endif // BWC_WORLD_H