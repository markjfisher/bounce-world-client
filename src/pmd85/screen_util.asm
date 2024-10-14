
EXTERN generic_console_xypos
EXTERN _bounce_charset

PUBLIC _video_ptr

PUBLIC _screen_fill
PUBLIC _screen_set_region
PUBLIC _screen_fill_region

PUBLIC _blit_char, _blit_char_upper
PUBLIC _blit_char_from
PUBLIC _clear_char

;----------------------------------------------------------------------------

;; video ram
defc    DISPLAY     = 0xC000

;----------------------------------------------------------------------------

SECTION data_user

;----------------------------------------------------------------------------

_video_ptr:
    dw DISPLAY

fill_pattern:
    dw 0

fill_mask:
    dw 0

region_wh:
    dw 0

line_count:
    db 0

;----------------------------------------------------------------------------

SECTION code_user

;----------------------------------------------------------------------------
;; extern void __FASTCALL__ screen_fill(int pattern);
_screen_fill:
    ld      (fill_pattern), hl
    ld      hl, DISPLAY
    ld      c, 0x80
    ld      de, 16
loop1:
    ld      b, 48
    ld      a, (fill_pattern)
loop2:
    ld      (hl), a
    inc     hl
    djnz    loop2
    add     hl, de
    ld      b, 48
    ld      a, (fill_pattern+1)
loop3:
    ld      (hl), a
    inc     hl
    djnz    loop3
    add     hl, de
    dec     c
    jr      nz, loop1
    ret

;----------------------------------------------------------------------------

;; extern void __CALLEE__  screen_set_region(char x, char y, char w, char h);
_screen_set_region:
    pop     hl      ; return address
    pop     de      ; h
    pop     bc      ; w
    ld      d, c    ; d = width, e = height
    pop     bc      ; y
    ex      (sp), hl ; x
    ld      b, c    ; b = y
    ld      c, l    ; c = x
; de = width, height
; bc = y, x
screen_set_region_asm:
    call    generic_console_xypos
    ld      (_video_ptr), hl
    ex      de, hl
    ld      (region_wh), hl
    ret


;----------------------------------------------------------------------------

;; extern void __FASTCALL__ screen_fill_region(long pattmask);
_screen_fill_region:
    ; save fill pattern
    ld      (fill_pattern), hl
    ex      de, hl
    ; save fill mask
    ld      (fill_mask), hl
    ; preapre variables for loop
    ld      hl, (region_wh)
    ld      a, l
    rlc
    rlc
    ld      c, a    ; c = number of lines to fill / 2 = text rows * (8 / 2)
    ld      a, 64
    sub     a, h    ; 64 - text columns
    ld      e, a    ; e = offset to next line
    ld      hl, (_video_ptr)
floop1:
    ; even line
    ld      a, (region_wh+1)
    ld      b, a    ; b = number of bytes to fill
    ld      a, (fill_pattern) ; even line pattern
    ld      d, a    ; d = pattern
floop2:
    ld      a, (fill_mask)
    and     a, (hl) ; apply mask
    xor     a, d    ; apply pattern
    ld      (hl), a
    inc     hl
    djnz    floop2
    ld      d, 0
    add     hl, de  ; next line

    ; odd line
    ld      a, (region_wh+1)
    ld      b, a    ; b = number of bytes to fill
    ld      a, (fill_pattern+1) ; odd line pattern
    ld      d, a    ; d = pattern
floop3:
    ld      a, (fill_mask)
    and     a, (hl) ; apply mask
    xor     a, d    ; apply pattern
    ld      (hl), a ; fill
    inc     hl
    djnz    floop3
    ld      d, 0
    add     hl, de  ; next line
    dec     c       ; decrement line pair counter
    jr      nz, floop1
    ret


;----------------------------------------------------------------------------
;; extern void __FASTCALL__ blit_char(unsigned char c);
; L = char
; video_ptr = destination
_blit_char_upper:
    ld      a, l
    cp      97      ; 'a'
    jr      c, _blit_char
    cp      123     ; 'z'
    jr      nc, _blit_char
    sub     32
    ld      l, a
    ; continue to _blit_char
_blit_char:
    ; DE = charset base
    ld      de, _bounce_charset
    ; L = char
    ; HL *= 8
    ld      h, 0
    add     hl, hl
    add     hl, hl
    add     hl, hl
    ; HL += DE
    add     hl, de
;; extern void __FASTCALL__ blit_char_from(unsigned char *);
; HL = source data ptr
_blit_char_from:
    ; BC = 64 (bytes per line)
    ld      bc, 64
    ; DE = src*
    ex      de, hl
    ; HL = dst*
    ld      hl, (_video_ptr)
    ; copy char to screen
    ld      a, (de)
    ld      (hl), a
    inc     de
    add     hl, bc
    ld      a, (de)
    ld      (hl), a
    inc     de
    add     hl, bc
    ld      a, (de)
    ld      (hl), a
    inc     de
    add     hl, bc
    ld      a, (de)
    ld      (hl), a
    inc     de
    add     hl, bc
    ld      a, (de)
    ld      (hl), a
    inc     de
    add     hl, bc
    ld      a, (de)
    ld      (hl), a
    ret

;----------------------------------------------------------------------------

;; extern void __FASTCALL__ clear_char(void);
_clear_char:
    xor     a
    ; BC = 64 (bytes per line)
    ld      bc, 64
    ; HL = dst*
    ld      hl, (_video_ptr)
    ; zeros to screen
    ld      (hl), a
    add     hl, bc
    ld      (hl), a
    add     hl, bc
    ld      (hl), a
    add     hl, bc
    ld      (hl), a
    add     hl, bc
    ld      (hl), a
    add     hl, bc
    ld      (hl), a
    xor     a           ; set Z flag
    ret

;----------------------------------------------------------------------------
