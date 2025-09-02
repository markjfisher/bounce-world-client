        .export         _pause
        .export         _wait_vsync

        .import         _waitvsync

        .include        "apple2.inc"

; void wait_vsync();
;
_wait_vsync:
        jsr     _waitvsync
        rts

; void pause(uint8_t count);
;
; does 'count' vsync waits

_pause:
        tax
:       jsr     _wait_vsync
        dex
        bne     :-
        rts
