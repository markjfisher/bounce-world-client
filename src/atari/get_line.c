#include <atari.h>
#include <stdint.h>

#include "cio.h"
#include "get_line.h"

void get_line(char* buf, uint8_t max_len)
{
	uint8_t i = 0;

	OS.iocb[0].buffer  = buf;
	OS.iocb[0].buflen  = max_len;
	OS.iocb[0].command = 5; // Get text record
	ciov();

	// remove the trailing 0x9b
	while(buf[i] != 0x9b && i < 128) { i++; }
	if (i < 128) {
		buf[i] = '\0';
	}
}