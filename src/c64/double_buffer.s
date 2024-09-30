        .export         _show_other_screen
        .export         _swap_buffer

        .export         _is_alt_screen

        .importzp       ptr1

.proc   _swap_buffer
        rts
.endproc

.proc   _show_other_screen
        rts
.endproc


.bss
_is_alt_screen:    .res 1
