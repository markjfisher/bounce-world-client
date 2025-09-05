#ifndef BWC_DELAY_H
#define BWC_DELAY_H

#ifndef _CMOC_VERSION_
#include <stdint.h>
#else
#include <cmoc.h>
#include <coco.h>
#endif

void wait_vsync();
void pause(uint8_t count);

#endif // BWC_DELAY_H
