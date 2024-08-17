        .export _set_dlist
        .export _swap_buffer

        .import _d1
        .import _dlist_scr_ptr
        .import _is_orig_screen_mem
        .import _screen_mem_orig

        .importzp ptr1

SAVMSC = $58

.proc   _swap_buffer
        lda     _is_orig_screen_mem
        beq     :+

        lda     #<_d1
        ldx     #>_d1
        dec     _is_orig_screen_mem
        beq     over

:
        lda     _screen_mem_orig
        ldx     _screen_mem_orig+1
        inc     _is_orig_screen_mem

over:
        sta     SAVMSC
        stx     SAVMSC+1
        rts

.endproc

.proc   _set_dlist
        ; copy SAVMSC values into location pointed to by _dlist_scr_ptr
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
