	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0
	.globl	__Z3foov                        ; -- Begin function _Z3foov
	.p2align	2
__Z3foov:                               ; @_Z3foov
	.cfi_startproc
; %bb.0:
	adrp	x8, _B@PAGE
	ldr	w9, [x8, _B@PAGEOFF]
	add	w9, w9, #1
	adrp	x10, _A@PAGE
	str	w9, [x10, _A@PAGEOFF]
	str	wzr, [x8, _B@PAGEOFF]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	mov	w0, #0                          ; =0x0
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_A                              ; @A
.zerofill __DATA,__common,_A,4,2
	.globl	_B                              ; @B
.zerofill __DATA,__common,_B,4,2
.subsections_via_symbols
