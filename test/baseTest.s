section .data
global globalChar
	globalChar db 0
	@L2 db "Hello World!",0
section .text
global int2str
int2str:
	push ebp
	mov ebp,esp
	sub esp,40
	mov eax,0
	mov [ebp-4],eax
.L15:
	mov eax,[ebp+12]
	mov ebx,0
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-8],ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L16
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
	jmp .L15
.L16:
.L14:
	mov esp,ebp
	pop ebp
	ret
global main
main:
	push ebp
	mov ebp,esp
	sub esp,4
	mov eax,@L2
	mov [globalChar],eax
	mov eax,12
	push eax
	mov eax,[globalChar]
	push eax
	call lscPrints
	add esp,8
	mov [ebp-4],eax
	mov eax,0
	jmp .L1
.L1:
	mov esp,ebp
	pop ebp
	ret
global testAssgin
testAssgin:
	push ebp
	mov ebp,esp
	sub esp,32
	mov eax,8
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
	mov eax,[ebp-24]
	mov ebx,2010
	add eax,ebx
	mov [ebp-28],eax
	mov eax,[ebp-28]
	mov [ebp-24],eax
	mov eax,[ebp-24]
	mov ebx,[ebp-12]
	mov [ebx],eax
	mov eax,[ebp-4]
	jmp .L4
.L4:
	mov esp,ebp
	pop ebp
	ret
global testPrintNum
testPrintNum:
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
	mov eax,2
	push eax
	lea eax,[ebp-20]
	push eax
	call lscPrints
	add esp,8
	mov [ebp-48],eax
.L10:
	mov esp,ebp
	pop ebp
	ret
