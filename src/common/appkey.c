#ifndef _CMOC_VERSION_
#include <conio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#include "conio_wrapper.h"
#endif

#include "appkey.h"
#include "fujinet-fuji.h"

char read_buffer[65];

void init_appkey(void) {
    fuji_set_appkey_details(CREATOR_ID, BOUNCY_WORLD_APP_ID, DEFAULT);
}

bool read_appkey(char *buffer, uint8_t max_len, uint8_t key) {
    bool key_result = false;
    bool read_result = false;
    uint16_t read_count = 0;

    memset(read_buffer, 0, max_len);
    key_result = fuji_read_appkey(key, &read_count, read_buffer);
    if (key_result && read_count > 0)
    {
        strncpy(buffer, read_buffer, max_len);
        if (read_count >= max_len) read_count = max_len - 1;
        *(buffer+read_count) = 0;
        read_result = true;
    }
    return read_result;
}

bool write_appkey(char *buffer, uint8_t key) {
    bool key_result = false;
    uint16_t write_count = 0;

    key_result = fuji_write_appkey(key, strlen(buffer), buffer);
}
