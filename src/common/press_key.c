#ifndef _CMOC_VERSION_
#include <conio.h>
#else
#include "conio_wrapper.h"
#endif

#include "delay.h"
#include "world.h"

#ifdef __PMD85__
#include "conio_wrapper.h"
#endif

void press_key() {
	
	chlinexy(6, 20, 28);
	revers(1);
	gotoxy(8, 21);
	cputs("Press a key to continue");
	revers(0);
	chlinexy(6, 22, 28);

	// we will periodically ping the server while waiting for the user to press a key
	// so that the client does not time out and get removed from the server
	while (kbhit() == 0) {
		fetch_client_state();
		// 20/60th of a second - enough to stop spamming, but react to user input fast enough.
		pause(20);
	}

	// fetch the key so it doesn't get act upon
#ifndef _CMOC_VERSION_
	cgetc();
#endif
}
