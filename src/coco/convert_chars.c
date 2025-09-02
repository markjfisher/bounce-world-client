#include <cbm.h>
#include <stdint.h>

#include "convert_chars.h"

// NEEDS FIXING IF CONTROL CHARS REQUIRED FOR INVERSE

void convert_chars(uint8_t *data, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {
		switch(data[i]) {
			// case 'r': data[i] = CH_ULCORNER; break;     // ┌
			case 0x52: data[i] = CH_ULCORNER; break;     // ┌
			// case ')': data[i] = CH_URCORNER; break;     // ┐
			case 0x29: data[i] = CH_URCORNER; break;     // ┐
			// case 'L': data[i] = CH_LLCORNER; break;     // └
			case 0x6c: data[i] = CH_LLCORNER; break;     // └
			// case '!': data[i] = CH_LRCORNER; break;     // ┘
			case 0x21: data[i] = CH_LRCORNER; break;     // ┘
			// case 'J': data[i] = CH_RTEE;     break;     // ┤
			case 0x27: data[i] = CH_RTEE;     break;     // ┤
			// case 't': data[i] = CH_LTEE;     break;     // ├
			case 0x54: data[i] = CH_LTEE;     break;     // ├
			// case 'T': data[i] = CH_TTEE;     break;     // ┬
			case 0x74: data[i] = CH_TTEE;     break;     // ┬
			// case '2': data[i] = CH_BTEE;     break;     // ┴
			case 0x32: data[i] = CH_BTEE;     break;     // ┴
			// case '|': data[i] = CH_VLINE;    break;     // │
			case 0x7c: data[i] = CH_VLINE;    break;     // │
			// case '-': data[i] = CH_HLINE;    break;     // ─
			case 0x2d: data[i] = CH_HLINE;    break;     // ─
			// case '+': data[i] = CH_CROSS;    break;     // ┼
			case 0x2b: data[i] = CH_CROSS;    break;     // ┼

			// 'a'
			case 0x41:                                     // >▌<
				data[i]   = 0xA1;
				break;
			case 0x42: 								      // >▐<
				data[i] = 0xA1;
				// data[i]   = 0x12;
				// data[i]   = 0xA1;
				// data[i]   = 0x92;
				break;

			case 0x43:                                     // >▄<
				data[i]   = 0xA2;
				break;
			case 0x44:                                     // >▀<
				data[i]   = 0xA2;
				// data[i]   = 0x12;
				// data[i]   = 0xA2;
				// data[i]   = 0x92;
				break;

			// no equivalents, using diamond
			case 0x45: data[i] = 0xBB;        break;     // >▖<
			case 0x46: data[i] = 0xAC;        break;     // >▗<
			case 0x47: data[i] = 0xBE;        break;     // >▘<
			case 0x48: data[i] = 0xBC;        break;     // >▝<

			// these are wrong, but I don't think I can do the inverse in a single char, but the rest of the app only wants one char
			// unless it detects for c64 that $12 is being used for reverse, and then prints 3 chars instead of 1. TBD
			case 0x49:                                     // >▜<
				data[i]   = 0xBB;
				// data[i]   = 0x12;
				// data[i]   = 0xBB;
				// data[i]   = 0x92;
				break;
			case 0x4a:                                     // >▛<
				data[i]   = 0xAC;
				// data[i]   = 0x12;
				// data[i]   = 0xAC;
				// data[i]   = 0x92;
				break;
			case 0x4b:                                     // >▟<
				data[i]   = 0xBE;
				// data[i]   = 0x12;
				// data[i]   = 0xBE;
				// data[i]   = 0x92;
				break;
			case 0x4c:                                     // >▙<
				data[i]   = 0xBC;
				// data[i]   = 0x12;
				// data[i]   = 0xBC;
				// data[i]   = 0x92;
				break;

			case 0x4d:                                     // >█<
				data[i]   = 0xA6;
				// data[i]   = 0x12;
				// data[i]   = 0x20;
				// data[i]   = 0x92;
				break;

			case 0x4e:                                     // >▚<
				data[i]   = 0xBF;
				break;
			case 0x50:                                     // >▞<
				data[i]   = 0xBF;
				// data[i]   = 0x12;
				// data[i]   = 0xBF;
				// data[i]   = 0x92;
				break;

			case 0x5c: // "\"
				data[i]   = 0xBF;		// there isnt a \ char in this charset, so use the ▚
				break; 

			default: break;
		}
	}
}