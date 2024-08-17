        .export _wait_vsync
        ; .export _pause

        .include "atari.inc"
        .include "zeropage.inc"

; .proc _pause
;         sta    tmp1

;         lda     #$00
;         sta     RTCLOK+2

; :       lda     RTCLOK+2
;         cmp     tmp1
;         bne     :-

;         rts
; .endproc

.proc _wait_vsync
:       lda VCOUNT
        bne :-

; :       lda VCOUNT
;         beq :-
        rts
.endproc