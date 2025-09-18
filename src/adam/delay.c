#include "delay.h"

extern unsigned int currentTime;

void pause(uint8_t count)
{
  unsigned int endCount;
  
  if (count == 0) return;

  endCount = currentTime + count;

  while(endCount != currentTime);
}