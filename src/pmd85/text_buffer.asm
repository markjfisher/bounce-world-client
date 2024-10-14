;; from screen_util
EXTERN _video_ptr
EXTERN _blit_char
EXTERN _clear_char

PUBLIC _gotox_tb
PUBLIC _gotoy_tb
PUBLIC _gotoxy_tb
PUBLIC _update_tb_ptr

PUBLIC _putc_tb
PUBLIC _putc_upper_tb
PUBLIC _puts_tb

PUBLIC _show_text_buffer

;; for debugging
PUBLIC _get_tb_buffer
PUBLIC _get_tb_row_flags

;; used in shape
PUBLIC buf_x, buf_y
PUBLIC buf_ptr, row_ptr

;----------------------------------------------------------------------------

;; buffer dimensions
defc    BUFFER_COLS = 40
defc    BUFFER_ROWS = 24

;; bytes per buffer row
defc    BUFFER_BPR  = (2 * BUFFER_COLS)

;; character height in pixels
;; 6 for 6x6 characters
;; 8 for 6x8 usual pmd characters
defc    BUFFER_CHAR_HEIGHT = 6

;; buffer position on screen (X0: bytes/chars from left; Y0: display lines from top)
defc    BUFFER_SCREEN_X0 = 4
defc    BUFFER_SCREEN_Y0 = (6 * 8 - 2)

;; video ram
defc    DISPLAY     = 0xC000

;; video ram bytes per scan line
;; 48 bytes is visible, 16 bytes not visible (notepad area)
defc    DISPLAY_BPL = 64

;; video ram bytes per character row
defc    DISPLAY_BPR = (BUFFER_CHAR_HEIGHT * DISPLAY_BPL)

;; start of buffer in video ram
defc    DISPX0Y0    = (DISPLAY + BUFFER_SCREEN_Y0 * DISPLAY_BPL + BUFFER_SCREEN_X0)

;----------------------------------------------------------------------------

SECTION data_user

;----------------------------------------------------------------------------

;; text buffer current x, y position
buf_x:  db 0
buf_y:  db 0

ASSERT buf_x + 1 = buf_y

;; pointer into text buffer data
buf_ptr:
        dw buffer

;; pointer to dirty flags for current row
row_ptr:
        dw row_flags

;; 24 pre-calculated row addresses in text buffer
buf_row_addr:
    dw      buffer, buffer + BUFFER_BPR, buffer + 2 * BUFFER_BPR, buffer + 3 * BUFFER_BPR
    dw      buffer +  4 * BUFFER_BPR, buffer +  5 * BUFFER_BPR, buffer +  6 * BUFFER_BPR, buffer +  7 * BUFFER_BPR
    dw      buffer +  8 * BUFFER_BPR, buffer +  9 * BUFFER_BPR, buffer + 10 * BUFFER_BPR, buffer + 11 * BUFFER_BPR
    dw      buffer + 12 * BUFFER_BPR, buffer + 13 * BUFFER_BPR, buffer + 14 * BUFFER_BPR, buffer + 15 * BUFFER_BPR
    dw      buffer + 16 * BUFFER_BPR, buffer + 17 * BUFFER_BPR, buffer + 18 * BUFFER_BPR, buffer + 19 * BUFFER_BPR
    dw      buffer + 20 * BUFFER_BPR, buffer + 21 * BUFFER_BPR, buffer + 22 * BUFFER_BPR, buffer + 23 * BUFFER_BPR

;; 24 pre-calculated row addresses in video ram
video_row_addr:
    dw      DISPX0Y0, DISPX0Y0 + DISPLAY_BPR, DISPX0Y0 + 2 * DISPLAY_BPR, DISPX0Y0 + 3 * DISPLAY_BPR
    dw      DISPX0Y0 +  4 * DISPLAY_BPR, DISPX0Y0 +  5 * DISPLAY_BPR, DISPX0Y0 +  6 * DISPLAY_BPR, DISPX0Y0 +  7 * DISPLAY_BPR
    dw      DISPX0Y0 +  8 * DISPLAY_BPR, DISPX0Y0 +  9 * DISPLAY_BPR, DISPX0Y0 + 10 * DISPLAY_BPR, DISPX0Y0 + 11 * DISPLAY_BPR
    dw      DISPX0Y0 + 12 * DISPLAY_BPR, DISPX0Y0 + 13 * DISPLAY_BPR, DISPX0Y0 + 14 * DISPLAY_BPR, DISPX0Y0 + 15 * DISPLAY_BPR
    dw      DISPX0Y0 + 16 * DISPLAY_BPR, DISPX0Y0 + 17 * DISPLAY_BPR, DISPX0Y0 + 18 * DISPLAY_BPR, DISPX0Y0 + 19 * DISPLAY_BPR
    dw      DISPX0Y0 + 20 * DISPLAY_BPR, DISPX0Y0 + 21 * DISPLAY_BPR, DISPX0Y0 + 22 * DISPLAY_BPR, DISPX0Y0 + 23 * DISPLAY_BPR

;----------------------------------------------------------------------------

SECTION bss_user

;; dirty row flags: 1 = dirty row, row needs to be painted on screen, 0 = no change on this row
row_flags:
    ds      BUFFER_ROWS

;; buffer: space for text buffer cells
;; - each cell keeps current and previous character value
;; - only cells with changes are painted into video ram by show_text_buffer()
buffer:
    ds      (BUFFER_ROWS * BUFFER_BPR)

;----------------------------------------------------------------------------

SECTION code_user

;----------------------------------------------------------------------------

;; some debugging stuff

; extern char __FASTCALL__ *get_tb_buffer(void);
_get_tb_buffer:
    ld      hl, buffer
    ret

; extern char __FASTCALL__ *get_tb_row_flags(void);
_get_tb_row_flags:
    ld      hl, row_flags
    ret

;----------------------------------------------------------------------------

; extern void __FASTCALL__ gotox_tb(unsigned char x);
_gotox_tb:
    ld      a, l
    ld      (buf_x), a
    jr      _update_tb_ptr

;----------------------------------------------------------------------------

; extern void __FASTCALL__ gotoy_tb(unsigned char y);
_gotoy_tb:
    ld      a, l
    ld      (buf_y), a
    jr      _update_tb_ptr

;----------------------------------------------------------------------------

; extern void __CALLEE__  gotoxy_tb(unsigned char x, unsigned char y);
_gotoxy_tb:
    pop     hl                  ; return address
    pop     bc                  ; y
    ld      a, c
    ld      (buf_y), a
    ex      (sp), hl            ; x
    ld      a, l
    ld      (buf_x), a
    ; continue to _update_tb_ptr

;----------------------------------------------------------------------------

; extern void __FASTCALL__ update_tb_ptr(void);
; input: buf_x, buf_y
; output: row_ptr, buf_ptr
_update_tb_ptr:
    ; row_ptr = &row_flags[buf_y]
    ld      a, (buf_y)
    ld      e, a
    ld      d, 0
    ld      hl, row_flags
    add	    hl, de
    ld	    (row_ptr), hl
;a) calculate ptr value, shorter, bit slower
    ; buf_ptr = &row_flags[2 * (buf_y * 40 + buf_x)]
    ; ld      h, d                ; get buf_y from DE, preserve DE for later
    ; ld      l, e
    ; ; HL = 5 * buf_y
    ; add     hl, hl
    ; add     hl, hl
    ; add     hl, de              ; +buf_y
    ; ; DE = 8 * HL  (40 * buf_y)
    ; add     hl, hl
    ; add     hl, hl
    ; add     hl, hl
    ; ; + buf_x
    ; ex      de, hl
    ; ld      hl, (buf_x)
    ; ld      h, 0
    ; add     hl, de
    ; ; * 2
    ; add     hl, hl
    ; ; + buffer
    ; ld      de, buffer
    ; add     hl, de
    ; ld      (buf_ptr), hl
    ; ret
;b) use pre-calculated row address, bit faster, lot bigger (table with pre-calculated values)
    ; HL = buf_y * 2
    ex      de, hl
    add     hl, hl
    ; HL += buf_row_addr
    ld      de, buf_row_addr
    add     hl, de
    ; LD DE, (HL)
    ld      e, (hl)
    inc     hl
    ld      d, (hl)
    ; += 2 * buf_x
    ld      hl, (buf_x)
    ld      h, 0
    add     hl, hl
    add     hl, de
    ld      (buf_ptr), hl
    ret

;----------------------------------------------------------------------------

; extern void __FASTCALL__ update_tb_ptr(void);
_update_video_ptr:
    ld	    hl, (buf_y)
    ld      h, 0
    add     hl, hl
    ; HL += video_row_addr
    ld      de, video_row_addr
    add     hl, de
    ; LD DE, (HL)
    ld      e, (hl)
    inc     hl
    ld      d, (hl)
    ; += buf_x
    ld      hl, (buf_x)
    ld      h, 0
    add     hl, de
    ld      (_video_ptr), hl
    ret

;----------------------------------------------------------------------------
; extern void __FASTCALL__ putc_upper_tb(char c);
_putc_upper_tb:
    ld      a, l
    cp      97      ; 'a'
    jr      c, _putc_tb
    cp      123     ; 'z'
    jr      nc, _putc_tb
    sub     32
    ld      l, a
    ; continue to _putc_tb
;----------------------------------------------------------------------------
; extern void __FASTCALL__ putc_tb(char c);
_putc_tb:
    ld      c, l            ; C = character
    ld      b, 0            ; flag to indicate we need to call update_tb_ptr
    ld      hl, buf_x
    ld      a, (hl)
    cp      BUFFER_COLS
    jr      c, pc1
    ld      (hl), b         ; buf_x = 0
    inc     b
    inc     hl              ; HL = &buf_y
    inc     (hl)            ; buf_y++
pc1:
    ld      hl, buf_y
    ld      a, (hl)
    cp      BUFFER_ROWS
    jr      c, pc2
    xor     a
    ld      (hl), a         ; buf_y = 0
    inc     b
pc2:
    ld      a, b
    or      a
    ; update buf_ptr and row_ptr
    call    nz, _update_tb_ptr 
    ; store char into buffer
    ld      hl, (buf_ptr)
    ld      (hl), c
    ; buf_ptr += 2
    inc     hl
    inc     hl
    ld      (buf_ptr), hl
    ; update row dirty flag
    ld      hl, (row_ptr)       ; update row dirty flag
    ld      (hl), h             ; any non-zero value
    ; buf_x++
    ld      hl, buf_x
    inc     (hl)
    ret

;----------------------------------------------------------------------------
; extern void __FASTCALL__ puts_tb(const char *s);
_puts_tb:
ps1:
    ld      a, (hl)
    or      a
    ret     z
    push    hl
    ld      l, a
    call    _putc_tb
    pop     hl
    inc     hl
    jp      ps1

;----------------------------------------------------------------------------
; extern void __FASTCALL__ show_text_buffer(void);
_show_text_buffer:
    ; buf_y = BUFFER_ROWS
    ld      a, BUFFER_ROWS
    ld      (buf_y), a
    ; reset video_ptr (video ram)
    ld      hl, DISPX0Y0
    ld      (_video_ptr), hl
    ; reset buf_ptr (text source)
    ld      hl, buffer
    ld      (buf_ptr), hl
    ex      de, hl
    ; reset row_ptr (row dirty flag)
    ld      hl, row_flags
    ld      (row_ptr), hl
    ld      b, h
    ld      c, l

; row loop
stb1:
; BC = row_ptr; DE = buf_ptr
    ; check row dirty flag
    ld      a, (bc)             ; A = dirty flags
    or      a
    jp      nz, stb2             ; -> dirty row
; row is clean, prepare for next row
    ; buf_ptr: DE += 2 * 40
    ld      hl, BUFFER_BPR
    add     hl, de
    ex      de, hl
    ; video_ptr += 6 * 64
    ; push de
    ; ld      hl, (_video_ptr)
    ; ld      de, DISPLAY_BPR
    ; add     hl, de
    ; ld      (_video_ptr), hl
    ; pop de
    ; bit faster (58T vs 74T)
    ld      hl, _video_ptr
    ld      a, DISPLAY_BPR % 256
    add     (hl)
    ld      (hl), a
    inc     hl
    ld      a, DISPLAY_BPR / 256
    adc     (hl)
    ld      (hl), a
stb2b:
; BC = row_ptr; DE = buf_ptr
    ; row_ptr++
    inc     bc
    ; rows--
    ld      hl, buf_y
    dec     (hl)
    jp      nz, stb1            ; -> next row
    ret                         ; done

; process dirty row
stb2:
    push    bc                  ; save BC with row_ptr
    ; cols: B = BUFFER_COLS
    ld      b, BUFFER_COLS
    ; HL = buf_ptr; DE = video_ptr
    ld      hl, (_video_ptr)
    ex      de, hl

; character loop
stb3:
; HL = buf_ptr; DE = video_ptr; B = cols
    ld      a, (hl)
    inc     hl
    cp      (hl)
    jp      z, stb3b            ; -> no change, skip this char

; process dirty character
    ld      (hl), a             ; update old char
    push    bc                  ; save B with cols
    push    hl                  ; save buf_ptr
    ex      de, hl              ; set updated video_ptr
    ld      (_video_ptr), hl
    ld      l, a                ; L = char to blit
    cp      a, 32               ; space char?
    call    z, _clear_char
    call    nz, _blit_char
; restore HL, DE, B
    ld      hl, (_video_ptr)    ; restore DE with _video_ptr
    ex      de, hl
    pop     hl                  ; restore HL with buf_ptr
    pop     bc                  ; restore B with cols

; prepare for next char
stb3b:
; HL = buf_ptr; DE = video_ptr
    inc     hl
    ; video_ptr
    inc     de
    ; cols--
    dec     b
    jp      nz, stb3            ; -> next char

; dirty row completed
    ex      de, hl              ; buf_ptr to DE; video_ptr to HL
    ; update video_ptr: next character row - line we completed right now
    ; video_ptr += 6 * 64 - 40
    ld      bc, DISPLAY_BPR - BUFFER_COLS
    add     hl, bc
    ld      (_video_ptr), hl
    ; restore BC with row_ptr
    pop     bc
    ; reset row dirty flag
    xor     a
    ld      (bc), a
    jp      stb2b

; ;----------------------------------------------------------------------------
