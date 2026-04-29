#include <time.h>
#include <stdint.h>
#include <conio.h>

#include "delay.h"

void pause(uint8_t count)
{
    clock_t target;
    if (!count) return;
    target = clock() + (clock_t)count;
    while (clock() < target) ;
}

void wait_vsync(void)
{
    while ((inp(0x3DA) & 0x08) != 0) ;
    while ((inp(0x3DA) & 0x08) == 0) ;
}
