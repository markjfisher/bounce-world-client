#include <conio.h>

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

	cgetc();
}