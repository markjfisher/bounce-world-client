#include <stdint.h>

#include "convert_chars.h"
#include "chardef.h"

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

			case 'a': data[i] = 25;		  	 break;     // >▌<
			case 'b': data[i] = 121;         break;     // >▐<

			case 'c': data[i] = 21;          break;     // >▄<
			case 'd': data[i] = 117;         break;     //    >▀<

			case 'e': data[i] = 15; 		 break;     // >▖<
			case 'f': data[i] = 9;  		 break;     // >▗<
			case 'g': data[i] = 12; 		 break;     // >▘<
			case 'h': data[i] = 11; 		 break;     // >▝<

            // inverted versions of the above 4
			case 'i': data[i] = 111;         break;     // >▜<
			case 'j': data[i] = 105;         break;     // >▛<
			case 'k': data[i] = 108;         break;     // >▟<
			case 'l': data[i] = 107;         break;     // >▙<

			case 'm': data[i] = 125;         break;     // >█<

			case 'n': data[i] = 99;          break;     // >▚<
			case 'p': data[i] = 101;         break;     //    >▞<

			case '/': data[i] = 6;           break;     // /
			case '\\': data[i] = 7;          break;     // \

			default: break;
		}
	}
}