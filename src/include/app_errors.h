#ifndef BWC_ERRORS_H
#define BWC_ERRORS_H

#ifndef _CMOC_VERSION_
#include <stdint.h>
#endif

void handle_err(char *reason);
extern uint8_t err;

#endif // BWC_ERRORS_H
