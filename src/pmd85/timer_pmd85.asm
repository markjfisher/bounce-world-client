; extern void __FASTCALL__ reset_timer(unsigned char c);
PUBLIC _reset_timer

; extern unsigned int __FASTCALL__ read_timer(unsigned char c);
PUBLIC _read_timer

;----------------------------------------------------------------------------
; extern void __FASTCALL__ reset_timer(unsigned char c);
_reset_timer:
        ; ld      a, 0x70 ;112
        ; out     95
        ; xor     a
        ; out     93
        ; out     93
        ; ret

        ld      a, l
        cp      3
        ret     nc
        add     0x5c
        ld      (out1+1), a     ; self-mod code
        ld      (out2+1), a     ; self-mod code
        ld      a, l
        rrca
        rrca
        or      0x30
        out     0x5f
        xor     a
out1:   out     0x5c            ; self-modded
out2:   out     0x5c            ; self-modded
        ret


;----------------------------------------------------------------------------
; extern unsigned int __FASTCALL__ read_timer(unsigned char c);
_read_timer:
        ; ld      a, 0x40
        ; out     0x5f            ; latch current value
        ; in      93
        ; ld      l, a
        ; in      93
        ; ld      h, a
        ; ret

        ld      a, l
        cp      3
        ret     nc
        rrca
        rrca
        out     0x5f            ; latch current value
        ld      a, l
        add     0x5c
        ld      (in1+1), a      ; self-mod code
        ld      (in2+1), a      ; self-mod code
in1:    in      0x5c            ; self-modded code, read latch, low byte
        ld      l, a
in2:    in      0x5c            ; self-modded code, read latch, high byte
        ld      h, a
        ret

