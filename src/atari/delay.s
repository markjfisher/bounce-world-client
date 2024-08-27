        .export _pause
        .export _wait_vsync

        .include "atari.inc"

.proc _wait_vsync
:       lda     VCOUNT
        bne     :-
        rts
.endproc

.proc _pause
        tax

:       jsr     _wait_vsync
        dex
        bne     :-
        rts
.endproc