#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "simulator.h"
#include "world.h"

#ifdef SIMULATE_FN

// track the cmd being an enable broadcast or disable for our simple simulator, we shall toggle the 2 times the broadcast is sent on and off
uint8_t sim_cmd_index = 0;

// track the index of the user data
uint8_t sim_step = 0;

void create_mock_shapes() {
    // set app_data with equivalent data from FN
    memcpy(app_data, shape_data, sizeof(shape_data));
}

void simulate_get_world_state() {
    // read 14 bytes of state into world_width
    memcpy((uint8_t *) &world_width, ws_1, sizeof(ws_1));
}

void simulate_get_world_clients() {
    // put 8 bytes per client in clients_buffer. We are just going to pretend we have 1 client called sim-1
    memset(clients_buffer, 0, 240);
    strcat(clients_buffer, "sim-1   ");
}

void simulate_get_broadcast() {
    // put message in broadcast_message
    memset(broadcast_message, 0, 120);
    strcat(broadcast_message, "This is a simulated message from the server to display on the client");
}

uint8_t simulate_get_world_cmd() {
    // put commands in app_data, return number of commands
    app_data[0] = cmd_data[sim_cmd_index][0];
    sim_cmd_index = 1 - sim_cmd_index;      // we only have 2 cmds that will be broadcast in the simulator, enable/disable broadcasts to pop up a window during simulation

    // only 1 byte to process
    return 1;
}

uint8_t simulate_get_client_data() {
    // put client data (objects etc) in app_data, return byte size
    uint8_t *ud = user_data[sim_step++];
    if (sim_step == NUM_ENTRIES) sim_step = 0;

    app_data[0]  = ud[0];
    app_data[1]  = ud[1];
    app_data[2]  = 4;
    app_data[3]  = 0;
    app_data[4]  = ud[2];
    app_data[5]  = ud[3];
    app_data[6]  = 0;
    app_data[7]  = ud[4];
    app_data[8]  = ud[5];
    app_data[9]  = 0;
    app_data[10] = ud[6];
    app_data[11] = ud[7];
    app_data[12] = 0;
    app_data[13] = ud[8];
    app_data[14] = ud[9];

    return 15;
}

// A single size 5 shape will do for simulation
uint8_t shape_data[27] = {0x00, 0x05, 0x20, 0x20, 0x72, 0x29, 0x20, 0x72, 0x2D, 0x21, 0x4C, 0x29, 0x4C, 0x29, 0x20, 0x72, 0x21, 0x20, 0x4C, 0x29, 0x7C, 0x20, 0x20, 0x20, 0x4C, 0x21, 0x20};

// the "world status" data is fixed as we are not adding new clients, or changing frozen/wrapping state during simulator
// width(2), height(2), body_count(2), 1_count(1), 2_count(1), 3_count(1), 4_count(1), 5_count(1), client_count(1), is_frozen(1), is_wrapping(1)
uint8_t ws_1[14] = {0xA0, 0x00, 0x58, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00 };

// cmd data
uint8_t cmd_data[2][1] = {
    {0x05},         // enable broadcast
    {0x06}          // disable broadcast
};


// USER DATA - this is the data that would come back from /w/<user id> endpoint representing the clients' "world" view of events (collisions etc) and object locations.
// In this static simulation we have 4 objects, all with shape id 0 (the only shape being used in this simulator).

// Typical real shape would actually be 15 bytes.
// {0xB5, 0x22, 0x04, 0x00, 0x12, 0x0A, 0x00, 0x07, 0x03, 0x00, 0x19, 0x13, 0x00, 0x20, 0x10}
// But column (0 based) 2, 3, 6, 9, 12 are always 4,0,0,0,0 respectively so can be removed from actual array to save space

// Column 2 of the data is the app_status value, bits represent different events:
//  $20 = collision (client can do some fx)
//  $08 = cmd (causes fetch of awaiting commands)
//  $04 = frozen (keeps being sent while server is in frozen mode) not used here
//  $02 = object change (causes refresh of screen for client to update info area)
//  $01 = client change (added/removed) not used here

// 1300 bytes (130 rows of 10 bytes)
uint8_t user_data[NUM_ENTRIES][DATA_SIZE] = {
    {0x47, 0x00, 0x19, 0x0C, 0x06, 0x10, 0x25, 0x0B, 0x12, 0x0A},
    {0x49, 0x00, 0x1A, 0x0D, 0x06, 0x11, 0x26, 0x0B, 0x12, 0x09},
    {0x4C, 0x00, 0x1B, 0x0E, 0x07, 0x14, 0x25, 0x0A, 0x12, 0x07},
    {0x4E, 0x00, 0x1C, 0x0F, 0x08, 0x12, 0x24, 0x09, 0x12, 0x06},
    {0x51, 0x00, 0x1E, 0x10, 0x08, 0x0F, 0x23, 0x08, 0x12, 0x04},
    {0x53, 0x00, 0x1F, 0x11, 0x09, 0x0E, 0x23, 0x07, 0x12, 0x03},
    {0x55, 0x00, 0x20, 0x12, 0x09, 0x0C, 0x22, 0x06, 0x12, 0x03},
    {0x58, 0x00, 0x21, 0x13, 0x0A, 0x09, 0x21, 0x05, 0x12, 0x05},
    {0x5A, 0x00, 0x22, 0x13, 0x0B, 0x08, 0x20, 0x05, 0x12, 0x06},
    {0x5C, 0x00, 0x23, 0x13, 0x0B, 0x06, 0x20, 0x04, 0x12, 0x08},
    {0x5E, 0x00, 0x24, 0x13, 0x0C, 0x04, 0x1F, 0x03, 0x12, 0x09},
    {0x61, 0x00, 0x25, 0x11, 0x0C, 0x03, 0x1E, 0x03, 0x12, 0x0B},
    {0x63, 0x00, 0x25, 0x11, 0x0D, 0x05, 0x1E, 0x03, 0x12, 0x0C},
    {0x66, 0x00, 0x24, 0x10, 0x0E, 0x08, 0x1D, 0x04, 0x12, 0x0E},
    {0x68, 0x00, 0x23, 0x0F, 0x0E, 0x0A, 0x1C, 0x05, 0x12, 0x0F},
    {0x6A, 0x00, 0x22, 0x0E, 0x0F, 0x0B, 0x1B, 0x06, 0x12, 0x10},
    {0x6D, 0x00, 0x20, 0x0D, 0x10, 0x0E, 0x1B, 0x07, 0x12, 0x12},
    {0x6F, 0x20, 0x1F, 0x0C, 0x10, 0x0F, 0x1A, 0x07, 0x12, 0x14},
    {0x72, 0x20, 0x1E, 0x0B, 0x0E, 0x0F, 0x19, 0x09, 0x14, 0x13},
    {0x74, 0x00, 0x1D, 0x0A, 0x0D, 0x0E, 0x18, 0x09, 0x16, 0x13},
    {0x77, 0x20, 0x1D, 0x0A, 0x0B, 0x0D, 0x16, 0x0A, 0x19, 0x13},
    {0x79, 0x00, 0x1C, 0x09, 0x0A, 0x0D, 0x15, 0x0A, 0x1A, 0x12},
    {0x7C, 0x00, 0x1C, 0x08, 0x08, 0x0C, 0x13, 0x0B, 0x1D, 0x12},
    {0x7E, 0x00, 0x1C, 0x08, 0x07, 0x0B, 0x12, 0x0C, 0x1F, 0x12},
    {0x80, 0x00, 0x1B, 0x07, 0x06, 0x0A, 0x11, 0x0C, 0x20, 0x12},
    {0x83, 0x00, 0x1B, 0x06, 0x04, 0x0A, 0x0F, 0x0D, 0x23, 0x12},
    {0x85, 0x00, 0x1B, 0x06, 0x03, 0x09, 0x0D, 0x0E, 0x25, 0x12},
    {0x88, 0x00, 0x1A, 0x05, 0x04, 0x08, 0x0B, 0x0E, 0x25, 0x12},
    {0x8A, 0x00, 0x1A, 0x04, 0x05, 0x08, 0x0A, 0x0F, 0x23, 0x11},
    {0x8D, 0x00, 0x19, 0x03, 0x07, 0x07, 0x08, 0x10, 0x20, 0x11},
    {0x8F, 0x00, 0x19, 0x03, 0x08, 0x06, 0x07, 0x10, 0x1F, 0x11},
    {0x91, 0x00, 0x19, 0x03, 0x09, 0x05, 0x06, 0x11, 0x1D, 0x11},
    {0x94, 0x00, 0x18, 0x04, 0x0B, 0x05, 0x04, 0x12, 0x1A, 0x11},
    {0x96, 0x00, 0x18, 0x04, 0x0C, 0x04, 0x03, 0x12, 0x19, 0x11},
    {0x99, 0x00, 0x18, 0x05, 0x0E, 0x03, 0x05, 0x13, 0x16, 0x10},
    {0x9B, 0x00, 0x17, 0x06, 0x0F, 0x03, 0x06, 0x13, 0x14, 0x10},
    {0x9E, 0x00, 0x17, 0x07, 0x11, 0x04, 0x08, 0x13, 0x12, 0x10},
    {0xA0, 0x00, 0x17, 0x07, 0x13, 0x04, 0x09, 0x13, 0x10, 0x10},
    {0xA2, 0x20, 0x17, 0x08, 0x14, 0x05, 0x0A, 0x12, 0x0E, 0x10},
    {0xA5, 0x20, 0x17, 0x0A, 0x15, 0x04, 0x09, 0x13, 0x0F, 0x0E},
    {0xA7, 0x00, 0x17, 0x0B, 0x15, 0x04, 0x08, 0x13, 0x0F, 0x0C},
    {0xAA, 0x00, 0x17, 0x0D, 0x16, 0x04, 0x07, 0x12, 0x10, 0x0A},
    {0xAC, 0x00, 0x18, 0x0E, 0x17, 0x04, 0x06, 0x11, 0x10, 0x08},
    {0xAF, 0x00, 0x18, 0x10, 0x18, 0x04, 0x05, 0x0F, 0x11, 0x05},
    {0xB1, 0x00, 0x18, 0x12, 0x19, 0x04, 0x04, 0x0E, 0x12, 0x04},
    {0xB3, 0x00, 0x18, 0x13, 0x1A, 0x03, 0x03, 0x0D, 0x12, 0x03},
    {0xB6, 0x00, 0x18, 0x13, 0x1B, 0x03, 0x04, 0x0B, 0x13, 0x05},
    {0xB8, 0x00, 0x19, 0x12, 0x1C, 0x03, 0x04, 0x0A, 0x13, 0x07},
    {0xBB, 0x00, 0x19, 0x10, 0x1D, 0x03, 0x06, 0x09, 0x14, 0x09},
    {0xBD, 0x00, 0x19, 0x0E, 0x1E, 0x03, 0x07, 0x08, 0x14, 0x0B},
    {0xC0, 0x20, 0x1A, 0x0D, 0x1F, 0x03, 0x08, 0x06, 0x14, 0x0D},
    {0xC2, 0x00, 0x1B, 0x0B, 0x1F, 0x03, 0x09, 0x05, 0x14, 0x0F},
    {0xC5, 0x00, 0x1C, 0x0A, 0x20, 0x03, 0x0A, 0x04, 0x14, 0x11},
    {0xC7, 0x00, 0x1D, 0x08, 0x21, 0x03, 0x0B, 0x03, 0x14, 0x13},
    {0xC9, 0x00, 0x1E, 0x07, 0x22, 0x03, 0x0C, 0x03, 0x14, 0x13},
    {0xCC, 0x00, 0x1F, 0x05, 0x23, 0x03, 0x0D, 0x05, 0x13, 0x10},
    {0xCE, 0x20, 0x1F, 0x05, 0x25, 0x03, 0x0E, 0x06, 0x13, 0x0F},
    {0xD1, 0x00, 0x1F, 0x03, 0x25, 0x03, 0x10, 0x07, 0x13, 0x0C},
    {0xD3, 0x20, 0x1F, 0x03, 0x24, 0x03, 0x10, 0x07, 0x14, 0x0C},
    {0xD5, 0x00, 0x1D, 0x04, 0x25, 0x03, 0x0F, 0x06, 0x15, 0x0D},
    {0xD8, 0x00, 0x1C, 0x05, 0x25, 0x03, 0x0E, 0x04, 0x17, 0x0E},
    {0xDA, 0x00, 0x1A, 0x06, 0x25, 0x03, 0x0D, 0x03, 0x19, 0x0E},
    {0xDD, 0x00, 0x18, 0x08, 0x25, 0x03, 0x0C, 0x04, 0x1B, 0x0F},
    {0xDF, 0x00, 0x17, 0x09, 0x26, 0x03, 0x0B, 0x05, 0x1C, 0x10},
    {0xE1, 0x00, 0x16, 0x0A, 0x26, 0x03, 0x0B, 0x06, 0x1E, 0x10},
    {0xE4, 0x00, 0x14, 0x0B, 0x25, 0x03, 0x0A, 0x08, 0x20, 0x11},
    {0xE6, 0x00, 0x13, 0x0C, 0x25, 0x03, 0x09, 0x09, 0x22, 0x12},
    {0xE9, 0x00, 0x11, 0x0E, 0x25, 0x03, 0x08, 0x0B, 0x24, 0x13},
    {0xEB, 0x00, 0x10, 0x0F, 0x25, 0x03, 0x07, 0x0C, 0x25, 0x14},
    {0xED, 0x00, 0x0E, 0x10, 0x24, 0x03, 0x07, 0x0D, 0x25, 0x13},
    {0xEF, 0x00, 0x0D, 0x11, 0x24, 0x03, 0x06, 0x0F, 0x23, 0x13},
    {0xF2, 0x00, 0x0B, 0x13, 0x24, 0x03, 0x05, 0x10, 0x21, 0x12},
    {0xF4, 0x00, 0x0A, 0x14, 0x24, 0x03, 0x04, 0x12, 0x20, 0x11},
    {0xF6, 0x00, 0x09, 0x13, 0x24, 0x03, 0x03, 0x13, 0x1E, 0x10},
    {0xF9, 0x20, 0x08, 0x11, 0x23, 0x03, 0x03, 0x13, 0x1C, 0x10},
    {0xFB, 0x00, 0x08, 0x10, 0x23, 0x03, 0x04, 0x13, 0x1B, 0x0F},
    {0xFD, 0x00, 0x09, 0x0E, 0x23, 0x04, 0x05, 0x12, 0x19, 0x0E},
    {0x00, 0x00, 0x0A, 0x0C, 0x23, 0x04, 0x07, 0x11, 0x17, 0x0D},
    {0x01, 0x00, 0x0A, 0x0A, 0x22, 0x04, 0x08, 0x11, 0x16, 0x0D},
    {0x0D, 0x20, 0x0D, 0x03, 0x21, 0x04, 0x0D, 0x0F, 0x0F, 0x09},
    {0x10, 0x20, 0x0D, 0x05, 0x21, 0x04, 0x0E, 0x10, 0x0F, 0x0B},
    {0x12, 0x00, 0x0D, 0x07, 0x21, 0x04, 0x0E, 0x12, 0x0F, 0x0B},
    {0x14, 0x20, 0x0C, 0x09, 0x21, 0x04, 0x0E, 0x14, 0x0F, 0x0C},
    {0x17, 0x20, 0x0B, 0x0B, 0x20, 0x04, 0x0F, 0x11, 0x10, 0x0D},
    {0x19, 0x20, 0x0B, 0x0D, 0x20, 0x04, 0x0E, 0x11, 0x12, 0x0C},
    {0x1C, 0x20, 0x09, 0x0F, 0x20, 0x04, 0x0E, 0x11, 0x14, 0x0B},
    {0x1E, 0x00, 0x07, 0x10, 0x20, 0x05, 0x0E, 0x12, 0x16, 0x0A},
    {0x20, 0x00, 0x06, 0x10, 0x1F, 0x05, 0x0E, 0x12, 0x18, 0x0A},
    {0x23, 0x00, 0x03, 0x12, 0x1F, 0x05, 0x0F, 0x13, 0x1A, 0x08},
    {0x25, 0x00, 0x03, 0x13, 0x1F, 0x05, 0x0F, 0x13, 0x1C, 0x07},
    {0x28, 0x20, 0x06, 0x13, 0x21, 0x03, 0x0F, 0x13, 0x1C, 0x08},
    {0x2A, 0x00, 0x07, 0x12, 0x22, 0x03, 0x0F, 0x13, 0x1D, 0x09},
    {0x2D, 0x00, 0x0A, 0x11, 0x23, 0x05, 0x10, 0x12, 0x1D, 0x0A},
    {0x2F, 0x00, 0x0C, 0x10, 0x25, 0x07, 0x10, 0x12, 0x1D, 0x0A},
    {0x31, 0x20, 0x0C, 0x0F, 0x26, 0x08, 0x11, 0x12, 0x1E, 0x0B},
    {0x34, 0x00, 0x0D, 0x0D, 0x24, 0x0A, 0x13, 0x12, 0x1E, 0x0B},
    {0x36, 0x00, 0x0D, 0x0B, 0x23, 0x0B, 0x15, 0x12, 0x1E, 0x0C},
    {0x39, 0x20, 0x0E, 0x09, 0x23, 0x0D, 0x17, 0x11, 0x1D, 0x0D},
    {0x3B, 0x00, 0x0F, 0x08, 0x24, 0x0E, 0x18, 0x11, 0x1C, 0x0E},
    {0x3E, 0x20, 0x10, 0x06, 0x24, 0x0F, 0x18, 0x13, 0x1C, 0x0D},
    {0x40, 0x00, 0x10, 0x05, 0x25, 0x10, 0x18, 0x13, 0x1C, 0x0C},
    {0x43, 0x08, 0x11, 0x03, 0x25, 0x12, 0x17, 0x10, 0x1C, 0x0B},
    {0x45, 0x00, 0x12, 0x04, 0x26, 0x13, 0x17, 0x0F, 0x1C, 0x0A},
    {0x47, 0x00, 0x12, 0x05, 0x25, 0x13, 0x17, 0x0D, 0x1D, 0x09},
    {0x4A, 0x00, 0x13, 0x07, 0x25, 0x11, 0x17, 0x0A, 0x1D, 0x07},
    {0x4C, 0x20, 0x12, 0x07, 0x24, 0x10, 0x19, 0x0A, 0x1D, 0x06},
    {0x4E, 0x00, 0x10, 0x07, 0x24, 0x0F, 0x1A, 0x0A, 0x1D, 0x05},
    {0x51, 0x00, 0x0E, 0x06, 0x23, 0x0E, 0x1D, 0x0A, 0x1E, 0x04},
    {0x53, 0x00, 0x0D, 0x06, 0x23, 0x0D, 0x1F, 0x0A, 0x1E, 0x03},
    {0x55, 0x20, 0x0C, 0x06, 0x25, 0x0D, 0x1E, 0x09, 0x1E, 0x03},
    {0x58, 0x20, 0x0A, 0x05, 0x25, 0x0D, 0x1E, 0x08, 0x1F, 0x04},
    {0x5A, 0x20, 0x08, 0x05, 0x23, 0x0D, 0x1D, 0x09, 0x1F, 0x03},
    {0x5D, 0x00, 0x06, 0x05, 0x21, 0x0D, 0x1C, 0x0A, 0x20, 0x04},
    {0x5F, 0x20, 0x05, 0x04, 0x1F, 0x0D, 0x1B, 0x0B, 0x21, 0x05},
    {0x61, 0x00, 0x04, 0x04, 0x1F, 0x0E, 0x19, 0x0B, 0x22, 0x06},
    {0x64, 0x00, 0x03, 0x04, 0x1E, 0x0F, 0x16, 0x0B, 0x23, 0x07},
    {0x66, 0x00, 0x05, 0x03, 0x1D, 0x10, 0x15, 0x0B, 0x23, 0x08},
    {0x68, 0x08, 0x06, 0x03, 0x1C, 0x10, 0x13, 0x0B, 0x24, 0x09},
    {0x6B, 0x00, 0x08, 0x03, 0x1B, 0x11, 0x10, 0x0C, 0x25, 0x0B},
    {0x6D, 0x00, 0x09, 0x03, 0x1B, 0x12, 0x0E, 0x0C, 0x26, 0x0C},
    {0x70, 0x00, 0x0B, 0x03, 0x1A, 0x13, 0x0C, 0x0C, 0x25, 0x0D},
    {0x72, 0x00, 0x0D, 0x04, 0x19, 0x13, 0x0A, 0x0C, 0x24, 0x0E},
    {0x74, 0x00, 0x0E, 0x04, 0x18, 0x13, 0x08, 0x0C, 0x24, 0x0F},
    {0x77, 0x00, 0x10, 0x04, 0x17, 0x12, 0x06, 0x0D, 0x23, 0x11},
    {0x79, 0x00, 0x11, 0x05, 0x17, 0x12, 0x04, 0x0D, 0x22, 0x12},
    {0x7C, 0x00, 0x13, 0x05, 0x16, 0x11, 0x03, 0x0D, 0x21, 0x13},
    {0x7E, 0x00, 0x15, 0x05, 0x15, 0x10, 0x05, 0x0D, 0x20, 0x13},
    {0x80, 0x00, 0x16, 0x06, 0x14, 0x0F, 0x07, 0x0D, 0x20, 0x12},
    {0x83, 0x00, 0x18, 0x06, 0x13, 0x0F, 0x0A, 0x0E, 0x1F, 0x11},
    {0x85, 0x00, 0x19, 0x06, 0x13, 0x0E, 0x0B, 0x0E, 0x1E, 0x10}
};

#endif
