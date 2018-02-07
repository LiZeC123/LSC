section .data
global globalChar
	globalChar db 0
	@L30 db "true",0
	@L33 db "false",0
section .text
global int2str
int2str:
	push ebp
	mov ebp,esp
	sub esp,40
	mov eax,0
	mov [ebp-4],eax
.L51:
	mov eax,[ebp+12]
	mov ebx,0
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-8],ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L52
	mov eax,[ebp+12]
	mov ebx,10
	mov edx,0
	idiv ebx
	mov [ebp-12],edx
	mov eax,[ebp-12]
	mov [ebp-16],eax
	mov eax,[ebp-4]
	mov ebx,1
	imul ebx
	mov [ebp-20],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-20]
	add eax,ebx
	mov [ebp-24],eax
	mov eax,[ebp-16]
	mov ebx,48
	add eax,ebx
	mov [ebp-32],eax
	mov eax,[ebp-32]
	mov ebx,[ebp-24]
	mov [ebx],eax
	mov eax,[ebp-4]
	mov [ebp-36],eax
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4],eax
	mov eax,[ebp+12]
	mov ebx,10
	mov edx,0
	idiv ebx
	mov [ebp-40],eax
	mov eax,[ebp-40]
	mov [ebp+12],eax
	jmp .L51
.L52:
.L50:
	mov esp,ebp
	pop ebp
	ret
global main
main:
	push ebp
	mov ebp,esp
	sub esp,48
	mov eax,5
	mov [ebp-24],eax
	mov eax,4
	mov [ebp-28],eax
	mov eax,3
	mov [ebp-32],eax
	mov eax,[ebp-28]
	mov ebx,[ebp-32]
	imul ebx
	mov [ebp-40],eax
	mov eax,[ebp-24]
	mov ebx,[ebp-40]
	add eax,ebx
	mov [ebp-44],eax
	mov eax,[ebp-44]
	mov [ebp-36],eax
	mov eax,[ebp-36]
	push eax
	lea eax,[ebp-20]
	push eax
	call int2str
	add esp,8
	lea eax,[ebp-20]
	push eax
	call printf
	add esp,4
	mov [ebp-48],eax
	mov eax,[ebp-36]
	jmp .L1
.L1:
	mov esp,ebp
	pop ebp
	ret
global printf
printf:
	push ebp
	mov ebp,esp
	mov eax,4
	mov ebx,1
	mov ecx,[ebp+8]
	mov edx,2
	int 128
	mov esp,ebp
	pop ebp
	ret
global testArray
testArray:
	push ebp
	mov ebp,esp
	sub esp,104
	mov eax,1
	mov ebx,4
	imul ebx
	mov [ebp-44],eax
	lea eax,[ebp-40]
	mov ebx,[ebp-44]
	add eax,ebx
	mov [ebp-48],eax
	mov eax,5
	mov ebx,[ebp-48]
	mov [ebx],eax
	mov eax,2
	mov ebx,4
	imul ebx
	mov [ebp-56],eax
	lea eax,[ebp-40]
	mov ebx,[ebp-56]
	add eax,ebx
	mov [ebp-60],eax
	mov eax,0
	mov ebx,[ebp-60]
	mov [ebx],eax
	mov eax,1
	mov ebx,4
	imul ebx
	mov [ebp-72],eax
	lea eax,[ebp-40]
	mov ebx,[ebp-72]
	add eax,ebx
	mov [ebp-76],eax
	mov eax,2
	mov ebx,4
	imul ebx
	mov [ebp-84],eax
	lea eax,[ebp-40]
	mov ebx,[ebp-84]
	add eax,ebx
	mov [ebp-88],eax
	mov eax,[ebp-76]
	mov eax,[eax]
	mov [ebp-96],eax
	mov eax,[ebp-88]
	mov eax,[eax]
	mov [ebp-100],eax
	mov eax,[ebp-96]
	mov ebx,[ebp-100]
	add eax,ebx
	mov [ebp-104],eax
	mov eax,[ebp-104]
	mov [ebp-68],eax
.L11:
	mov esp,ebp
	pop ebp
	ret
global testAssgin
testAssgin:
	push ebp
	mov ebp,esp
	sub esp,36
	mov eax,10
	mov [ebp-4],eax
	lea eax,[ebp-4]
	mov [ebp-8],eax
	mov eax,[ebp-8]
	mov [ebp-12],eax
	mov eax,[ebp-12]
	mov eax,[eax]
	mov [ebp-20],eax
	mov eax,[ebp-20]
	mov [ebp-24],eax
	lea eax,[ebp-24]
	mov [ebp-28],eax
	mov eax,[ebp-28]
	mov [ebp-32],eax
	mov eax,[ebp-24]
	mov ebx,[ebp-32]
	mov [ebx],eax
.L5:
	mov esp,ebp
	pop ebp
	ret
global testBreak
testBreak:
	push ebp
	mov ebp,esp
	sub esp,24
	mov eax,0
	mov [ebp-4],eax
.L41:
	mov eax,[ebp-4]
	mov ebx,5
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-8],ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L42
	mov eax,0
	mov [ebp-12],eax
.L44:
	mov eax,[ebp-12]
	mov ebx,5
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-16],ecx
	mov eax,[ebp-16]
	cmp eax,0
	je .L45
	mov eax,[ebp-12]
	mov ebx,2
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-20],ecx
	mov eax,[ebp-20]
	cmp eax,0
	je .L48
	mov eax,0
	mov [ebp-24],eax
	mov eax,[ebp-24]
	mov ebx,1
	add eax,ebx
	mov [ebp-24],eax
	jmp .L44
	jmp .L49
.L48:
	mov eax,0
	mov [ebp-20],eax
	mov eax,[ebp-20]
	mov ebx,1
	add eax,ebx
	mov [ebp-20],eax
	jmp .L45
.L49:
	jmp .L44
.L45:
	jmp .L41
.L42:
.L40:
	mov esp,ebp
	pop ebp
	ret
global testIf
testIf:
	push ebp
	mov ebp,esp
	sub esp,12
	mov eax,5
	mov [ebp-4],eax
	mov eax,[ebp-4]
	mov ebx,5
	mov ecx,0
	cmp eax,ebx
	sete cl
	mov [ebp-8],ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L29
	mov eax,@L30
	push eax
	call printf
	add esp,4
	mov [ebp-12],eax
	jmp .L32
.L29:
	mov eax,@L33
	push eax
	call printf
	add esp,4
	mov [ebp-8],eax
.L32:
.L27:
	mov esp,ebp
	pop ebp
	ret
global testWhile
testWhile:
	push ebp
	mov ebp,esp
	sub esp,12
	mov eax,0
	mov [ebp-4],eax
.L36:
	mov eax,[ebp-4]
	mov ebx,5
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-8],ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L37
	call testIf
	add esp,0
	mov eax,[ebp-4]
	mov [ebp-12],eax
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4],eax
	jmp .L36
.L37:
.L35:
	mov esp,ebp
	pop ebp
	ret
