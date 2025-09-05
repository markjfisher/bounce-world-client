#ifndef _CMOC_VERSION_
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cmoc.h>
#include <coco.h>
#include "conio_wrapper.h"
#endif

#include "broadcast.h"
#include "data.h"
#include "screen.h"

#ifdef __PMD85__
#include "conio_wrapper.h"
#endif

#ifdef __APPLE2__
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

void broadcast() {
    char lineBuffer[MAX_WIDTH + 1]; // Buffer for a single line of text, +1 for null terminator
    uint8_t i, lineLen, wordLen, startCol, startRow = 4;
    char *msgPtr, *wordPtr;

    // Calculate starting column to center the box
    startCol = (SCREEN_WIDTH - MAX_WIDTH) / 2 - 1; // Subtracting 1 for the box border

    // Top border of the box
    GOTOXY(startCol, startRow++);
    cputc(CH_ULCORNER);
    for (i = 0; i < MAX_WIDTH; i++) cputc(CH_HLINE);
    cputc(CH_URCORNER);

    // Prepare message for word wrapping
    msgPtr = broadcast_message;
    lineBuffer[0] = '\0'; // Start with an empty line
    lineLen = 0;

    while (*msgPtr) {
        // Find the next space or end of message
        for (wordPtr = msgPtr; *wordPtr && *wordPtr != ' '; wordPtr++);
        wordLen = wordPtr - msgPtr;

        // Check if the word fits in the current line, including a space if not the first word
        if (lineLen + wordLen + (lineLen > 0 ? 1 : 0) > MAX_WIDTH) {
            // Fill the rest of the line with spaces for alignment
            while (lineLen < MAX_WIDTH) {
                lineBuffer[lineLen++] = ' ';
            }
            lineBuffer[lineLen] = '\0'; // Ensure null-termination

            // Print the current line with right border
            GOTOXY(startCol, startRow++);
            cputc(CH_VLINE);
            cputs(lineBuffer);
            cputc(CH_VLINE);

            // Reset the line buffer for the next line
            lineLen = 0;
        }

        // If not the first word in the line, add a space
        if (lineLen > 0 && (lineLen + wordLen) < MAX_WIDTH) {
            lineBuffer[lineLen++] = ' ';
        }

        // Copy the word to the line buffer
        strncpy(lineBuffer + lineLen, msgPtr, wordLen);
        lineLen += wordLen;

        // Ensure null-termination
        // lineBuffer[lineLen] = '\0';

        // Move to the next word, skipping spaces
        msgPtr = wordPtr;
        while (*msgPtr == ' ') msgPtr++;
    }

    // Fill the rest of the last line with spaces for alignment
    if (lineLen > 0) {
        while (lineLen < MAX_WIDTH) {
            lineBuffer[lineLen++] = ' ';
        }
        lineBuffer[lineLen] = '\0'; // Ensure null-termination

        // Print the last line with right border
        GOTOXY(startCol, startRow++);
        cputc(CH_VLINE);
        cputs(lineBuffer);
        cputc(CH_VLINE);
    }

    // Bottom border of the box
    GOTOXY(startCol, startRow);
    cputc(CH_LLCORNER);
    for (i = 0; i < MAX_WIDTH; i++) cputc(CH_HLINE);
    cputc(CH_LRCORNER);
}
