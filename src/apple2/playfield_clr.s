        .export   _playfield_clr

        .import   _is_alt_screen
        .importzp ptr1

; Custom clear screen for rows 0-21
; in either buffer.
; The apple doesn't store screen data in a simple continuous space, so we load the various offsets per row and clear 40 bytes from it.
; Thanks Woz.

.proc _playfield_clr
        ldx     #21                     ; 22 rows

l0:
        lda     scr_offsets_lo, x
        sta     ptr1
        lda     scr_offsets_hi, x

        ldy     _is_alt_screen
        beq     over                    ; if y = 0 we are on screen 1 of 2, so address is $0400

        clc
        adc     #$04                    ; otherwise it's $0800, which get by adding 4 to table high byte values.

over:
        sta     ptr1 + 1
        ; now zero out 40 bytes from ptr1
        ldy     #39
        lda     #$A0                    ; space char on screen
:       sta     (ptr1), y
        dey
        bpl     :-

        dex
        bpl     l0

        rts

.endproc

.data
; rows 0-21 address offsets from 400 to clear 40 bytes from its particular row.
.define scr_offsets $0400, $0480, $0500, $0580, $0600, $0680, $0700, $0780, $0428, $04a8, $0528, $05a8, $0628, $06a8, $0728, $07a8, $0450, $04d0, $0550, $05d0, $0650, $06d0

scr_offsets_lo:         .lobytes scr_offsets
scr_offsets_hi:         .hibytes scr_offsets
