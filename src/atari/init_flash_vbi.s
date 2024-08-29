        .export    _init_flash_vbi

        .import    _debug

        .import    _current_flash_time
        .import    _is_flashing_screen

        .include   "atari.inc"

.proc _init_flash_vbi
        ; jsr     _debug
        ldy     #<_continue_flash
        ldx     #>_continue_flash
        ; immediate VBI, gives us 2000 cycles to play with.
        lda     #$06
        jmp     SETVBV
.endproc

.proc _continue_flash
        ; are we flashing screen?
        lda     _is_flashing_screen
        beq     done

        ldy     _current_flash_time
        cmp     #20
        bcs     end_flash

        lda     flash_data, y
        sta     COLBK
        inc     _current_flash_time
        bne     done

end_flash:
        lda     #$00
        sta     _is_flashing_screen

done:
        jmp     SYSVBV

.endproc

.data
flash_data:     .byte 15, 13, 11, 9, 8, 7, 6, 5, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0