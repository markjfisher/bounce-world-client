/**
 * @brief   Return error code of previous sub device operation
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose opcode is OP_FUJI or OP_NET
 */

#include <dw.h>
#include <fujinet-fuji-pmd85.h>

uint8_t bus_error(uint8_t opcode)
{
    struct _senderrorcmd
    {
        uint8_t opcode;
        uint8_t cmd;
    } sec;

    uint8_t z = 0;
    uint8_t err = 0;
    
    sec.opcode = opcode;
    sec.cmd = FUJICMD_SEND_ERROR;

    bus_ready();
    dwwrite((uint8_t *)&sec, sizeof(sec));
    z = dwread((uint8_t *)&err, sizeof(err));

    return z ? err : BUS_ERROR;
}

uint8_t net_error(uint8_t devid)
{
    struct _errcmd
    {
        uint8_t opcode;
        uint8_t id;
        uint8_t command;
        uint8_t aux1;
        uint8_t aux2;
    } ec;

    uint8_t z = 0;
    uint8_t err = 0;
    
    ec.opcode = OP_NET;
    ec.id = devid;
    ec.command = FUJICMD_SEND_ERROR;
    ec.aux1 = ec.aux2 = 0;

    bus_ready();
    dwwrite((uint8_t *)&ec, sizeof(ec));
    z = dwread(&err, 1); // returns 1 on success, 0 on error
   
    return z ? err : BUS_ERROR;
}
