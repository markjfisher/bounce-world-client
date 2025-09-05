#include <cmoc.h>
#include <coco.h>

int strncasecmp(const char *s1, const char *s2, size_t n)
{
    for ( ; n > 0 && *s1 && *s2; --n, ++s1, ++s2)
    {
        int diff = toupper((int) (unsigned char) *s1) - toupper((int) (unsigned char) *s2);
        if (diff)
            return diff;
    }
    if (n == 0)
        return 0;
    if (!*s1)
        return *s2 ? -1 : 0;
    // Here, n > 0 && *s1 != '\0', so *s2 must be '\0'.
    return +1;  // s1 is longer, so it comes after s2.
}

