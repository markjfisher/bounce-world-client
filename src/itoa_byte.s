        .export  _itoa_byte

        .import  div10
        .import  popax

        .include "zeropage.inc"

; itoa_byte(char *s, uint8_t i)
;
; VERY simple itoa routine for value 0-255.
; Allocate 4 bytes at least to the string. Will null terminate

.proc _itoa_byte
        sta     tmp3                    ; hold onto A

        jsr     popax                   ; store the pointer to the string in ptr1
        sta     ptr1
        stx     ptr1+1

        lda     tmp3                    ; value we want to divide
        ldx     #'0'
        jsr     div10                   ; A = quotient, X = remainder

        ; is A over 10? If it is, we have number > 100
        cmp     #$3A                    ; $30 + 10
        bcs     over_100

        ; check if we're under 10
        cmp     #'0'
        beq     under_10

        ; just 2 digits
        ldy     #$00
        sta     (ptr1), y               ; the 10s value
        iny
        txa                             ; the remainder into A
        sta     (ptr1), y
        iny
        lda     #$00                    ; nul byte
        sta     (ptr1), y
        rts

        ; need to separate A into 2 chars. First char can only be 1 or 2 (100-199, 200-255)
        ; $3A == 10x
        ; $44 == 20x
over_100:
        pha                             ; save the value while we push a 1 or 2
        ldy     #$00
        cmp     #$44                    ; is it 20+?
        bcs     over_200

        ; store 1 in the first digit
        lda     #$31
        sta     (ptr1), y
        pla
        sec
        sbc     #$0a                    ; remove 10 from the number, so 3A becomes $30 which is the 10s value
        bcc     :+                      ; always
over_200:
        ; store 2 in the first digit
        lda     #$32
        sta     (ptr1), y
        pla                             ; restore the value for 2nd digit
        sec
        sbc     #$14                    ; remove 20 from the number, so $44 to $30 to get 2nd digit

        ; #########################################
        ; now print the 2nd digit of 3
:       iny
        sta     (ptr1), y

        ; finally the remainder digit
        txa
        iny
        sta     (ptr1), y

        iny
        bne     nul_terminate

under_10:
        ldy     #$00
        txa
        sta     (ptr1), y
        iny

nul_terminate:
        lda     #$00
        sta     (ptr1), y

        rts

.endproc
