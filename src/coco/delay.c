#include <cmoc.h>
#include <coco.h>

void wait_vsync()
{
    pause(1);	
}
void pause(uint8_t count)
{
    if (!count)
        return;
    setTimer(0);
    while (getTimer() < count);
}


