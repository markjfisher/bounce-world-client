        .export         _check_text_buffer_location
        .export         _show_other_screen
        .export         _swap_buffer

        .export         _is_orig_screen_mem

        .importzp       ptr1
        .macpack        cpu

TEXTLN_HI = $29
SHOW_SCR1 = $C054
SHOW_SCR2 = $C055

.proc   _show_other_screen
        lda     _is_orig_screen_mem
        beq     swap_to_screen2

.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     SHOW_SCR1
.else
        lda     #$00
        sta     SHOW_SCR1
.endif
        rts

swap_to_screen2:
        sta     SHOW_SCR2
        rts

.endproc

; we only need to swap the flag value on apple2, the TEXTLN_HI value has to be changed after a goto conio function, so a lot more frequently than atari
.proc   _swap_buffer
        lda     _is_orig_screen_mem
        beq     :+

.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     _is_orig_screen_mem
.else
        lda     #$00
        sta     _is_orig_screen_mem
.endif
        rts

:       lda     #$01
        sta     _is_orig_screen_mem
        rts

.endproc

; if we're on alternate screen, add 4 to TEXTLN_HI
.proc   _check_text_buffer_location
        ; check if we're on the primary screen
        lda     _is_orig_screen_mem
        bne     done

        ; alternate screen currently being used
        ; add 4 to TEXTLN_HI (note 4 x INC $29 = 20 cycles, below is 8)
        clc
        lda     TEXTLN_HI
        adc     #$04
        sta     TEXTLN_HI

done:   rts

.endproc

.bss
_is_orig_screen_mem:    .res 1
