        .export    _init_vbi
        .export    _restore_vbi

        .export    vbi_main

        .import    _debug
        .import    _txt_c3

        ;; DLI values
        ; track where we are in the screen for the colour changes
        .import    current_section

        ; set in C for the iteration of the flash pulse
        .import    _current_flash_time
        ; boolean to say we want to flash screen or not. when true, C is indicating we want to start
        .import    _is_flashing_screen

        ; boolean to say we want to play a collision sound
        .import    _is_playing_collision
        ; which index of the sound envelope we are at
        .import    _current_volume_index
        ; the volume shape data
        .import    _sc0
        .import    _sc1
        .import    _sc2

        .include   "atari.inc"

.proc _init_vbi
        ; save the old VBI
        lda     VVBLKI
        sta     old_vbi
        lda     VVBLKI+1
        sta     old_vbi+1

        ; set the new VBI
        ldy     #<vbi_main
        ldx     #>vbi_main
        lda     #$06
        jmp     SETVBV
.endproc

.proc _restore_vbi
        ldy     old_vbi
        ldx     old_vbi+1
        lda     #$06
        jmp     SETVBV
.endproc


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; VBI routines

; we are at the top of the screen, so set the colour to black, a DLI will change the colour on rest of screen
.proc vbi_main
        ;; uncomment if you want attract to be honoured
        ; lda     ATRACT
        ; cmp     #$7F
        ; bcs     skip_PF2

        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        ;; set PF2 to BLACK

        lda     #$00
        sta     COLPF2
        sta     current_section
        sta     ATRACT
        lda     _txt_c3
        sta     COLBK
        sta     COLOR4

skip_PF2:

        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        ;; DO BG SCREEN FLASH FOR COLLISION

        lda     _is_flashing_screen
        beq     done_flash

        ldy     _current_flash_time
        cpy     #16
        bcs     end_flash

        lda     flash_data, y
        sta     COLBK
        sta     COLOR4
        inc     _current_flash_time
        bne     done_flash

end_flash:
        lda     #$00
        sta     _is_flashing_screen
        sta     _current_flash_time
        lda     _txt_c3
        sta     COLBK
        sta     COLOR4

done_flash:


        ; END VBI
        jmp     SYSVBV
.endproc


; .proc vbi_continue_sounds
;         ; are we playing a collision sound?
;         lda     _is_playing_collision
;         beq     done

;         ; check if the count has moved on enough to change to next sound
;         inc     sound_round
;         lda     sound_round

;         lsr     a                       ; we are executing every 2 VBIs, so divide count by 2 to get current index
;         cmp     _current_volume_index
;         beq     done

;         sta     _current_volume_index
;         cmp     #23                     ; volume data range is 0-22
;         bcc     :+

;         ; the final sound volume is 0 on all channels, so no need to call stop_sound
;         ; jsr     _debug
;         lda     #$00
;         sta     _is_playing_collision
;         sta     sound_round
;         beq     done

; :
;         lda     #$80                    ; pre move distortion into high nibble
;         sta     sound_data+2            ; distortion is constant

;         ; sound 0, 80, 8, sc0[idx]
;         lda     #$00
;         sta     sound_data              ; voice 0
;         lda     #80
;         sta     sound_data+1            ; frequency (aka pitch)
;         ldy     _current_volume_index
;         lda     _sc0, y
;         sta     sound_data+3            ; volume
;         jsr     do_sound

;         ; sound 1, 100, 8, sc1[idx]
;         lda     #$02
;         sta     sound_data              ; voice 1 (doubled)
;         lda     #100
;         sta     sound_data+1            ; frequency (aka pitch)
;         ldy     _current_volume_index
;         lda     _sc1, y
;         sta     sound_data+3            ; volume
;         jsr     do_sound

;         ; sound 2, 130, 8, sc2[idx]
;         lda     #$04
;         sta     sound_data              ; voice 2 (doubled)
;         lda     #130
;         sta     sound_data+1            ; frequency (aka pitch)
;         ldy     _current_volume_index
;         lda     _sc2, y
;         sta     sound_data+3            ; volume
;         jsr     do_sound


; done:
;         jmp     SYSVBV

; .endproc


; ; custom version of __sound which doesn't use tmp vars, or stack
; .proc do_sound
;         ldy     #$00
;         lda     sound_data, y           ; voice needs to be times 2 for correct x offset, ie. voices are 0,2,4 instead of 0,1,2
;         tax
;         iny
;         lda     sound_data, y           ; get freq (aka pitch)
;         sta     AUDF1, x

; ; THIS PART BREAKS SIO
;         lda     #$00
;         sta     AUDCTL
;         lda     #$03
;         sta     SKCTL
; ;

;         iny
;         lda     sound_data, y           ; this is pre-multiplied by 16 into high nibble, and is the distortion
;         clc
;         iny
;         adc     sound_data, y           ; add the volume
;         sta     AUDC1, x
;         rts
; .endproc

; .bss
; ; space for the sound parameters
; ; voice, freq, distortion, volume
; sound_data:     .res 4


.data
old_vbi:        .word 0
; sound_round:     .byte 0
flash_data:     .byte 15, 13, 11, 9, 8, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0