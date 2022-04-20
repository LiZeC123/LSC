section .data
section .text
global @start
@start:
    call main
    mov eax,1
    mov ebx,0
    int 128
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
global lscp2i
lscp2i:
	push ebp
	mov ebp,esp
	mov eax [ebp+8]		; 将输入的数据原样输出
    mov esp,ebp
	pop ebp
	ret