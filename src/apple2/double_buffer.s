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
        ; x = 1 - x for binary value swaps between 0 and 1.
        ; On 6502, this is quicker than doing test for 1 or 0 and then swapping it (12 + 13) / 2
        ; On 65c02 this is 1 cycle slower on average (10 + 12) / 2, so take the hit for simpler code
        sec                                     ; 2
        lda     #$01                            ; 2 (4)
        sbc     _is_orig_screen_mem             ; 4 (8)
        sta     _is_orig_screen_mem             ; 4 (12)
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
        lda     #$04
        adc     TEXTLN_HI
        sta     TEXTLN_HI

done:   rts

.endproc

.bss
_is_orig_screen_mem:    .res 1
