#include <cmoc.h>
#include <coco.h>
#include <hirestxt.h>

#include "double_buffer.h"

/*
 * Double buffering for the CoCo.
 *
 * Buffer A is the standard PMODE 4 screen at $0E00. Buffer B is at $E000,
 * inside RAM that is normally hidden behind ROM:
 *   - CoCo 3: machine is always in all-RAM mode; $E000-$F7FF holds the
 *     Super Extended Basic RAM copy, unused while this program runs. It is
 *     saved to physical block $30 (lower 64K, untouched by Basic/HDB-DOS)
 *     at init and restored at exit, since Basic needs it back.
 *   - 64K CoCo 1/2: the Basic/HDB-DOS ROMs ($8000-$DFFF) are copied to the
 *     RAM underneath, then the SAM is switched to all-RAM mode, exposing
 *     free RAM at $E000. HDB-DOS keeps running from its identical RAM copy.
 *   - Anything else: db_mode stays 0 and these functions are no-ops.
 */

#define BUFFER_A ((byte *) 0x0E00)
#define BUFFER_B ((byte *) 0xE000)
#define BUFFER_SIZE 6144

#define DB_OFF         0
#define DB_COCO3       1
#define DB_COCO12_ROM  2  /* we switched to all-RAM mode; undo at exit */
#define DB_COCO12_RAM  3  /* machine was already in all-RAM mode */

uint8_t is_alt_screen;

static uint8_t db_mode = DB_OFF;

static byte *screen_buffers[2] = { BUFFER_A, BUFFER_B };

/* Probes whether $C000 is writable RAM, i.e. the machine is already in
   all-RAM mode (e.g. a DWLOAD-installed HDB-DOS). */
static byte ram_at_c000(void)
{
    byte result;
    asm
    {
        orcc    #$50
        ldb     $C000
        comb
        stb     $C000
        cmpb    $C000
        beq     @isram
        clra
        bra     @restore
@isram
        lda     #1
@restore
        comb
        stb     $C000       ; restore original byte (write is harmless in ROM mode)
        andcc   #$AF
        sta     :result
    }
    return result;
}

/* CoCo 1/2, ROM mode: checks for RAM at $E000 in all-RAM mode.
   Guards against 32K machines that mirror the upper half: if writes to
   $E000 track through to $6000 across both test patterns, it is a mirror,
   not real 64K. $6000 is saved and restored in case the writes landed there. */
static byte detect_64k(void)
{
    byte result;
    asm
    {
        orcc    #$50
        ldb     $6000
        pshs    b
        sta     $FFDF       ; all-RAM mode
        lda     #$55
        sta     $E000
        lda     $E000
        cmpa    #$55
        bne     @no64k
        lda     $6000
        cmpa    #$55
        bne     @yes64k     ; $6000 unaffected: $E000 is distinct RAM
        lda     #$AA
        sta     $E000
        lda     $E000
        cmpa    #$AA
        bne     @no64k
        lda     $6000
        cmpa    #$AA
        bne     @yes64k     ; $6000 tracked only by coincidence of first pattern
@no64k
        clrb
        bra     @restore
@yes64k
        ldb     #1
@restore
        sta     $FFDE       ; back to ROM mode
        puls    a
        sta     $6000
        andcc   #$AF
        stb     :result
    }
    return result;
}

/* CoCo 1/2, ROM mode: clones Extended Basic, Color Basic and the HDB-DOS
   cartridge (including the DriveWire vectors at $D93F/$D941) to the RAM
   underneath, then leaves the machine in all-RAM mode. Unlike the CoCo 3,
   the SAM discards writes to the upper 32K in map type 0, so each byte is
   read in ROM mode and stored after switching to RAM mode. Interrupts stay
   masked: in RAM mode the vectors are garbage until the copy reaches $BFFx. */
static void copy_roms_to_ram(void)
{
    asm
    {
        orcc    #$50
        ldx     #$8000
@cploop
        lda     ,x
        sta     $FFDF       ; all-RAM mode: expose RAM for the store
        sta     ,x+
        sta     $FFDE       ; ROM mode for the next read
        cmpx    #$E000
        blo     @cploop
        sta     $FFDF
        andcc   #$AF
    }
}

/* CoCo 3: copies the 6K at $E000 to/from physical block $30 through a
   temporary MMU window at $8000. GIME MMU registers are write-only, so the
   window slot is restored to $3C, the standard DECB task-0 mapping. */
static void secb_exchange(byte restore)
{
    asm { orcc #$50 }
    * (byte *) 0xFFA4 = 0x30;
    if (restore)
        memcpy((void *) 0xE000, (void *) 0x8000, BUFFER_SIZE);
    else
        memcpy((void *) 0x8000, (void *) 0xE000, BUFFER_SIZE);
    * (byte *) 0xFFA4 = 0x3C;
    asm { andcc #$AF }
}

void init_double_buffer(void)
{
    if (* (uint16_t *) 0xFFF8 == 0xFEF7)
    {
        secb_exchange(0);
        db_mode = DB_COCO3;
    }
    else if (ram_at_c000())
    {
        db_mode = DB_COCO12_RAM;
    }
    else if (detect_64k())
    {
        copy_roms_to_ram();
        db_mode = DB_COCO12_ROM;
    }
}

void shutdown_double_buffer(void)
{
    if (!db_mode)
        return;

    setPmodeGraphicsAddressEx(BUFFER_A, TRUE);
    setHiResTextBuffer(BUFFER_A);
    is_alt_screen = 0;

    if (db_mode == DB_COCO3)
    {
        secb_exchange(1);
    }
    else if (db_mode == DB_COCO12_ROM)
    {
        asm { sta $FFDE }
    }
    db_mode = DB_OFF;
}

void swap_buffer(void)
{
    if (!db_mode)
        return;
    is_alt_screen ^= 1;
    setHiResTextBuffer(screen_buffers[is_alt_screen]);
}

void show_other_screen(void)
{
    if (!db_mode)
        return;
    setPmodeGraphicsAddressEx(screen_buffers[is_alt_screen], TRUE);
}
