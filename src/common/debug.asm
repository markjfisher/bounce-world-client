; Coco (6809) version of data.s

	EXPORT _debug

	SECTION code

; just an RTS for us to latch a breakpoint onto in Altirra/AppleWin etc.
_debug:
	RTS

	ENDSECTION
