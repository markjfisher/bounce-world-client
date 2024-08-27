        .export   _dli
        .export   dli_is_lower

        .include  "atari.inc"

; This dli makes the background BLACK for the graphics area at top of screen, and BLUE for the text area in last 4 lines
; The DLI instructions are added in dlist.c

.proc _dli
        pha                     ; store A while we do our routine

        lda     dli_is_lower
        sta     WSYNC           ; ensure we're at start of scan line for color change
        bne     do_lower

        sta     COLPF2
        inc     dli_is_lower    ; make it 1, for next iteration
        bne     done            ; always

do_lower:
        lda     #$94            ; blue
        sta     COLPF2
        dec     dli_is_lower    ; set to 0 for next iteration

done:   pla                     ; restore A
        rti                     ; and end DLI

.endproc

.data
dli_is_lower:   .byte 0
