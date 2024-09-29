#ifndef BWC_RESILIENCE_H
#define BWC_RESILIENCE_H

#include <stdint.h>

void try_open(char *msg, char *url, uint8_t mode);

#endif // BWC_RESILIENCE_H