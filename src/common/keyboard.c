#include <conio.h>
#include <screen.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

void change_speed(bool is_increase) {

}

void freeze_world() {

}

void add_body(uint8_t size) {
	gotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT + 1);
	cputs("adding body");
}

void toggle_wrapping() {

}

void handle_kb() {
	char c;
	if (kbhit() == 0) return;

	c = cgetc();
	switch (c) {
		case '+': change_speed(true); break;
		case '-': change_speed(false); break;
		case 'F': freeze_world(); break;
		
		case '1':
		case '2':
		case '3':
		case '4':
		case '5': add_body(c - '0'); break;

		case 'W': toggle_wrapping(); break;

		default: break;
	}
}