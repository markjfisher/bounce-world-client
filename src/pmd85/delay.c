#include <stdint.h>

void wait_vsync() {};
void pause(uint8_t count)
{
    while(count--)
        for(int i=0; i<1000; i++);
};
