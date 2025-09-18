#if !defined(__PMD85__) && !defined(_CMOC_VERSION_) && !defined(__ADAM__)
#include <cc65.h>
#endif
#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#include "conio_wrapper.h"
#else
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#endif

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
