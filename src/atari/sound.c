#include <atari.h>
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "data.h"
#include "delay.h"
#include "debug.h"
#include "sound.h"

// void __fastcall__ _sound (unsigned char voice, unsigned char pitch, unsigned char distortion, unsigned char volume);

// collision sound volumes for the 3 channels. based on EXPLOSION from https://www.atariarchives.org/creativeatari/An_Atari_Library_of_Sound.php
// each volume is held for 2 v_syncs, to total about 44 jiffies, or about 0.7s
// in the original, the sound is held slightly longer, as it fades away, and has an initial note in voice 2, both are omit for simplicity.
// all arrays end with 0 which turns sound off as well.
uint8_t sc0[] = {15, 12, 10,  8,  7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0};
uint8_t sc1[] = {15, 13, 11, 10,  9, 7, 7, 6, 5, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0};
uint8_t sc2[] = {15, 14, 12, 11, 10, 9, 8, 7, 6, 6, 5, 5, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 0};

bool is_playing_collision = false;
uint8_t current_volume_index = 0;

extern void init_sound_vbi();

void note(uint8_t n, uint8_t n2, uint8_t n3, uint8_t d, uint8_t f, uint8_t p) {
  static uint8_t i;

  _sound(0 , n, 10, 8);
  if (n2)
    _sound(1, n2, 10, 6);
  if (n3)
    _sound(2, n3, 10, 4);

  pause(d);

  for (i = 7; i < 255; i--) { 
    _sound(0, n, 10, i);
    if (n2 && i > 1) {
	    _sound(1, n2, 10, i - 2);
	}
    if (n3 && i > 3) {
	    _sound(2, n3, 10, i - 4);
	}
    pause(f);
  }
  pause(p);
}

void init_sound() {
	OS.soundr = 0;
	// setup a deferred VBI for continuing the sound
	// init_sound_vbi();
}

void stop_sound() {
	_sound(0, 0, 0, 0);
	_sound(1, 0, 0, 0);
	_sound(2, 0, 0, 0);
	// _sound(3, 0, 0, 0);
}

// void sound_join_game() {
// 	uint8_t j;
// 	for(j=0;j<2;j++) {
// 		note(81,0,0,0,1,0);
// 		if (j==0)
// 			note(96,0,0,0,1,0);
// 	}
// }

void sound_collision() {
	// if (is_playing_collision) {
	// 	stop_sound();
	// }
	is_playing_collision = true;
	// initialise the collision values, set the clock to 0 so we can track jiffies
	current_volume_index = 0;
	_sound(0,  80, 8, 15);
	// _sound(1, 100, 8, 15);
	// _sound(2, 130, 8, 15);
}
