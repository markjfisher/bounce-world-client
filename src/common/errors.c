#include <cc65.h>
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "app_errors.h"
#include "fujinet-network.h"

uint8_t err = 0;

void handle_err(char *reason) {
    if (err) {
        clrscr();
        gotoxy(0, 0);
        cputs("Error: ");
        cputs(reason);

        if (doesclrscrafterexit()) {
            cgetc();
        }
        exit(1);
    }
}