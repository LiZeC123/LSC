section .data
global globalChar
	globalChar db 0
	.L2 db Hello World!\n\000
	.L31 db true\000
	.L34 db false\000
section .text
global main
main:
	push ebp
	mov ebp,esp
	sub esp,36
	mov eax,.L2
	mov [ebp-4] eax
	mov eax,[ebp-4]
	push eax
	call printf
	add esp,4
	mov [ebp-8] eax
	mov eax,[ebp-8]
	mov [ebp-12] eax
	mov eax,5
	mov [ebp-16] eax
	mov eax,4
	mov [ebp-20] eax
	mov eax,3
	mov [ebp-24] eax
	mov eax,[ebp-20]
	mov ebx,[ebp-24]
	imul ebx
	mov [ebp-32] eax
	mov eax,[ebp-16]
	mov ebx,[ebp-32]
	add eax,ebx
	mov [ebp-36] eax
	mov eax,[ebp-36]
	mov [ebp-28] eax
	mov eax,[ebp-28]
	jmp .L1
.L1:
	mov esp,ebp
	pop ebp
	ret
global printf
printf:
global testArray
testArray:
	push ebp
	mov ebp,esp
	sub esp,104
	mov eax,1
	mov ebx,4
	imul ebx
	mov [ebp-44] eax
	lea eax,[ebp-40]
	mov ebx,[ebp-44]
	add eax,ebx
	mov [ebp-48] eax
	mov eax,5
	mov ebx,[ebp-48]
	mov [ebx],eax
	mov eax,2
	mov ebx,4
	imul ebx
	mov [ebp-56] eax
	lea eax,[ebp-40]
	mov ebx,[ebp-56]
	add eax,ebx
	mov [ebp-60] eax
	mov eax,0
	mov ebx,[ebp-60]
	mov [ebx],eax
	mov eax,1
	mov ebx,4
	imul ebx
	mov [ebp-72] eax
	lea eax,[ebp-40]
	mov ebx,[ebp-72]
	add eax,ebx
	mov [ebp-76] eax
	mov eax,2
	mov ebx,4
	imul ebx
	mov [ebp-84] eax
	lea eax,[ebp-40]
	mov ebx,[ebp-84]
	add eax,ebx
	mov [ebp-88] eax
	mov eax,[ebp-76]
	mov eax,[eax]
	mov [ebp-96] eax
	mov eax,[ebp-88]
	mov eax,[eax]
	mov [ebp-100] eax
	mov eax,[ebp-96]
	mov ebx,[ebp-100]
	add eax,ebx
	mov [ebp-104] eax
	mov eax,[ebp-104]
	mov [ebp-68] eax
.L12:
	mov esp,ebp
	pop ebp
	ret
global testAssgin
testAssgin:
	push ebp
	mov ebp,esp
	sub esp,36
	mov eax,10
	mov [ebp-4] eax
	lea eax [ebp-4]
	mov [ebp-8] eax
	mov eax,[ebp-8]
	mov [ebp-12] eax
	mov eax,[ebp-12]
	mov eax,[eax]
	mov [ebp-20] eax
	mov eax,[ebp-20]
	mov [ebp-24] eax
	lea eax [ebp-24]
	mov [ebp-28] eax
	mov eax,[ebp-28]
	mov [ebp-32] eax
	mov eax,[ebp-24]
	mov ebx,[ebp-32]
	mov [ebx],eax
.L6:
	mov esp,ebp
	pop ebp
	ret
global testBreak
testBreak:
	push ebp
	mov ebp,esp
	sub esp,24
	mov eax,0
	mov [ebp-4] eax
.L42:
	mov eax,[ebp-4]
	mov ebx,5
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-8] ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L43
	mov eax,0
	mov [ebp-12] eax
.L45:
	mov eax,[ebp-12]
	mov ebx,5
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-16] ecx
	mov eax,[ebp-16]
	cmp eax,0
	je .L46
	mov eax,[ebp-12]
	mov ebx,2
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-20] ecx
	mov eax,[ebp-20]
	cmp eax,0
	je .L49
	mov eax,0
	mov [ebp-24] eax
	mov eax,[ebp-24]
	mov ebx,1
	add eax,ebx
	mov [ebp-24] eax
	jmp .L45
	jmp .L50
.L49:
	mov eax,0
	mov [ebp-20] eax
	mov eax,[ebp-20]
	mov ebx,1
	add eax,ebx
	mov [ebp-20] eax
	jmp .L46
.L50:
	jmp .L45
.L46:
	jmp .L42
.L43:
.L41:
	mov esp,ebp
	pop ebp
	ret
global testIf
testIf:
	push ebp
	mov ebp,esp
	sub esp,12
	mov eax,5
	mov [ebp-4] eax
	mov eax,[ebp-4]
	mov ebx,5
	mov ecx,0
	cmp eax,ebx
	sete cl
	mov [ebp-8] ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L30
	mov eax,.L31
	push eax
	call printf
	add esp,4
	mov [ebp-12] eax
	jmp .L33
.L30:
	mov eax,.L34
	push eax
	call printf
	add esp,4
	mov [ebp-8] eax
.L33:
.L28:
	mov esp,ebp
	pop ebp
	ret
global testWhile
testWhile:
	push ebp
	mov ebp,esp
	sub esp,12
	mov eax,0
	mov [ebp-4] eax
.L37:
	mov eax,[ebp-4]
	mov ebx,5
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-8] ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L38
	call testIf
	add esp,0
	mov eax,[ebp-4]
	mov [ebp-12] eax
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4] eax
	jmp .L37
.L38:
.L36:
	mov esp,ebp
	pop ebp
	ret
