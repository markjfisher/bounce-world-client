#include <atari.h>
#include <stdint.h>

#include "cio.h"
#include "getLine.h"

void get_line(char* buf, uint8_t len)
{
	uint8_t i = 0;

	OS.iocb[0].buffer  = buf;
	OS.iocb[0].buflen  = len;
	OS.iocb[0].command = 5; // Get text record
	ciov();

	// remove the trailing 0x9b
	while(buf[i] != 0x9b && i < len) { i++; }
	if (i < len) {
		buf[i] = '\0';
	}
}