section .data
section .text
global lscWrite
lscWrite:
    push ebp
	mov ebp,esp
	mov eax,4
	mov ebx,1
	mov ecx,[ebp+8]
	mov edx,[ebp+12]
	int 128
	mov esp,ebp
	pop ebp
	ret
global lscRead
lscRead:
	push ebp
	mov ebp,esp
	mov eax,3
	mov ebx,1
	mov ecx,[ebp+8]
	mov edx,[ebp+12]
	int 128
	mov esp,ebp
	pop ebp
	ret
global lscPrintInt
lscPrintInt:
	push ebp
	mov ebp,esp
	sub esp,40
	mov eax,[ebp+8]
	push eax
	lea eax,[ebp-40]
	push eax
	call lscint2str
	add esp,8
	lea eax,[ebp-40]
	push eax
	call lscPrintStr
	add esp,4
.L55:
	mov esp,ebp
	pop ebp
	ret
global lscPrintStr
lscPrintStr:
	push ebp
	mov ebp,esp
	sub esp,20
	mov eax,0
	mov [ebp-4],eax
.L26:
	mov eax,[ebp-4]
	mov ebx,1
	imul ebx
	mov [ebp-8],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-8]
	add eax,ebx
	mov [ebp-12],eax
	mov eax,[ebp-12]
	mov eax,[eax]
	mov [ebp-16],al
	mov eax,0
	mov al,[ebp-16]
	mov ebx,0
	mov bl,0
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-20],ecx
	mov eax,[ebp-20]
	cmp eax,0
	je .L27
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4],eax
	jmp .L26
.L27:
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4],eax
	mov eax,[ebp-4]
	push eax
	mov eax,[ebp+8]
	push eax
	call lscWrite
	add esp,8
.L25:
	mov esp,ebp
	pop ebp
	ret
global lscReadInt
lscReadInt:
	push ebp
	mov ebp,esp
	sub esp,44
	mov eax,10
	push eax
	lea eax,[ebp-40]
	push eax
	call lscRead
	add esp,8
	mov eax,10
	push eax
	lea eax,[ebp-40]
	push eax
	call lscstr2int
	add esp,8
	mov [ebp-44],eax
	mov eax,[ebp-44]
	jmp .L23
.L23:
	mov esp,ebp
	pop ebp
	ret
