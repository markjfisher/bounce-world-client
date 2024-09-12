#include <dw.h>
#include <fujinet-network.h>
#include <fujinet-fuji-pmd85.h>
#include <fujinet-network-pmd85.h>

uint8_t network_close(char* devicespec)
{
    // struct _nc
    // {
    //     uint8_t opcode;
    //     uint8_t unit;
    //     uint8_t cmd;
    // } nc;

    // nc.opcode = OP_NET;
    // nc.unit = network_unit(devicespec);
    // nc.cmd = 'C';

    // bus_ready();

    // dwwrite((uint8_t *)&nc, sizeof(nc));
    
    // return net_error(nc.unit) == BUS_SUCCESS ? FN_ERR_OK : FN_ERR_IO_ERROR;
    struct _closecmd
    {
        uint8_t opcode;
        uint8_t id;
        uint8_t command;
        uint8_t aux1;
        uint8_t aux2;
    } cc;

    cc.opcode = OP_NET;
    cc.id = network_unit(devicespec);
    cc.command = 'C';
    cc.aux1 = cc.aux2 = 0;

    // Wait for ready
    bus_ready();

    // Send command
    dwwrite((uint8_t *)&cc, sizeof(cc));

    return FN_ERR_OK;
}
