#include <string.h>

#include "dw.h"
#include "fujinet-network.h"
#include "fujinet-network-pmd85.h"
#include "fujinet-fuji-pmd85.h"


uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans)
{
    struct _o
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t mode;
        uint8_t trans;
        char devicespec[256];
    } o;

    o.opcode = OP_NET;
    o.unit = network_unit(devicespec);
    o.cmd = 'O';
    o.mode = mode;
    o.trans = trans;
    strcpy(o.devicespec,devicespec);

    bus_ready();
    dwwrite((uint8_t *)&o, sizeof(o));
    
    return net_error(o.unit) == BUS_SUCCESS ? FN_ERR_OK : FN_ERR_IO_ERROR;
}
