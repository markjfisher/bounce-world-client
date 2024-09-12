#include "dw.h"
#include "fujinet-fuji-pmd85.h"
#include "fujinet-network-pmd85.h"
#include "fujinet-network.h"

uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len)
{
    struct _w
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint16_t len;
    } w;

    w.opcode = OP_NET;
    w.unit = network_unit(devicespec);
    w.cmd = 'W';
    w.len = ((len << 8) | (len >> 8));

    bus_ready();
    dwwrite((uint8_t *)&w,sizeof(w));
    dwwrite((uint8_t *)buf, len);
    
    // return bus_error(OP_NET) == BUS_SUCCESS;
    return FN_ERR_OK;
}
