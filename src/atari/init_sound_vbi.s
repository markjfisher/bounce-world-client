        .export    _init_sound_vbi
        .export    curr_round

        .import    _debug
        .import    _is_playing_collision
        .import    _current_volume_index
        .import    _sc0
        .import    _sc1
        .import    _sc2


        .include   "atari.inc"

.proc _init_sound_vbi
        ; jsr     _debug
        ldy     #<_continue_sounds
        ldx     #>_continue_sounds
        ; immediate VBI, gives us 2000 cycles to play with.
        lda     #$06
        jmp     SETVBV
.endproc

.proc _continue_sounds
        ; are we playing a collision sound?
        lda     _is_playing_collision
        beq     done

        ; check if the count has moved on enough to change to next sound
        inc     curr_round
        lda     curr_round

        lsr     a                       ; we are executing every 2 VBIs, so divide count by 2 to get current index
        cmp     _current_volume_index
        beq     done

        sta     _current_volume_index
        cmp     #23                     ; volume data range is 0-22
        bcc     :+

        ; the final sound volume is 0 on all channels, so no need to call stop_sound
        ; jsr     _debug
        lda     #$00
        sta     _is_playing_collision
        sta     curr_round
        beq     done

:
        lda     #$80                    ; pre move distortion into high nibble
        sta     sound_data+2            ; distortion is constant

        ; sound 0, 80, 8, sc0[idx]
        lda     #$00
        sta     sound_data              ; voice 0
        lda     #80
        sta     sound_data+1            ; frequency (aka pitch)
        ldy     _current_volume_index
        lda     _sc0, y
        sta     sound_data+3            ; volume
        jsr     do_sound

        ; sound 1, 100, 8, sc1[idx]
        lda     #$02
        sta     sound_data              ; voice 1 (doubled)
        lda     #100
        sta     sound_data+1            ; frequency (aka pitch)
        ldy     _current_volume_index
        lda     _sc1, y
        sta     sound_data+3            ; volume
        jsr     do_sound

        ; sound 2, 130, 8, sc2[idx]
        lda     #$04
        sta     sound_data              ; voice 2 (doubled)
        lda     #130
        sta     sound_data+1            ; frequency (aka pitch)
        ldy     _current_volume_index
        lda     _sc2, y
        sta     sound_data+3            ; volume
        jsr     do_sound


done:
        jmp     SYSVBV

.endproc


; custom version of __sound which doesn't use tmp vars, or stack
.proc do_sound
        ldy     #$00
        lda     sound_data, y           ; voice needs to be times 2 for correct x offset, ie. voices are 0,2,4 instead of 0,1,2
        tax
        iny
        lda     sound_data, y           ; get freq (aka pitch)
        sta     AUDF1, x

        lda     #$00
        sta     AUDCTL
        lda     #$03
        sta     SKCTL
        iny
        lda     sound_data, y           ; this is pre-multiplied by 16 into high nibble, and is the distortion
        clc
        iny
        adc     sound_data, y           ; add the volume
        sta     AUDC1, x
        rts
.endproc

.bss
; space for the sound parameters
; voice, freq, distortion, volume
sound_data:     .res 4


.data
curr_round:     .byte 0
