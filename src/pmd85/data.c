#include "data.h"

// GENERAL BUFFERS
uint8_t url_buffer[128];
uint8_t app_data[APP_DATA_SIZE];
uint8_t shapes_buffer[SHAPES_BUFFER_SIZE];
uint8_t name[9];
uint8_t name_pad;
uint8_t endpoint[80];
uint8_t client_data_url[96];
uint8_t clients_buffer[240];
uint8_t broadcast_message[120];

// room for 50 shapes is 50 * 5 = 250 bytes, in BSS so not saved to disk. The data for the shapes strings is stored in _shapes_buffer above.
ShapeRecord shapes[50];
uint8_t shape_count;

// CLIENT INFO
uint8_t client_id;
uint8_t client_str[4];

// status byte for client
uint8_t app_status;

// WORLD FLAGS/DATA
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
