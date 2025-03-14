#include <stdint.h>
#include <apple2enh.h>

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

			case 'a': data[i] = 0xDF;        break;     // >▌<
			case 'b': data[i] = 0xDA;        break;     // >▐<

			case 'c': data[i] = 0x5F;        break;     // >▄<
			case 'd': data[i] = 0xCC;        break;     //    >▀<

			// no equivalents, using diamond
			case 'e': data[i] = 0xDB;        break;     // >▖<
			case 'f': data[i] = 0xDB;        break;     // >▗<
			case 'g': data[i] = 0xDB;        break;     // >▘<
			case 'h': data[i] = 0xDB;        break;     // >▝<

            // no equivalents, using diamond
			case 'i': data[i] = 0xDD;        break;     // >▜<
			case 'j': data[i] = 0xDD;        break;     // >▛<
			case 'k': data[i] = 0xDD;        break;     // >▟<
			case 'l': data[i] = 0xDD;        break;     // >▙<

			case 'm': data[i] = 0xD6;        break;     // >█< inverted space, using a alternate dot pattern instead

            // don't have these in Atascii
			// case 'n': data[i] = 11+128; break;     // >▚<
			// case 'p': data[i] = 11+128; break;     // >▞<

			// case '/': data[i] = 6; break;          // /
			// case '\\': data[i] = 7; break;         // \

			default: break;
		}
	}
}