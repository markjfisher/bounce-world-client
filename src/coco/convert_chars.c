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

			case 'a': data[i] = 171;  		 break;     // >▌<
			case 'b': data[i] = 172;         break;     // >▐< inverted version of previous

			case 'c': data[i] = 173;         break;     // >▄<
			case 'd': data[i] = 174;         break;     // >▀< inverted version of previous

			case 'e': data[i] = 175;         break;     // >▖<
			case 'f': data[i] = 176;         break;     // >▗<
			case 'g': data[i] = 177;         break;     // >▘<
			case 'h': data[i] = 178;         break;     // >▝<

            // inverted versions of the above 4
			case 'i': data[i] = 179;         break;     // >▜<
			case 'j': data[i] = 180;         break;     // >▛<
			case 'k': data[i] = 181;         break;     // >▟<
			case 'l': data[i] = 182;         break;     // >▙<

			case 'm': data[i] = 183;         break;     // >█< inverted space

			case 'n': data[i] = 184;         break;     // >▚<
			case 'p': data[i] = 185;         break;     // >▞<

			case '/': data[i] = '/';         break;     // /
			case '\\': data[i] = '\\';       break;     // \

			default: break;
		}
	}
}
