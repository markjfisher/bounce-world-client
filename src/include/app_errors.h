#ifndef APP_ERRORS_H
#define APP_ERRORS_H

#include <stdint.h>

void handle_err(char *reason);
extern uint8_t err;

#endif // APP_ERRORS_H