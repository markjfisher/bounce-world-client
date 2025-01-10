        .export         _playfield_clr

        .import         _is_showing_info

        .include        "atari.inc"
        .importzp       ptr1


; just clears this many bytes (40 x SCREEN_HEIGHT)
; 880 is 22 rows, i.e. with the info rows
; 960 is 24 rows, i.e. without the info rows

;BW_SCREEN_SIZE_22 = 880 = 0x0370
;BW_SCREEN_SIZE_24 = 960 = 0x03C0

.proc _playfield_clr

        lda     _is_showing_info
        bne     clear_22

        ldy     #$bf                   ; low byte of BW_SCREEN_SIZE_24-1
        bne     clear_24

clear_22:
        ldy     #$6f                   ; low byte of BW_SCREEN_SIZE_22-1

clear_24:
        lda     SAVMSC
        sta     ptr1
        lda     SAVMSC+1
        clc
        adc     #$03                   ; high byte of count
        sta     ptr1+1

        lda     #$00
        ldx     #$03                   ; high byte of count

:       sta     (ptr1), y
        dey
        bne     :-
        sta     (ptr1), y

        dex
        bmi     :+
        dey
        dec     ptr1+1
        bne     :-              ; always. relying on high byte of screen memory never reducing to page 0

:       rts

.endproc