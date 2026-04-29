#ifndef STRNCASECMP_H
#define STRNCASECMP_H

#include <string.h>

#define strncasecmp(s1, s2, n) _strnicmp((s1), (s2), (n))

#endif
