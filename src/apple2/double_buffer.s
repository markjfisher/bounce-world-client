        .export         _check_text_buffer_location
        .export         _show_other_screen
        .export         _swap_buffer

        .export         _is_alt_screen

        .importzp       ptr1
        .macpack        cpu

TEXTLN_HI = $29
SHOW_SCR1 = $C054
SHOW_SCR2 = $C055

.proc   _show_other_screen
        ldx     _is_alt_screen
        lda     SHOW_SCR1,x
        rts
.endproc

; we only need to swap the flag value on apple2, the TEXTLN_HI value has to be changed after a goto conio function, so a lot more frequently than atari
.proc   _swap_buffer
        lda     _is_alt_screen
        eor     #$01
        sta     _is_alt_screen
        rts
.endproc

; if we're on alternate screen, add 4 to TEXTLN_HI
.proc   _check_text_buffer_location
        ; check if we're on the primary screen
        lda     _is_alt_screen
        beq     done

        ; alternate screen currently being used
        ; add 4 to TEXTLN_HI (note 4 x INC $29 = 20 cycles, below is 8)
        clc
        lda     #$04
        adc     TEXTLN_HI
        sta     TEXTLN_HI

done:   rts

.endproc

.bss
_is_alt_screen:    .res 1
