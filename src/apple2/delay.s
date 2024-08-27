;
; Copied from cc65 source, original version by
; Oliver Schmidt, 2020-06-14
;
; This version removes the "APPLE2ENH" only enforcement.
;

        .export         _pause
        .export         _wait_vsync

        .import         ostype

        .include        "apple2.inc"

; void wait_vsync();
;
_wait_vsync:
        bit     ostype
        bmi     iigs            ; $8x
        bvs     iic             ; $4x

:       bit     RDVBLBAR
        bpl     :-              ; Blanking
:       bit     RDVBLBAR
        bmi     :-              ; Drawing
        rts

        ; Apple IIgs TechNote #40, VBL Signal
iigs:   bit     RDVBLBAR
        bmi     iigs            ; Blanking
:       bit     RDVBLBAR
        bpl     :-              ; Drawing
        rts

        ; Apple IIc TechNote #9, Detecting VBL
iic:    sei
        sta     IOUDISOFF
        lda     RDVBLMSK
        bit     ENVBL
        bit     PTRIG           ; Reset VBL interrupt flag
:       bit     RDVBLBAR
        bpl     :-
        asl
        bcs     :+              ; VBL interrupts were already enabled
        bit     DISVBL
:       sta     IOUDISON        ; IIc Tech Ref Man: The firmware normally leaves IOUDIS on.
        cli
        rts

; void pause(uint8_t count);
;
; does 'count' vsync waits

_pause:
        tax
:       jsr     _wait_vsync
        dex
        bne     :-
        rts
