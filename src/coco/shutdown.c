#include <cmoc.h>
#include <coco.h>
#include <conio_wrapper.h>

void cleanup_client()
{
    shutdown_double_buffer();
    closeHiResTextScreen();
    pmode(0, 0);
    screen(0, 0);
    cls(255);
}
