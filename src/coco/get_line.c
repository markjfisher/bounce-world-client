#include <cmoc.h>
#include <coco.h>

#include "get_line.h"

void get_line(char* buf, uint8_t max_len) {
	char *tmp;

	// cmoc's readline method returns the BASIC input buffer,
	// so we don't have to allocate space for it.
	tmp = readline();
	strncpy(buf, tmp, max_len);
}
