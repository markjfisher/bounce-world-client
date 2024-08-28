        .export         _cleanup_client
        .import         _pause
        .include        "atari.inc"

.proc _cleanup_client
        ; restore click
        lda     #$00
        sta     NOCLIK
        ; turn off DLI
        lda     #$40
        sta     NMIEN

        ; RESET THE SCREEN by closing and opening E: on IOCB#0
        ldx     #$00
        lda     #$0C
        sta     ICCOM, x
        jsr     CIOV

        ldx     #$00            ; channel 0
        lda     #$03            ; open
        sta     ICCOM, x
        lda     #<dev_name
        sta     ICBAL, x
        lda     #>dev_name
        sta     ICBAH, x
        lda     #$0C
        sta     ICAX1, x
        jsr     CIOV

        lda     #$ff
        jmp     _pause
        ; implicit rts
.endproc

.data
dev_name:        .byte "E:", 0