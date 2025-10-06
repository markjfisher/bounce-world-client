#include <cmoc.h>
#include <coco.h>
#include <conio_wrapper.h>

void cleanup_client() {
    pmode(0, 0);
    cls(255);
    screen(0, 0);
}
