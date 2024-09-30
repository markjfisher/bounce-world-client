#include <dw.h>
#include <fujinet-network-pmd85.h>
#include <fujinet-network.h>
#include <fujinet-fuji-pmd85.h>

uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
{
    struct _s
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t aux1;
        uint8_t aux2;
    } s;

    struct _sr
    {
        uint16_t bw;
        uint8_t c;
        uint8_t err;
    } sr;

    s.opcode = OP_NET;
    s.unit = network_unit(devicespec);
    s.cmd = 'S';
    s.aux1 = s.aux2 = 0;

    bus_ready();
    dwwrite((uint8_t *)&s, sizeof(s));
    net_get_response(s.unit, (uint8_t *)&sr, sizeof(sr));

    *bw = ((sr.bw << 8) | (sr.bw >> 8));
    *c = sr.c;
    *err = sr.err;
    
    //return net_error(s.unit) == BUS_SUCCESS ? FN_ERR_OK : FN_ERR_IO_ERROR;
    return FN_ERR_OK;
}
