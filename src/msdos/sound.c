#include <conio.h>
#include "delay.h"
#include "sound.h"

#define PIT_MODE_PORT  0x43
#define PIT_CH2_PORT   0x42
#define SPEAKER_PORT   0x61
#define PIT_FREQ       1193180UL

static void speaker_tone(uint16_t freq)
{
    uint16_t divisor = (uint16_t)(PIT_FREQ / freq);
    outp(PIT_MODE_PORT, 0xB6);
    outp(PIT_CH2_PORT, divisor & 0xFF);
    outp(PIT_CH2_PORT, (divisor >> 8) & 0xFF);
    outp(SPEAKER_PORT, (uint8_t)(inp(SPEAKER_PORT) | 0x03));
}

static void speaker_off(void)
{
    outp(SPEAKER_PORT, (uint8_t)(inp(SPEAKER_PORT) & 0xFC));
}

void init_sound(void) { }
void stop_sound(void) { speaker_off(); }

// Descending three-tone "thud" played in ~165ms (3 timer ticks at 18.2 Hz).
// The steps are perceived as one continuous impact rather than three beeps.
void sound_collision(void)
{
    speaker_tone(1200);
    pause(1);
    speaker_tone(600);
    pause(1);
    speaker_tone(220);
    pause(1);
    speaker_off();
}
