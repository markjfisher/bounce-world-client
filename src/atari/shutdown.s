        .export         _cleanup_client
        .export         dev_name

        .import         _pause
        .import         _restore_vbi
        .include        "atari.inc"

.proc _cleanup_client
        ; restore click
        lda     #$00
        sta     NOCLIK
        ; turn off DLI
        lda     #$40
        sta     NMIEN

        ; restore the VBI
        jsr     _restore_vbi

        ; the reset of the screen code is not working, so just returning for now until I have more time
        rts


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
        ; this is hanging when it enters CIOV
        jsr     CIOV

        ; i was hoping this would cause screen to hold on the reset screen for a few seconds
        lda     #$60
        jmp     _pause
        ; implicit rts
.endproc

.data
dev_name:        .byte "E:", $9b
