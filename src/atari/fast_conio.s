        .export         _cincx
        .export         _cputc_fast
        .export         _gotoxy_fast

        .import         _mul40
        .import         _revflag
        .import         popa

        .import         _debug
        .import         _pause

        .importzp       ptr4

        .include        "atari.inc"

; void cputc_fast(c)
;
; minimal version of cputc that doesn't update the old cursor position or care about CR/LF chars
; for the object drawing only.

_cputc_fast:
        cmp     #' '
        beq     _cincx

        asl     a               ; shift out the inverse bit
        adc     #$c0            ; grab the inverse bit; convert ATASCII to screen code
        bpl     codeok          ; screen code ok?
        eor     #$40            ; needs correction
codeok: lsr     a               ; undo the shift
        bcc     cputdirect
        eor     #$80            ; restore the inverse bit

cputdirect:
        pha

        lda     ROWCRS
        jsr     _mul40          ; sets A/X to A*40
        adc     SAVMSC
        sta     ptr4
        txa
        adc     SAVMSC+1
        sta     ptr4+1
        pla
        ora     _revflag
        ldy     COLCRS
        sta     (ptr4), y

        inc     COLCRS

        rts


_cincx:
        inc     COLCRS
        rts

_gotoxy_fast:
        sta     ROWCRS          ; Set Y
        jsr     popa            ; Get X
        sta     COLCRS          ; Set X
        lda     #0
        sta     COLCRS+1
        rts