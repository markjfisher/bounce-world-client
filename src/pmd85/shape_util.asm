
;; from screen_util
EXTERN _video_ptr, _blit_char, _clear_char

;; from text_buffer
EXTERN buf_x, buf_y, x2flag
EXTERN buf_ptr, row_ptr

PUBLIC _blit_shape
PUBLIC _clear_shape

PUBLIC _clear_rect_tb:
PUBLIC _print_shape_tb

;----------------------------------------------------------------------------

SECTION data_user

;----------------------------------------------------------------------------

; pointer to shape characters
shape_data:
    dw  0
; width to draw
shape_width:
    db  0
; height to draw
shape_height:
    db  0
; how many characters to skip when starting next row
shape_skip_to_next:
    db  0
; width counter
shape_wcount:
    db  0
; saved _video_ptr
video_ptr_old:
    dw  0

;----------------------------------------------------------------------------

SECTION code_user

;----------------------------------------------------------------------------

_blit_shape:
    pop     hl      ; return address
    pop     bc      ; skip_to_next
    ld      a, c
    ld      (shape_skip_to_next), a
    pop     bc      ; height
    ld      a, c
    ld      (shape_height), a
    pop     bc      ; width
    ld      a, c
    ld      (shape_width), a
    ld      (shape_wcount), a
    ex      (sp), hl ; shape data
    ld      (shape_data), hl
    ld      hl, (_video_ptr)    ; save video ptr
    ld      (video_ptr_old), hl
; loop
bs1:
    ; load shape character into L
    ld      hl, (shape_data)
    ld      a, (hl)             ; A = char to blit
    inc     hl                  ; shape_data++
    ld      (shape_data), hl
    or      a
    jp      z,bs2

    ; draw character
    ld      l, a
    call    _blit_char

bs2:
    ; next char
    ld      hl, (_video_ptr)    ; video_ptr++
    inc     hl
    ld      (_video_ptr), hl
    ld      a, (shape_wcount)   ; --shape_wcount
    dec     a
    ld      (shape_wcount), a
    jp      nz,bs1              ; --> loop

    ; next row
    ld      a, (shape_width)    ; reset width counter
    ld      (shape_wcount), a
    ld      a, (shape_skip_to_next) ; shape_data += shape_skip_to_next
    or      a
    jp      z,bs3
    ld      e, a
    ld      d, 0
    ld      hl, (shape_data)
    add     hl, de
    ld      (shape_data), hl
bs3:
    ld      d, 0x01             ; video_ptr = video_ptr_old + 6 * 64
    ld      e, 0x80
    ld      hl, (video_ptr_old)
    add     hl, de
    ld      (_video_ptr), hl
    ld      (video_ptr_old), hl ; save video_ptr

    ld      a, (shape_height)   ; --height
    dec     a
    ld      (shape_height), a
    jp      nz,bs1              ; --> loop
    ret


;----------------------------------------------------------------------------

_clear_shape:
    pop     hl      ; return address
    pop     bc      ; skip_to_next
    ld      a, c
    ld      (shape_skip_to_next), a
    pop     bc      ; height
    ld      a, c
    ld      (shape_height), a
    pop     bc      ; width
    ld      a, c
    ld      (shape_width), a
    ld      (shape_wcount), a
    ex      (sp), hl ; shape data
    ld      (shape_data), hl
    ld      hl, (_video_ptr)    ; save video ptr
    ld      (video_ptr_old), hl
; loop
cs1:
    ; load shape character into L
    ld      hl, (shape_data)
    ld      a, (hl)             ; A = char to blit
    inc     hl                  ; shape_data++
    ld      (shape_data), hl
    or      a
    jp      z,cs2

    ; clear character
    ld      l, a
    call    _clear_char

cs2:
    ; next char
    ld      hl, (_video_ptr)    ; video_ptr++
    inc     hl
    ld      (_video_ptr), hl
    ld      a, (shape_wcount)   ; --shape_wcount
    dec     a
    ld      (shape_wcount), a
    jp      nz,cs1              ; --> loop

    ; next row
    ld      a, (shape_width)    ; reset width counter
    ld      (shape_wcount), a
    ld      a, (shape_skip_to_next) ; shape_data += shape_skip_to_next
    or      a
    jp      z,cs3
    ld      e, a
    ld      d, 0
    ld      hl, (shape_data)
    add     hl, de
    ld      (shape_data), hl
cs3:
    ld      d, 0x01             ; video_ptr = video_ptr_old + 6 * 64
    ld      e, 0x80
    ld      hl, (video_ptr_old)
    add     hl, de
    ld      (_video_ptr), hl
    ld      (video_ptr_old), hl ; save video_ptr

    ld      a, (shape_height)   ; --height
    dec     a
    ld      (shape_height), a
    jp      nz,cs1              ; --> loop
    ret


;----------------------------------------------------------------------------

; extern void __CALLEE__  clear_rect_tb(char w, char h);
; uses: buf_ptr, row_ptr, buf_x
_clear_rect_tb:
    pop     hl                  ; return address
    pop     bc                  ; h = height
    ld      a, c
    ld      (shape_height), a
    ex      (sp), hl            ; w = width
    ld      a, l
    ld      (shape_width), a

    ; BC = x2flag + buf_x
    ld      hl, (buf_x)
    ld      h, 0
    ld      bc, x2flag
    add     hl, bc
    ld      b, h
    ld      c, l
; row loop
csb1:
    ; BC must be set to x2flag + buf_x
    ; buf_ptr points to destination
    push    bc                  ; save BC for next row
    ld      a, (shape_width)    ; reset width counter
    ld      (shape_wcount), a   ; shape_wcount = shape_width
    ld      hl, (buf_ptr)
    ex      de, hl              ; DE = &buf_ptr (dst pointer)
; char loop
csb2:
    ; clear char in buffer
    ld      a, 32               ; space char
    ld      (de), a
    ; update row dirty flag
    ; *row_ptr |= x2flag[buf_x];
    ld      a, (bc)             ; a = x2flag[buf_x]
    ld      hl, (row_ptr)       ; *row_ptr |= a
    or      a, (hl)
    ld      (hl), a

    ; next char
    inc     de                  ; dst pointer += 2
    inc     de
    inc     bc                  ; address of next x2flag
    ld      hl, shape_wcount    ; --shape_wcount
    dec     (hl)
    jp      nz, csb2            ; --> loop

    ; next row
    pop     bc                  ; restore BC: x2flag + buf_x
    ld      hl, shape_height    ; if (--height == 0) return;
    dec     (hl)
    ret     z                   ; done

    ; prepare for next row
    ld      de, 80              ; buf_ptr += 2*40
    ld      hl, (buf_ptr)
    add     hl, de
    ld      (buf_ptr), hl
    ld      hl, row_ptr        ; row_ptr++;
    inc     (hl)
    jp      csb1                ; continue with next row

;----------------------------------------------------------------------------

; extern void __CALLEE__  print_shape_tb(char *shape_data, char w, char h, char stn);
; uses: buf_ptr, row_ptr, buf_x
_print_shape_tb:
    pop     hl                  ; return address
    pop     bc                  ; stn = skip to next
    ld      a, c
    ld      (shape_skip_to_next), a
    pop     bc                  ; h = height
    ld      a, c
    ld      (shape_height), a
    pop     bc                  ; w = width
    ld      a, c
    ld      (shape_width), a
    ex      (sp), hl            ; shape data
    ld      (shape_data), hl
    ; BC = x2flag + buf_x
    ld      hl, (buf_x)
    ld      h, 0
    ld      bc, x2flag
    add     hl, bc
    ld      b, h
    ld      c, l
; row loop
ds1:
    ; BC must be set to x2flag + buf_x
    ; buf_ptr points to destination
    push    bc                  ; save BC for next row
    ld      a, (shape_width)    ; reset width counter
    ld      (shape_wcount), a   ; shape_wcount = shape_width
    ld      hl, (buf_ptr)
    ex      de, hl              ; DE = &buf_ptr (dst pointer)
    ld      hl, (shape_data)    ; HL = &_shape_data (src pointer)
; char loop
ds2:
    ; load shape character
    ld      a, (hl)             ; A = *shape_data++
    inc     hl                  ; shape_data++
    push    hl                  ; save src pointer
    ; make space ' ' char transparent
    cp      32
    jp      z, ds4
    ; store char into buffer
    ld      (de), a
    ; update row dirty flag
    ; *row_ptr |= x2flag[buf_x];
    ld      a, (bc)             ; a = x2flag[buf_x]
    ld      hl, (row_ptr)      ; *row_ptr |= a
    or      a, (hl)
    ld      (hl), a

    ; next char
ds4:
    inc     de                  ; dst pointer += 2
    inc     de
    inc     bc                  ; address of next x2flag
    ld      hl, shape_wcount    ; --shape_wcount
    dec     (hl)
    pop     hl                  ; restore src pointer (shape_data)
    jp      nz, ds2             ; --> loop

    ; next row
    ex      de, hl              ; save updated src pointer (shape_data)
    pop     bc                  ; restore BC: x2flag + buf_x
    ld      hl, shape_height    ; if (--height == 0) return;
    dec     (hl)
    ret     z                   ; done

    ; prepare for next row
    ; shape_data += shape_skip_to_next
    ld      hl, (shape_skip_to_next) ; shape_data += shape_skip_to_next
    ld      h, 0
    add     hl, de
    ld      (shape_data), hl
    ; buf_ptr += 2*40
    ld      de, 80
    ld      hl, (buf_ptr)
    add     hl, de
    ld      (buf_ptr), hl
    ld      hl, row_ptr        ; row_ptr++;
    inc     (hl)
    jp      ds1                 ; continue with next row

;----------------------------------------------------------------------------
