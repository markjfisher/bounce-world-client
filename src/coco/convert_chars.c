#include <cmoc.h>
#include <coco.h>

#include "chardef.h"
#include "convert_chars.h"

void convert_chars(uint8_t *data, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {
		switch(data[i]) {
			case 'r': data[i] = CH_ULCORNER; break;     // ┌
			case ')': data[i] = CH_URCORNER; break;     // ┐
			case 'L': data[i] = CH_LLCORNER; break;     // └
			case '!': data[i] = CH_LRCORNER; break;     // ┘
			case 'J': data[i] = CH_RTEE;     break;     // ┤
			case 't': data[i] = CH_LTEE;     break;     // ├
			case 'T': data[i] = CH_TTEE;     break;     // ┬
			case '2': data[i] = CH_BTEE;     break;     // ┴
			case '|': data[i] = CH_VLINE;    break;     // │
			case '-': data[i] = CH_HLINE;    break;     // ─
			case '+': data[i] = CH_CROSS;    break;     // ┼

			case 'a': data[i] = CH_VLINE_L;  break;     // >▌<
			case 'b': data[i] = CH_VLINE_R;  break;     // >▐< inverted version of previous

			case 'c': data[i] = CH_HLINE_T;  break;     // >▄<
			case 'd': data[i] = CH_HLINE_B;  break;     // >▀< inverted version of previous

			case 'e': data[i] = 141;         break;     // >▖<
			case 'f': data[i] = 142;         break;     // >▗<
			case 'g': data[i] = 135;         break;     // >▘<
			case 'h': data[i] = 139;         break;     // >▝<

            // inverted versions of the above 4
			case 'i': data[i] = CH_URCORNER; break;     // >▜<
			case 'j': data[i] = CH_ULCORNER; break;     // >▛<
			case 'k': data[i] = CH_LRCORNER; break;     // >▟<
			case 'l': data[i] = CH_LLCORNER; break;     // >▙<

			case 'm': data[i] = 128;         break;     // >█< inverted space

			case 'n': data[i] = 134;         break;     // >▚<
			case 'p': data[i] = 137;         break;     // >▞<

			case '/': data[i] = '/';         break;     // /
			case '\\': data[i] = '\\';       break;     // \

			default: break;
		}
	}
}
