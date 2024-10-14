
PUBLIC _uint8_to_a
PUBLIC _uint8_to_a10
PUBLIC _uint8_to_a16

SECTION code_user

; _uint8_to_a10 is based on
; https://github.com/Zeda/Z80-Optimized-Routines/blob/master/conversion/itoa_8.z80
; modified to work with z88dk and i8080
; unsigned char version

;----------------------------------------------------------------------------
; extern void __CALLEE__  uint8_to_a10(unsigned char n, unsigned char *str, unsigned char base);
_uint8_to_a:
    pop     hl                  ; return address
    ex      (sp), hl            ; base
    ld      a, 10
    cp      l
    jr      z, _uint8_to_a10    ; base = 10

_uint8_to_a16:
    pop     hl                  ; return address
    pop     de                  ; str
    ex      (sp), hl            ; n

    ld      a, l
    rrca
    rrca
    rrca
    rrca
    and     0xf
    cp      10
    jr      c, x1
    add     7                   ; 'A' - '9' - 1
x1: add     '0'
    ld      (de), a
    inc     de

    ld      a, l
    and     0xf
    cp      10
    jr      c, x2
    add     7                   ; 'A' - '9' - 1
x2: add     '0'
    ld      (de), a
    inc     de

    xor     a
    ld      (de), a
    ret

;----------------------------------------------------------------------------
; extern void __CALLEE__  uint8_to_a10(unsigned char n, unsigned char *str);
_uint8_to_a10:
    pop     hl                  ; return address
    pop     de                  ; str
    ex      (sp), hl            ; n
    ex      de, hl              ; HL = str
    ld      a, e                ; A = n

;calculate 100s place, plus 1 for a future calculation
    ld      b,'0'
    cp      100
    jr      c, l1
    sub     100
    inc     b
    cp      100
    jr      c, l1
    sub     100
    inc     b
l1:
;calculate 10s place digit, +1 for future calculation
    ld      de, $0A2F
l2:
    inc     e
    sub     d
    jr      nc, l2
    ld      c, a

;Digits are now in D, C, A
; strip leading zeros!
    ld      a, '0'
    cp      b
    jr      z, l3
    ld      (hl),b
    inc     hl
    db      $FE  ; start of `cp *` to skip the next byte, turns into `cp $BB` which will always return nz and nc
l3:
    cp      e
    jr      z, l4
    ld      (hl),e
    inc     hl
l4:
    add     a,c
    add     a,d
    ld      (hl),a
    inc     hl
    ld      (hl),0

    ret

