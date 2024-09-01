        .export   _dli
        .export   _enable_dli
        .export   current_section

        .import   _is_flashing_screen
        .import   _debug

        .import   _wait_vsync

        .include  "atari.inc"

; This dli makes the background BLACK for the graphics area at top of screen, and BLUE for the text area in last 4 lines
; The DLI instructions are added in dlist.c

.proc _dli
        pha                     ; store A while we do our routine

        lda     _is_flashing_screen
        bne     :+

        ; the screen flash is just changing background for a few frames, so skip this if it is flashing
        lda     #$00
        sta     COLBK

:       lda     current_section ; which part of the screen are we in? 0 means we have just done a VBI and reset
        sta     WSYNC           ; ensure we're at start of scan line for color change
        bne     section_1

section_0:
        lda     #$94            ; blue at bottom

        .byte   $2C             ; BIT [2 bytes] (skips the next LDA)
        ;bne     set_and_inc

section_1:
        lda     #$28            ; orange

set_and_inc:
        sta     COLPF2
        inc     current_section

done:   lda     #$0F            ; bright white
        sta     COLPF1          ; text colour

        pla                     ; restore A
        rti                     ; and end DLI

.endproc

.proc _enable_dli
        ; has to be done at the beginning of the screen to ensure the DLI at top
        ; of the screen is fired first, and not the 2nd halfway down the screen.
        jsr     _wait_vsync
        lda     #$C0
        sta     NMIEN
        rts
.endproc

.data
current_section:        .byte 0
