#include <stdint.h>

#include "convert_chars.h"

void convert_chars(char *s) {
	uint8_t i;
	for (i = 0; s[i] != '\0'; i++) {
		switch(s[i]) {
			case 'r': s[i] = 17; break;         // ┌
			case ')': s[i] = 5; break;          // ┐
			case 'L': s[i] = 26; break;         // └
			case '!': s[i] = 3; break;          // ┘
			case 'J': s[i] = 4; break;          // ┤
			case 't': s[i] = 1; break;          // ├
			case 'T': s[i] = 23; break;         // ┬
			case '2': s[i] = 24; break;         // ┴
			case '|': s[i] = 124; break;        // │
			case '-': s[i] = 18; break;         // ─
			case '+': s[i] = 19; break;         // ┼

			case 'a': s[i] = 25; break;         // >▌<
			case 'b': s[i] = 25+128; break;     // >▐< inverted version of previous

			case 'c': s[i] = 21; break;         // >▄<
			case 'd': s[i] = 21+128; break;     // >▀< inverted version of previous

			case 'e': s[i] = 15; break;         // >▖<
			case 'f': s[i] = 9; break;          // >▗<
			case 'g': s[i] = 12; break;         // >▘<
			case 'h': s[i] = 11; break;         // >▝<

            // inverted versions of the above 4
			case 'i': s[i] = 15+128; break;     // >▜<
			case 'j': s[i] = 9+128; break;      // >▛<
			case 'k': s[i] = 12+128; break;     // >▟<
			case 'l': s[i] = 11+128; break;     // >▙<

			case 'm': s[i] = 32+128; break;     // >█< inverted space

            // don't have these in Atascii
			// case 'n': s[i] = 11+128; break;     // >▚<
			// case 'p': s[i] = 11+128; break;     // >▞<

			case '/': s[i] = 6; break;          // /
			case '\\': s[i] = 7; break;         // \

			default: break;
		}
	}
}