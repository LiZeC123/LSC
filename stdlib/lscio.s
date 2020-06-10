section .data
section .text
global lscPrintInt
lscPrintInt:
	push ebp
	mov ebp,esp
	sub esp,48
	mov eax,[ebp+8]
	push eax
	lea eax,[ebp-48]
	push eax
	call lscint2str
	add esp,8
	lea eax,[ebp-48]
	push eax
	call lscPrintStr
	add esp,4
.L84:
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
.L32:
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
	je .L33
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4],eax
	jmp .L32
.L33:
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
.L31:
	mov esp,ebp
	pop ebp
	ret
global lscReadInt
lscReadInt:
	push ebp
	mov ebp,esp
	sub esp,52
	mov eax,10
	push eax
	lea eax,[ebp-48]
	push eax
	call lscRead
	add esp,8
	mov eax,10
	push eax
	lea eax,[ebp-48]
	push eax
	call lscstr2int
	add esp,8
	mov [ebp-52],eax
	mov eax,[ebp-52]
	jmp .L29
.L29:
	mov esp,ebp
	pop ebp
	ret
global lscint2str
lscint2str:
	push ebp
	mov ebp,esp
	sub esp,96
	mov eax,[ebp+12]
	mov ebx,0
	mov ecx,0
	cmp eax,ebx
	sete cl
	mov [ebp-4],ecx
	mov eax,[ebp-4]
	cmp eax,0
	je .L40
	mov eax,0
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
	mov al,48
	mov ebx,[ebp-12]
	mov [ebx],eax
	mov eax,1
	mov ebx,1
	imul ebx
	mov [ebp-20],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-20]
	add eax,ebx
	mov [ebp-24],eax
	mov eax,[ebp-24]
	mov eax,[eax]
	mov [ebp-28],al
	mov eax,0
	mov al,0
	mov ebx,[ebp-24]
	mov [ebx],eax
	jmp .L38
	jmp .L47
.L40:
.L47:
	mov eax,0
	mov [ebp-4],eax
	mov eax,[ebp+12]
	mov ebx,0
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-8],ecx
	mov eax,[ebp-8]
	cmp eax,0
	je .L49
	mov eax,1
	mov [ebp-4],eax
	mov eax,[ebp+12]
	neg eax
	mov [ebp-12],eax
	mov eax,[ebp-12]
	mov [ebp+12],eax
	jmp .L51
.L49:
.L51:
	mov eax,0
	mov [ebp-56],eax
.L52:
	mov eax,[ebp+12]
	mov ebx,0
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-60],ecx
	mov eax,[ebp-60]
	cmp eax,0
	je .L53
	jmp .L55
.L56:
	mov eax,[ebp+12]
	mov ebx,10
	mov edx,0
	idiv ebx
	mov [ebp-64],eax
	mov eax,[ebp-64]
	mov [ebp+12],eax
	jmp .L52
.L55:
	mov eax,[ebp-56]
	mov [ebp-68],eax
	mov eax,[ebp-56]
	mov ebx,1
	add eax,ebx
	mov [ebp-56],eax
	mov eax,[ebp-68]
	mov ebx,1
	imul ebx
	mov [ebp-72],eax
	lea eax,[ebp-52]
	mov ebx,[ebp-72]
	add eax,ebx
	mov [ebp-76],eax
	mov eax,[ebp+12]
	mov ebx,10
	mov edx,0
	idiv ebx
	mov [ebp-84],edx
	mov eax,[ebp-84]
	mov ebx,48
	add eax,ebx
	mov [ebp-88],eax
	mov eax,[ebp-76]
	mov eax,[eax]
	mov [ebp-80],al
	mov eax,[ebp-88]
	mov ebx,[ebp-76]
	mov [ebx],eax
	jmp .L56
.L53:
	mov eax,0
	mov [ebp-60],eax
	mov eax,[ebp-4]
	cmp eax,0
	je .L64
	mov eax,[ebp-60]
	mov [ebp-64],eax
	mov eax,[ebp-60]
	mov ebx,1
	add eax,ebx
	mov [ebp-60],eax
	mov eax,[ebp-64]
	mov ebx,1
	imul ebx
	mov [ebp-68],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-68]
	add eax,ebx
	mov [ebp-72],eax
	mov eax,[ebp-72]
	mov eax,[eax]
	mov [ebp-76],al
	mov eax,0
	mov al,45
	mov ebx,[ebp-72]
	mov [ebx],eax
	jmp .L69
.L64:
.L69:
	mov eax,[ebp-56]
	mov ebx,1
	sub eax,ebx
	mov [ebp-56],eax
.L70:
	mov eax,[ebp-56]
	mov ebx,0
	mov ecx,0
	cmp eax,ebx
	setge cl
	mov [ebp-64],ecx
	mov eax,[ebp-64]
	cmp eax,0
	je .L71
	mov eax,[ebp-60]
	mov [ebp-68],eax
	mov eax,[ebp-60]
	mov ebx,1
	add eax,ebx
	mov [ebp-60],eax
	mov eax,[ebp-68]
	mov ebx,1
	imul ebx
	mov [ebp-72],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-72]
	add eax,ebx
	mov [ebp-76],eax
	mov eax,[ebp-56]
	mov [ebp-84],eax
	mov eax,[ebp-56]
	mov ebx,1
	sub eax,ebx
	mov [ebp-56],eax
	mov eax,[ebp-84]
	mov ebx,1
	imul ebx
	mov [ebp-88],eax
	lea eax,[ebp-52]
	mov ebx,[ebp-88]
	add eax,ebx
	mov [ebp-92],eax
	mov eax,[ebp-92]
	mov eax,[eax]
	mov [ebp-96],al
	mov eax,[ebp-76]
	mov eax,[eax]
	mov [ebp-80],al
	mov eax,0
	mov al,[ebp-96]
	mov ebx,[ebp-76]
	mov [ebx],eax
	jmp .L70
.L71:
	mov eax,[ebp-60]
	mov ebx,1
	imul ebx
	mov [ebp-64],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-64]
	add eax,ebx
	mov [ebp-68],eax
	mov eax,[ebp-68]
	mov eax,[eax]
	mov [ebp-72],al
	mov eax,0
	mov al,0
	mov ebx,[ebp-68]
	mov [ebx],eax
.L38:
	mov esp,ebp
	pop ebp
	ret
global lscstr2int
lscstr2int:
	push ebp
	mov ebp,esp
	sub esp,84
	mov eax,0
	mov [ebp-4],eax
	mov eax,0
	mov [ebp-8],eax
	mov eax,1
	mov [ebp-12],eax
	mov eax,[ebp-4]
	mov ebx,1
	imul ebx
	mov [ebp-16],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-16]
	add eax,ebx
	mov [ebp-20],eax
	mov eax,[ebp-20]
	mov eax,[eax]
	mov [ebp-24],al
	mov eax,0
	mov al,[ebp-24]
	mov ebx,0
	mov bl,45
	mov ecx,0
	cmp eax,ebx
	sete cl
	mov [ebp-28],ecx
	mov eax,[ebp-28]
	cmp eax,0
	je .L6
	mov eax,1
	neg eax
	mov [ebp-32],eax
	mov eax,[ebp-32]
	mov [ebp-12],eax
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4],eax
	jmp .L8
.L6:
.L8:
.L9:
	mov eax,[ebp-4]
	mov ebx,1
	imul ebx
	mov [ebp-16],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-16]
	add eax,ebx
	mov [ebp-20],eax
	mov eax,[ebp-20]
	mov eax,[eax]
	mov [ebp-24],al
	mov eax,0
	mov al,[ebp-24]
	mov ebx,0
	mov bl,10
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-28],ecx
	mov eax,[ebp-4]
	mov ebx,1
	imul ebx
	mov [ebp-32],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-32]
	add eax,ebx
	mov [ebp-36],eax
	mov eax,[ebp-36]
	mov eax,[eax]
	mov [ebp-40],al
	mov eax,0
	mov al,[ebp-40]
	mov ebx,0
	mov bl,0
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-44],ecx
	mov eax,[ebp-28]
	cmp eax,0
	setne cl
	mov ebx,[ebp-44]
	cmp ebx,0
	setne bl
	and eax,ebx
	mov [ebp-48],eax
	mov eax,[ebp-4]
	mov ebx,[ebp+12]
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-52],ecx
	mov eax,[ebp-48]
	cmp eax,0
	setne cl
	mov ebx,[ebp-52]
	cmp ebx,0
	setne bl
	and eax,ebx
	mov [ebp-56],eax
	mov eax,[ebp-56]
	cmp eax,0
	je .L10
	mov eax,[ebp-4]
	mov ebx,1
	imul ebx
	mov [ebp-60],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-60]
	add eax,ebx
	mov [ebp-64],eax
	mov eax,[ebp-64]
	mov eax,[eax]
	mov [ebp-68],al
	mov eax,0
	mov al,[ebp-68]
	mov ebx,48
	sub eax,ebx
	mov [ebp-72],eax
	mov eax,[ebp-72]
	mov [ebp-76],eax
	mov eax,[ebp-8]
	mov ebx,10
	imul ebx
	mov [ebp-80],eax
	mov eax,[ebp-80]
	mov ebx,[ebp-76]
	add eax,ebx
	mov [ebp-84],eax
	mov eax,[ebp-84]
	mov [ebp-8],eax
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4],eax
	jmp .L9
.L10:
	mov eax,[ebp-12]
	mov ebx,[ebp-8]
	imul ebx
	mov [ebp-16],eax
	mov eax,[ebp-16]
	jmp .L1
.L1:
	mov esp,ebp
	pop ebp
	ret
