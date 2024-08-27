#ifndef BWC_SOUND_H
#define BWC_SOUND_H

// allow targets to initialise sound system
void init_sound();

// stop all sound channels
void stop_sound();

// start the sound of a collision
void sound_collision();

// void sound_client_event();
// void sound_object_event();

#endif // BWC_SOUND_H