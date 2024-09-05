#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "broadcast.h"
#include "data.h"
#include "screen.h"

#ifdef __APPLE2__
#include <apple2enh.h>
#include "conio_helpers.h"
#define GOTOXY gotoxy_buffer
#define CPUTSXY cputsxy_buffer
#define CPUTCXY cputcxy_buffer
#else
#define GOTOXY gotoxy
#define CPUTSXY cputsxy
#define CPUTCXY cputcxy
#endif

// for the corner chars
#if defined(__ATARI__)
#include <atari.h>
#elif defined(__CBM__)
#include <cbm.h>
#endif

// How large the box can be for text. Add 2 for borders
#define MAX_WIDTH 22

// Helper function to print the horizontal border of the box
void print_horizontal_border(int start_x, int start_y, int width) {
	int i;
    CPUTCXY(start_x, start_y, CH_ULCORNER);
    for (i = 1; i < width - 1; i++) {
        cputc(CH_HLINE);
    }
    cputc(CH_URCORNER);
}

// Helper function to print spaces
void print_spaces(int x, int y, int count) {
	int i;
	GOTOXY(x, y);
    for (i = 0; i < count; i++) {
        cputc(' ');
    }
}

// Main function to display the wrapped message
void show_broadcast() {
	int i;
	int wordlen, space_width;
	const char *word;
	const char *space;
	int len;
	int box_width;
	int start_x, start_y;
	int x, y;

    len = strlen(broadcast_message);
	if (len < MAX_WIDTH) {
		box_width = len + 2;		
	} else {
		// +2 for the vertical borders
	    box_width = MAX_WIDTH + 2;
	}

    start_x = (SCREEN_WIDTH - box_width) / 2;
    start_y = 5;

    x = start_x + 1; // One space after the left border
    y = start_y + 1; // One line below the top border

    // Print top border
    print_horizontal_border(start_x, start_y, box_width);

    word = broadcast_message;
    space = strchr(broadcast_message, ' ');

    while (word) {
        wordlen = space ? (space - word) : strlen(word);

        if (x + wordlen - start_x > MAX_WIDTH) {
            // Fill the rest of the line with spaces
            print_spaces(x, y, start_x + MAX_WIDTH - x + 1);

            // Move to the next line
            y++;
            x = start_x + 1;

            // Print left border for the new line
            CPUTCXY(start_x, y, CH_VLINE);
        }

        // Print the word
		GOTOXY(x, y);
        for (i = 0; i < wordlen; i++) {
            CPUTCXY(x + i, y, word[i]);
        }

        // Update x position
        x += wordlen;

        if (space) {
            space_width = (x + 1 - start_x <= MAX_WIDTH) ? 1 : 0;
            print_spaces(x, y, space_width); // Print a space if the next word fits
            x += space_width; // Move x position after the space
            word = space + 1; // Move to the next word
            space = strchr(word, ' '); // Find the next space
        } else {
            word = NULL; // No more words
        }
    }

    // Fill the rest of the line with spaces
    print_spaces(x, y, start_x + MAX_WIDTH - x + 1);

    // Print bottom border
    y++;
    print_horizontal_border(start_x, y, box_width);

    // Print the left and right borders
    for (i = start_y + 1; i < y; i++) {
        CPUTCXY(start_x, i, CH_VLINE);
        CPUTCXY(start_x + box_width - 1, i, CH_VLINE);
    }

    // Print the bottom corners
    CPUTCXY(start_x, y, CH_LLCORNER);
    CPUTCXY(start_x + box_width - 1, y, CH_LRCORNER);
}