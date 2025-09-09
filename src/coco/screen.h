#ifndef BWC_SCREEN_H
#define BWC_SCREEN_H

// these are the dimensions for the current platform, height should be half the width, allowing some space for text messages below 
#ifdef COCO2_BUILD
#define SCREEN_WIDTH (32)
#define SCREEN_HEIGHT (16)
#else
#define SCREEN_WIDTH (40)
#define SCREEN_HEIGHT (24)
#endif
#endif // BWC_SCREEN_H
