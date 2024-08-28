        .export _pause
        .export _wait_vsync

        .include "atari.inc"

.proc _wait_vsync
:       lda     VCOUNT
        bne     :-

; you have to wait for 1 now, else pause will skip immediately as VCOUNT is still 0
:       lda     VCOUNT
        beq     :-
        rts
.endproc

; void pause(uint8_t jiffies)
;
; each vblank is 1 jiffy, and we wait for that many to occur.
; NTSC: 59.9227 Hz (6 = 0.1s, 20 = 0.33s)
;  PAL: 49.8607 Hz (5 = 0.1s, 20 = 0.40s)

.proc _pause
        tax

:       jsr     _wait_vsync
        dex
        bne     :-
        rts
.endproc