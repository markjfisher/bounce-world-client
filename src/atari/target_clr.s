        .export         _target_clr

        .include        "atari.inc"
        .importzp       ptr1


; just clears 800 (40x20) bytes of the screen
BW_SCREEN_SIZE = 800

.proc _target_clr

        lda     SAVMSC
        sta     ptr1
        lda     SAVMSC+1
        clc
        adc     #>(BW_SCREEN_SIZE-1)
        sta     ptr1+1

        lda     #$00
        ldy     #<(BW_SCREEN_SIZE-1)
        ldx     #>(BW_SCREEN_SIZE-1)

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