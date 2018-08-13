section .data
section .text
global exit
exit:
    push ebp
	mov ebp,esp
	mov eax,1
	mov ebx,[ebp+8]
	int 128
	mov esp,ebp
	pop ebp
	ret
global lscmalloc
lscmalloc:
    push ebp
	mov ebp,esp
	
    mov eax, 192        ; sys_mmap_pgoff  参考 https://blog.csdn.net/u014089131/article/details/54574964
    mov ebx, 0    	    ; addr = NULL
    mov ecx, [ebp+8]    ; len
	add ecx, 8			; 申请额外的空间
    mov edx, 0x7        ; prot = PROT_READ|PROT_WRITE|PROT_EXEC
    mov esi, 0x22       ; flags = MAP_PRIVATE|MAP_ANONYMOUS
    mov edi, 0xffffffff ; fd = -1
    push ebp            ; 接下来要使用ebp,因此先保存到栈
    mov ebp, 0			; offset = 0
    int 128             ; 系统调用申请内存,返回值保存到eax
	pop ebp            	; 

	mov ebx, 0x4d43534c	; magin number LSCM
	mov [eax],ebx
	mov [eax+4],ecx		; 分配的内存大小
	add eax,8
   
	mov esp,ebp
	pop ebp
	ret
global lscfree
lscfree:
    push ebp
	mov ebp,esp

    mov eax, 91         ; munmap
    mov ebx, [ebp+8]    ; addr
    sub ebx,8           ; 恢复首地址位置
    mov ecx, [ebx+4]    ; 取出保存的大小
    int 128             ; 系统调用释放内存
    
    mov esp,ebp
	pop ebp
	ret
global lscint2str
lscint2str:
	push ebp
	mov ebp,esp
	sub esp,80
	mov eax,0
	mov [ebp-44],eax
.L33:
	mov eax,[ebp+12]
	mov ebx,0
	mov ecx,0
	cmp eax,ebx
	setne cl
	mov [ebp-48],ecx
	mov eax,[ebp-48]
	cmp eax,0
	je .L34
	mov eax,[ebp+12]
	mov ebx,10
	mov edx,0
	idiv ebx
	mov [ebp-52],edx
	mov eax,[ebp-52]
	mov [ebp-56],eax
	mov eax,[ebp-44]
	mov ebx,1
	imul ebx
	mov [ebp-60],eax
	lea eax,[ebp-40]
	mov ebx,[ebp-60]
	add eax,ebx
	mov [ebp-64],eax
	mov eax,[ebp-56]
	mov ebx,48
	add eax,ebx
	mov [ebp-72],eax
	mov eax,[ebp-64]
	mov eax,[eax]
	mov [ebp-68],al
	mov eax,[ebp-72]
	mov ebx,[ebp-64]
	mov [ebx],eax
	mov eax,[ebp-44]
	mov [ebp-76],eax
	mov eax,[ebp-44]
	mov ebx,1
	add eax,ebx
	mov [ebp-44],eax
	mov eax,[ebp+12]
	mov ebx,10
	mov edx,0
	idiv ebx
	mov [ebp-80],eax
	mov eax,[ebp-80]
	mov [ebp+12],eax
	jmp .L33
.L34:
	mov eax,0
	mov [ebp-48],eax
	mov eax,[ebp-44]
	mov ebx,1
	sub eax,ebx
	mov [ebp-44],eax
.L43:
	mov eax,[ebp-44]
	mov ebx,0
	mov ecx,0
	cmp eax,ebx
	setge cl
	mov [ebp-52],ecx
	mov eax,[ebp-52]
	cmp eax,0
	je .L44
	mov eax,[ebp-48]
	mov ebx,1
	imul ebx
	mov [ebp-56],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-56]
	add eax,ebx
	mov [ebp-60],eax
	mov eax,[ebp-44]
	mov ebx,1
	imul ebx
	mov [ebp-68],eax
	lea eax,[ebp-40]
	mov ebx,[ebp-68]
	add eax,ebx
	mov [ebp-72],eax
	mov eax,[ebp-72]
	mov eax,[eax]
	mov [ebp-76],al
	mov eax,[ebp-60]
	mov eax,[eax]
	mov [ebp-64],al
	mov eax,0
	mov al,[ebp-76]
	mov ebx,[ebp-60]
	mov [ebx],eax
	mov eax,[ebp-44]
	mov ebx,1
	sub eax,ebx
	mov [ebp-44],eax
	mov eax,[ebp-48]
	mov ebx,1
	add eax,ebx
	mov [ebp-48],eax
	jmp .L43
.L44:
	mov eax,[ebp-48]
	mov ebx,1
	imul ebx
	mov [ebp-52],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-52]
	add eax,ebx
	mov [ebp-56],eax
	mov eax,[ebp-56]
	mov eax,[eax]
	mov [ebp-60],al
	mov eax,0
	mov al,0
	mov ebx,[ebp-56]
	mov [ebx],eax
.L32:
	mov esp,ebp
	pop ebp
	ret
global lscstr2int
lscstr2int:
	push ebp
	mov ebp,esp
	sub esp,64
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
	mov ebx,[ebp+12]
	mov ecx,0
	cmp eax,ebx
	setl cl
	mov [ebp-32],ecx
	mov eax,[ebp-28]
	cmp eax,0
	setne cl
	mov ebx,[ebp-32]
	cmp ebx,0
	setne bl
	and eax,ebx
	mov [ebp-36],eax
	mov eax,[ebp-36]
	cmp eax,0
	je .L10
	mov eax,[ebp-4]
	mov ebx,1
	imul ebx
	mov [ebp-40],eax
	mov eax,[ebp+8]
	mov ebx,[ebp-40]
	add eax,ebx
	mov [ebp-44],eax
	mov eax,[ebp-44]
	mov eax,[eax]
	mov [ebp-48],al
	mov eax,0
	mov al,[ebp-48]
	mov ebx,48
	sub eax,ebx
	mov [ebp-52],eax
	mov eax,[ebp-52]
	mov [ebp-56],eax
	mov eax,[ebp-8]
	mov ebx,10
	imul ebx
	mov [ebp-60],eax
	mov eax,[ebp-60]
	mov ebx,[ebp-56]
	add eax,ebx
	mov [ebp-64],eax
	mov eax,[ebp-64]
	mov [ebp-8],eax
	mov eax,[ebp-4]
	mov ebx,1
	add eax,ebx
	mov [ebp-4],eax
	jmp .L9
.L10:
	mov eax,[ebp-8]
	jmp .L1
.L1:
	mov esp,ebp
	pop ebp
	ret
