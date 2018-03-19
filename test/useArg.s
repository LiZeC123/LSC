section .data
section .text
global main
main:
	push ebp
	mov ebp,esp
	sub esp,0
	mov eax,[ebp+8]
	push eax
	call lscPrintInt
	add esp,4
	mov eax,0
	jmp .L1
.L1:
	mov esp,ebp
	pop ebp
	ret
