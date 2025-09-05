#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#include "conio_wrapper.h"
#else
#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#endif /* _CMOC_VERSION_ */

#ifdef __PMD85__
#include "conio_wrapper.h"
#include "itoa_wrapper.h"
#endif

// NON printf version of hex dump routine, saving about 1k from app size

void hd(void* data, uint8_t size) {
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t p = 0;
    uint8_t start = 0;
    uint8_t padding = 0;
    uint8_t c;
    char hexStr[3]; // For two hex digits and null terminator

    for (i = 0; i < size; i++) {
        memset(hexStr, 0, 3);
        itoa(*((uint8_t*)data + i), hexStr, 16);
        if (strlen(hexStr) < 2) { // Ensure two digits are printed
            cputs("0");
        }
        cputs(hexStr);
        cputs(" ");

        if ((i + 1) % 8 == 0 || i == size - 1) {
            padding = ((i + 1) % 8) ? (8 - (i + 1) % 8) : 0;
            for (p = 0; p < padding; p++) {
                cputs("   "); // for alignment
            }
            cputs(" | ");
            start = i - (i % 8);
            for (j = start; j <= i; j++) {
                c = *((uint8_t *)data + j);
                if (isprint(c)) {
                    cputc(c);
                } else {
                    cputc('.');
                }
            }
            cputs("\r\n"); // Move to the next line
        }
    }
}
