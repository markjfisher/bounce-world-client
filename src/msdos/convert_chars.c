#include <stdint.h>

#include "chardef.h"
#include "convert_chars.h"

void convert_chars(uint8_t *data, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
        switch (data[i]) {
            case 'r': data[i] = CH_ULCORNER; break;
            case ')': data[i] = CH_URCORNER; break;
            case 'L': data[i] = CH_LLCORNER; break;
            case '!': data[i] = CH_LRCORNER; break;
            case 'J': data[i] = CH_RTEE;     break;
            case 't': data[i] = CH_LTEE;     break;
            case 'T': data[i] = CH_TTEE;     break;
            case '2': data[i] = CH_BTEE;     break;
            case '|': data[i] = CH_VLINE;    break;
            case '-': data[i] = CH_HLINE;    break;
            case '+': data[i] = CH_CROSS;    break;

            case 'a': data[i] = 0xDD; break;
            case 'b': data[i] = 0xDE; break;

            case 'c': data[i] = 0xDC; break;
            case 'd': data[i] = 0xDF; break;

            case 'e': data[i] = 0xB0; break;
            case 'f': data[i] = 0xB0; break;
            case 'g': data[i] = 0xB0; break;
            case 'h': data[i] = 0xB0; break;

            case 'i': data[i] = 0xB1; break;
            case 'j': data[i] = 0xB1; break;
            case 'k': data[i] = 0xB1; break;
            case 'l': data[i] = 0xB1; break;

            case 'm': data[i] = 0xDB; break;

            case 'n': data[i] = 0xB2; break;
            case 'p': data[i] = 0xB2; break;

            default: break;
        }
    }
}
