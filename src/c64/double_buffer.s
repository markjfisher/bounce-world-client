        .export         _show_other_screen
        .export         _swap_buffer

        .export         _is_orig_screen_mem

        .importzp       ptr1

.proc   _swap_buffer
        rts
.endproc

.proc   _show_other_screen
        rts
.endproc


.bss
_is_orig_screen_mem:    .res 1
