#ifndef BWC_ERRORS_H
#define BWC_ERRORS_H

#include <stdint.h>

void handle_err(char *reason);
extern uint8_t err;

#endif // BWC_ERRORS_H