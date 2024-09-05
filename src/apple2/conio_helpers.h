#ifndef BWC_CONIO_HELPERS_H
#define BWC_CONIO_HELPERS_H

#ifdef __APPLE2__

#include <stdint.h>

void gotoxy_buffer(uint8_t x, uint8_t y);
void cputsxy_buffer(uint8_t x, uint8_t y, char *s);
void cputcxy_buffer(uint8_t x, uint8_t y, char c);

#endif

#endif // BWC_CONIO_HELPERS_H