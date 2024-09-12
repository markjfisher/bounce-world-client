#ifndef __PMD85__
#include <cc65.h>
#endif
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "app_errors.h"
#include "fujinet-network.h"

#ifdef __PMD85__
#include "conio_wrapper.h"
#endif

uint8_t err = 0;

void handle_err(char *reason) {
    if (err) {
        gotoxy(0, 20);
        cputs("Error: ");
        cputs(reason);

        if (doesclrscrafterexit()) {
            cgetc();
        }
        exit(1);
    }
}