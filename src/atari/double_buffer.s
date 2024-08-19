        .export         _show_other_screen
        .export         _swap_buffer

        .export         _d1
        .export         _dlist_scr_ptr
        .export         _screen_mem_orig
        .export         _is_orig_screen_mem

        .importzp       ptr1

SAVMSC = $58

.proc   _swap_buffer
        lda     _is_orig_screen_mem
        beq     :+

        lda     #$00
        sta     _is_orig_screen_mem
        lda     #<_d1
        ldx     #>_d1
        bne     over            ; always true, temp buffer is never in ZP

:
        lda     #$01
        sta     _is_orig_screen_mem
        lda     _screen_mem_orig
        ldx     _screen_mem_orig+1

over:
        sta     SAVMSC
        stx     SAVMSC+1
        rts

.endproc

.proc   _show_other_screen
        ; copy SAVMSC values into location pointed to by _dlist_scr_ptr, which will flip memory of dlist to new screen
        lda     _dlist_scr_ptr
        sta     ptr1
        lda     _dlist_scr_ptr+1
        sta     ptr1+1

        ldy     #$00
        lda     SAVMSC
        sta     (ptr1), y
        iny
        lda     SAVMSC+1
        sta     (ptr1), y
        rts
.endproc


.bss
_d1:                    .res 960      ; full 40x24 for screen data
_dlist_scr_ptr:         .res 2
_screen_mem_orig:       .res 2
_is_orig_screen_mem:    .res 1