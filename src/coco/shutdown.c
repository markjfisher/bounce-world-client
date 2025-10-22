#include <cmoc.h>
#include <coco.h>
#include <conio_wrapper.h>

void cleanup_client() 
{
    closeHiResTextScreen();
    pmode(0, 0);
    screen(0, 0);
    cls(255);
}
