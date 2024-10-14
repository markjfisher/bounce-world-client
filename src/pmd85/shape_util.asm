
;; from screen_util
EXTERN _video_ptr, _blit_char, _clear_char

;; from text_buffer
EXTERN buf_x, buf_y
EXTERN buf_ptr, row_ptr
EXTERN _update_tb_ptr

IFDEF SHAPE_BLITTER
PUBLIC _blit_shape
PUBLIC _clear_shape
ENDIF

PUBLIC _clear_rect_tb:
PUBLIC _print_shape_tb

; from double_buffer
EXTERN _dirty_rectangles, _dirty_ptr
defc MAX_DIRTY_RECTS = 52

PUBLIC _add_dirty_rect

; from data.c
EXTERN _shapes

; from text_buffer.asm
; TODO move to text_buffer.inc
defc    BUFFER_COLS = 40
defc    BUFFER_ROWS = 24

PUBLIC _shape_to_buffer
PUBLIC _get_shape_data

;----------------------------------------------------------------------------

SECTION bss_user

;----------------------------------------------------------------------------

; pointer to shape characters
shape_data:
    dw  0

; shape position
; buf_x, buf_y is used for shape_position

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

ASSERT shape_width + 1 = shape_height

;----------------------------------------------------------------------------

SECTION code_user

; some debugging stuff

; extern void char __FASTCALL__ *_get_shape_data(void)
_get_shape_data:
    ld      hl, shape_data
    ret

;----------------------------------------------------------------------------

; the code does not paint shapes directly into video RAM anymore
; to avoid flickering the text_buffer is used

IFDEF SHAPE_BLITTER

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

ENDIF ; SHAPE_BLITTER

;----------------------------------------------------------------------------

; extern void __CALLEE__  clear_rect_tb(char w, char h);
; uses: buf_ptr, row_ptr
_clear_rect_tb:
    pop     hl                  ; return address
    pop     bc                  ; C = height
    ex      (sp), hl            ; L = width
    ld      a, l
    ld      (shape_width), a    ; store width
    ld      b, a                ; B = width
    ld      hl, (buf_ptr)
    ex      de, hl              ; DE = &buf_ptr (dst pointer)

; row loop
csb1:
; B = width; C = height; DE = buf_ptr
    ld      a, 32               ; space char

; char loop
csb2:
; A = 32; B = width; C = height; DE = buf_ptr
    ; clear char in buffer
    ld      (de), a
    ; next char
    inc     de                  ; dst pointer += 2
    inc     de
    dec     b                   ; --width
    jp      nz, csb2            ; --> loop

    ; prepare for next row
    ld      hl, (row_ptr)       ; update row dirty flag
    ld      (hl), h             ; any non-zero value
    inc     hl                  ; row_ptr++;
    ld      (row_ptr), hl
    dec     c                   ; --height
    ret     z                   ; done

    ; buf_ptr: DE += 2 * (40 - shape_width)
    ld      a, (shape_width)
    ld      b, a                ; reload B with width
    ld      a, 40
    sub     a, b
    add     a
    add     e
    ld      e, a
    jp      nc, csb1            ; continue with next row
    inc     d
    jp      csb1                ; continue with next row

;----------------------------------------------------------------------------

; extern void __CALLEE__  print_shape_tb(char *shape_data, char w, char h, char stn);
; is using: buf_ptr, row_ptr, shape_width, shape_skip_to_next
_print_shape_tb:
    pop     hl                  ; return address
    pop     bc                  ; stn = skip to next
    ld      a, c
    ld      (shape_skip_to_next), a
    pop     bc                  ; C = height
    pop     de                  ; width
    ld      b, e                ; B = width
    ex      (sp), hl            ; HL = shape data
print_shape_tb_asm:
; input:
;   HL = shape_data
;   B = w
;   C = h
;   shape_skip_to_next = stn
; is using: buf_ptr, row_ptr, shape_width, shape_skip_to_next
    ld      a, b                ; store width
    ld      (shape_width), a
print_shape_tb_asm2:
    ex      de, hl              ; load DE with buf_ptr
    ld      hl, (buf_ptr)
    ex      de, hl              ; DE = buf_ptr

; row loop
ds1:
; B = width; C = height; DE = buf_ptr; HL = shape_data

; char loop
ds2:
; B = width; DE = buf_ptr, HL = shape_data
    ; load shape character
    ld      a, (hl)             ; A = *shape_data
    ; make space char transparent
    cp      32
    jp      z, ds3
    ; store char into buffer
    ld      (de), a
ds3:
    ; next char
    inc     hl                  ; shape_data++
    inc     de                  ; dst pointer += 2
    inc     de
    dec     b                   ; --width
    jp      nz, ds2            ; --> loop

    ; prepare for next row
    push    hl                  ; save HL with updated shape_data ptr
    ld      hl, (row_ptr)       ; update row dirty flag
    ld      (hl), h             ; any non-zero value
    inc     hl                  ; row_ptr++;
    ld      (row_ptr), hl
    pop     hl                  ; restore HL with updated shape_data ptr
    dec     c                   ; --height
    ret     z                   ; done

    ; buf_ptr: DE += 2 * (40 - shape_width)
    ld      a, (shape_width)
    ld      b, a                ; reload B with width
    ld      a, 40               ; 40
    sub     a, b                ; -width
    add     a                   ; *2
    add     e                   ; +DE
    ld      e, a
    jp      nc, ds4
    inc     d
ds4:
    ; shape_data: HL += shape_skip_to_next
    ld      a, (shape_skip_to_next)
    add     l
    ld      l, a
    jp      nc, ds1             ; -> continue with next row
    inc     h
    jp      ds1                 ; -> continue with next row

;----------------------------------------------------------------------------

; extern void __CALLEE__ add_dirty_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
_add_dirty_rect:
    pop     hl              ; return address
    pop     de              ; h
    pop     bc              ; w
    ld      d, c            ; D = width, E = height
    pop     bc              ; C = y
    ex      (sp), hl        ; x
    ld      b, l            ; B = x
; BC = x, y
; DE = width, height
add_dirty_rect_asm:
    ; if (dirty_rectangles[0] >= MAX_DIRTY_RECTS) return; // buffer is full
    ld      hl, _dirty_rectangles
    ld      a, (hl)
    cp      MAX_DIRTY_RECTS
    ret     nc

    ; dirty_rectangles[0]++;
    inc     (hl)
    ; store rectangle: x, y, w, h
    ld      hl, (_dirty_ptr)
    ld      (hl), b         ; x
    inc     hl
    ld      (hl), c         ; y
    inc     hl
    ld      (hl), d         ; w
    inc     hl
    ld      (hl), e         ; h
    inc     hl
    ; update dirty_ptr
    ld      (_dirty_ptr), hl
    ret

;----------------------------------------------------------------------------

; extern void __CALLEE__ shape_to_buffer(uint8_t shape_id, int8_t center_x, int8_t center_y);
_shape_to_buffer:
    pop     hl              ; return address
    pop     de              ; E = center_y
    ld      a, e
    ld      (buf_y), a
    pop     bc              ; C = center_x
    ex      (sp), hl        ; L = shape_id
    ld      h, 0
    ; HL *= sizeof ShapeRecord (5)
    ld      d, h
    ld      e, l
    add     hl, hl
    add     hl, hl
    add     hl, de
    ; HL += shapes + offset to ShapeRecord::shape_width (1)
    ld      de, _shapes + 1
    add     hl, de
    ld      a, (hl)
    ld      (shape_width), a
    ld      (shape_height), a
    ld      (shape_wcount), a
    ; advance HL to point ShapeRecord::shape_data
    inc     hl
    inc     hl
    ld      e, (hl)
    inc     hl
    ld      d, (hl)
    ex      de, hl
    ld      (shape_data), hl
    ; B = round(width / 2)
    or      a
    rra
    adc     0
    ld      b, a
    ; x = center_x - round(w / 2)
    ld      a, c
    sub     a, b
    ld      (buf_x), a
    ; y = center_y - round(w / 2)
    ld      a, (buf_y)
    sub     b
    ld      (buf_y), a
    ld      b, a                ; A = B = y
    ld      hl, shape_height
; top | bottom clip
    jp      p, tb2              ; y >= 0
; y < 0
    ; height += y
    add     (hl)
    ld      (hl), a
    ; shape_data += width * -y
    xor     a
    ld      (buf_y), a        ; (y = 0)
    ld      d, a
    ld      a, (shape_width)
    ld      e, a
    ld      hl, (shape_data)
tb1:
    ; B: y < 0
    add     hl, de
    inc     b
    jp      nz, tb1
    ld      (shape_data), hl
    jp      tb3

; y >= 0
tb2:
    ; if (y+h > SCREEN_HEIGHT) {h = SCREEN_HEIGHT - y;}
    add     (hl)
    cp      BUFFER_ROWS + 1
    jp      c, tb3
    ld      a, BUFFER_ROWS
    sub     b
    ld      (hl), a

tb3:
    ; if (h <= 0) return
    ld      a, (shape_height)
    or      a
    ret     m
    ret     z

; left | right clip
    ld      a, (buf_x)
    ld      b, a                ; A = B = x
    or      a
    ld      hl, shape_width
    jp      p, tb4
; x < 0
    ; width += x
    add     (hl)
    ld      (hl), a
    ; shape_data += -x
    xor     a
    ld      (buf_x), a        ; (x = 0)
    ld      d, a
    sub     b
    ld      e, a
    ld      hl, (shape_data)
    add     hl, de
    ld      (shape_data), hl
    jp      tb5

; x >= 0
tb4:
    ; if (x+w > SCREEN_WIDTH) {w = SCREEN_WIDTH - x;}
    add     (hl)
    cp      BUFFER_COLS + 1
    jp      c, tb5
    ld      a, BUFFER_COLS
    sub     b
    ld      (hl), a

tb5:
    ; if (w <= 0) return
    ld      a, (shape_width)
    or      a
    ret     m
    ret     z

    ; shape_skip_to_next row = original width - clipped width
    ld      a, (shape_wcount)
    ld      hl, shape_width
    sub     (hl)
    ld      (shape_skip_to_next), a

; print clipped shape into text buffer
	; gotoxy_tb(x, y);
    call    _update_tb_ptr      ; input: buf_x, buf_y
	; print_shape_tb(data, w, h, s);
; ASSERT shape_width + 1 = shape_height
    ld      hl, (shape_width)
    ld      b, l                ; B = width
    ld      c, h                ; C = height
    ld      hl, (shape_data)    ; HL = shape_data
    call    print_shape_tb_asm
	; add_dirty_rect(x, y, w, h);
; ASSERT buf_x + 1 = buf_y
    ld      hl, (buf_x)
    ld      b, l                ; x
    ld      c, h                ; y
    ld      hl, (shape_width)
    ld      d, l                ; w
    ld      e, h                ; h
    jp      add_dirty_rect_asm

;----------------------------------------------------------------------------
