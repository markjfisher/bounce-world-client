#ifndef BWC_SIMULATOR_H
#define BWC_SIMULATOR_H

#include <stdint.h>

#define DATA_SIZE 10
#define NUM_ENTRIES 130

extern uint8_t user_data[NUM_ENTRIES][DATA_SIZE];
extern uint8_t shape_data[27];
extern uint8_t ws_1[14];
extern uint8_t cmd_data[2][1];

void create_mock_shapes();
void simulate_get_world_state();
void simulate_get_world_clients();
void simulate_get_broadcast();
uint8_t simulate_get_world_cmd();
uint8_t simulate_get_client_data();

#endif // BWC_SIMULATOR_H