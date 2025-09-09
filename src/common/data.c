#include "data.h"

// GENERAL BUFFERS
char    server_url[80];
uint8_t app_data[APP_DATA_SIZE];
uint8_t shapes_buffer[SHAPES_BUFFER_SIZE];
char    name[9];
uint8_t name_pad;
char    clients_buffer[512];
char    broadcast_message[120];
uint8_t cmd_tmp[64];
char    client_data_cmd[10];
uint8_t client_data_cmd_len;

// room for 50 shapes is 50 * 5 = 250 bytes, in BSS so not saved to disk. The data for the shapes strings is stored in _shapes_buffer above.
ShapeRecord shapes[50];
uint8_t shape_count;

// CLIENT INFO
char    client_id;
char    client_str[8];

// status byte for client
uint8_t app_status;

// WORLD FLAGS/DATA - must be contiguous in memory
// TODO: make this a struct
uint16_t world_width;
uint16_t world_height;
uint16_t body_count;
uint8_t body_1;
uint8_t body_2;
uint8_t body_3;
uint8_t body_4;
uint8_t body_5;
uint8_t num_clients;
uint8_t world_is_frozen;
uint8_t world_is_wrapped;


uint8_t is_running_sim       = 0x01;
uint8_t current_step         = 0xff;
uint8_t info_display_count   = 0x00;
uint8_t is_darkmode          = 0x01;
uint8_t is_showing_info      = 0x00;
uint8_t is_showing_clients   = 0x00;
uint8_t is_showing_broadcast = 0x00;
uint8_t flash_on_collision   = 0x00;
