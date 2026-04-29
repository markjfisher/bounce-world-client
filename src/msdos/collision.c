#include "data.h"
#include "collision.h"
#include "sound.h"

void collision_fx(void)
{
    if (play_sound_on_collision) {
        sound_collision();
    }
}
