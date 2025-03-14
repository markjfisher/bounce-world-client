#include <stdint.h>
#include <apple2enh.h>

void pause(uint8_t count)
{
	while (count--)
		waitvsync();
};
