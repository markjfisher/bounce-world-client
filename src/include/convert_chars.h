#ifndef BWC_CONVERT_CHARS_H
#define BWC_CONVERT_CHARS_H

// Platform specific character conversion from the "neutral" format to chars to display
// The string contains mappings as follows on the server side, we need functions to convert them back to local chars to print.
// Chars not in this list are passed through as-is

/*
	'┌' -> 'r'.code
	'┐' -> ')'.code
	'└' -> 'L'.code
	'┘' -> '!'.code
	'┤' -> 'J'.code
	'├' -> 't'.code
	'┬' -> 'T'.code
	'┴' -> '2'.code
	'│' -> '|'.code
	'─' -> '-'.code
	'┼' -> '+'.code
	'▌' -> 'a'.code
	'▐' -> 'b'.code
	'▄' -> 'c'.code
	'▀' -> 'd'.code
	'▖' -> 'e'.code
	'▗' -> 'f'.code
	'▘' -> 'g'.code
	'▝' -> 'h'.code
	'▜' -> 'i'.code
	'▛' -> 'j'.code
	'▟' -> 'k'.code
	'▙' -> 'l'.code
	'█' -> 'm'.code 	// whole inverted space
	'▚' -> 'n'.code		// skip 'o' so it can be used in shapes
	'▞' -> 'p'.code
 */

void convert_chars(uint8_t *data, uint8_t len);


#endif // BWC_CONVERT_CHARS_H