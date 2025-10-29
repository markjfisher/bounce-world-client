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
#include "fujinet-fuji.h"

// Mark Fisher (aka FENROCK [FE0C]) creator id
#define CREATOR_ID 0xFE0C
#define BOUNCY_WORLD_APP_ID 0x02
#define APP_KEY_ENDPOINT 0x01
#define APP_KEY_NAME 0x02

#define ENDPOINT_LEN 60
#define NAME_LEN 8

char read_buffer[65];


bool read_endpoint_appkey(char *endpoint_buf)
{
    bool r;
	uint16_t read_count = 0;

    fuji_set_appkey_details(CREATOR_ID, BOUNCY_WORLD_APP_ID, DEFAULT);

    r = fuji_read_appkey(APP_KEY_ENDPOINT, &read_count, read_buffer);
    if (r == true && read_count > 0)
    {
        read_buffer[read_count] = 0;
        strncpy(endpoint_buf, read_buffer, ENDPOINT_LEN);
        return true;
    }
    else
    {
        endpoint_buf[0] = 0;
        return false;
    }
}

bool write_endpoint_appkey(char* endpoint_buf)
{
    bool r;
	uint16_t write_count = 0;

    fuji_set_appkey_details(CREATOR_ID, BOUNCY_WORLD_APP_ID, DEFAULT);

    r = fuji_write_appkey(APP_KEY_ENDPOINT, strlen(endpoint_buf), endpoint_buf);

    return r;
}

bool read_name_appkey(char *name_buf)
{
    bool r;
	uint16_t read_count = 0;

    fuji_set_appkey_details(CREATOR_ID, BOUNCY_WORLD_APP_ID, DEFAULT);

    r = fuji_read_appkey(APP_KEY_NAME, &read_count, read_buffer);
    if (r == true && read_count > 0)
    {
        read_buffer[read_count] = 0;
        strncpy(name_buf, read_buffer, NAME_LEN);
        return true;
    }
    else
    {
        name_buf[0] = 0;
        return false;
    }
}

bool write_name_appkey(char* name_buf)
{
    bool r;

    fuji_set_appkey_details(CREATOR_ID, BOUNCY_WORLD_APP_ID, DEFAULT);

    r = fuji_write_appkey(APP_KEY_NAME, strlen(name_buf), name_buf);

    return r;
}


