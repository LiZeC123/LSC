	.file	"baseTest.c"
	.intel_syntax noprefix
	.comm	globalChar,4,4
	.section	.rodata
.LC0:
	.string	"Hello World!\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	lea	ecx, [esp+4]
	.cfi_def_cfa 1, 0
	and	esp, -16
	push	DWORD PTR [ecx-4]
	push	ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	mov	ebp, esp
	push	ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	sub	esp, 36
	mov	DWORD PTR [ebp-32], OFFSET FLAT:.LC0
	sub	esp, 12
	push	DWORD PTR [ebp-32]
	call	printf
	add	esp, 16
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], 5
	mov	DWORD PTR [ebp-20], 4
	mov	DWORD PTR [ebp-16], 3
	mov	eax, DWORD PTR [ebp-20]
	imul	eax, DWORD PTR [ebp-16]
	mov	edx, eax
	mov	eax, DWORD PTR [ebp-24]
	add	eax, edx
	mov	DWORD PTR [ebp-12], eax
	mov	eax, DWORD PTR [ebp-12]
	mov	ecx, DWORD PTR [ebp-4]
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	lea	esp, [ecx-4]
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.globl	testAssgin
	.type	testAssgin, @function
testAssgin:
.LFB1:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 40
	mov	eax, DWORD PTR gs:20
	mov	DWORD PTR [ebp-12], eax
	xor	eax, eax
	mov	DWORD PTR [ebp-28], 10
	lea	eax, [ebp-28]
	mov	DWORD PTR [ebp-20], eax
	mov	eax, DWORD PTR [ebp-20]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR [ebp-24], eax
	lea	eax, [ebp-24]
	mov	DWORD PTR [ebp-16], eax
	mov	edx, DWORD PTR [ebp-24]
	mov	eax, DWORD PTR [ebp-16]
	mov	DWORD PTR [eax], edx
	nop
	mov	ecx, DWORD PTR [ebp-12]
	xor	ecx, DWORD PTR gs:20
	je	.L4
	call	__stack_chk_fail
.L4:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	testAssgin, .-testAssgin
	.globl	testArray
	.type	testArray, @function
testArray:
.LFB2:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 56
	mov	eax, DWORD PTR gs:20
	mov	DWORD PTR [ebp-12], eax
	xor	eax, eax
	mov	DWORD PTR [ebp-48], 5
	mov	DWORD PTR [ebp-44], 0
	mov	edx, DWORD PTR [ebp-48]
	mov	eax, DWORD PTR [ebp-44]
	add	eax, edx
	mov	DWORD PTR [ebp-56], eax
	nop
	mov	ecx, DWORD PTR [ebp-12]
	xor	ecx, DWORD PTR gs:20
	je	.L6
	call	__stack_chk_fail
.L6:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	testArray, .-testArray
	.section	.rodata
.LC1:
	.string	"true"
.LC2:
	.string	"false"
	.text
	.globl	testIf
	.type	testIf, @function
testIf:
.LFB3:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 24
	mov	DWORD PTR [ebp-12], 5
	cmp	DWORD PTR [ebp-12], 5
	jne	.L8
	sub	esp, 12
	push	OFFSET FLAT:.LC1
	call	printf
	add	esp, 16
	jmp	.L10
.L8:
	sub	esp, 12
	push	OFFSET FLAT:.LC2
	call	printf
	add	esp, 16
.L10:
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	testIf, .-testIf
	.globl	testWhile
	.type	testWhile, @function
testWhile:
.LFB4:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 24
	mov	DWORD PTR [ebp-12], 0
	jmp	.L12
.L13:
	call	testIf
	add	DWORD PTR [ebp-12], 1
.L12:
	cmp	DWORD PTR [ebp-12], 4
	jle	.L13
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	testWhile, .-testWhile
	.globl	testBreak
	.type	testBreak, @function
testBreak:
.LFB5:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 16
	mov	DWORD PTR [ebp-16], 0
	jmp	.L15
.L19:
	mov	DWORD PTR [ebp-12], 0
	jmp	.L16
.L18:
	cmp	DWORD PTR [ebp-12], 2
	je	.L17
	mov	DWORD PTR [ebp-8], 0
	add	DWORD PTR [ebp-8], 1
	jmp	.L16
.L17:
	mov	DWORD PTR [ebp-4], 0
	add	DWORD PTR [ebp-4], 1
	jmp	.L15
.L16:
	cmp	DWORD PTR [ebp-12], 4
	jle	.L18
.L15:
	cmp	DWORD PTR [ebp-16], 4
	jle	.L19
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	testBreak, .-testBreak
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.6) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
