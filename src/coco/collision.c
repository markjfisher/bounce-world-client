#include "data.h"
#include "conio_wrapper.h"
#include "collision.h"
#include "sound.h"

void collision_fx()
{
    if (play_sound_on_collision)
    {
        sound_collision();
    }
    if (flash_on_collision)
    {
        // Use screen colors for flash effect
        for (int i = 0; i < 4; i++)
        {
            switch_colorset();
            delay((5)); 
        }
    }
}