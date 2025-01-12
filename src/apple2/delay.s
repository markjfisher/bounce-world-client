;
; Copied from cc65 source, original version by
; Oliver Schmidt, 2020-06-14
;
; This version removes the "APPLE2ENH" only enforcement.
;

        .export         _pause
        .export         _wait_vsync

        .import         WAIT

; removing this as it's not supported on ][+ and isn't really needed
; void wait_vsync();
;
_wait_vsync:
        rts

; void pause(uint8_t count);
;
; does 'count' x 1/60th of a second pauses


;; https://gswv.apple2.org.za/a2zine/GS.WorldView/Resources/USEFUL.TABLES/WAIT.DELAY.CR.txt
_pause:
        tax
:
        lda     #$4A            ; Sleep about 16ms, or 1/60th of a second (4A is 14.7ms, but this accounts for any additional looping to get 1s pretty close when called with pause(60))
        jsr     WAIT

        dex
        bne     :-
        rts
