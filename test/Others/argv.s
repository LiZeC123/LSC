section .data
section .text
global @start
@start:
    call preMain
    mov eax,1
    mov ebx,0
    int 128
global preMain
preMain:
    push eax
    push ebp
    mov ebp esp
    mov eax,[esp]   ; 获取数组长度
    add eax,2       ; 为*s 和i 分配空间
    mov ebx,4
    imul ebx
    sub esp,eax
    mov eax,1       ; init i = 1
    mov [ebp-8],eax
    lea eax,[ebp+12]; init s = &arr[0]
    mov [ebp-4],eax 
    mov [ebp-20],eax
.forHead:
    mov eax,[ebp-8] ; if i < argv ?
	mov ebx,[ebp+8]
	mov ecx,0
	cmp eax,ebx
	setl cl
    cmp ecx,0
	je .forEnd
.ifHead:
    mov eax,0
    mov eax,[ebp-4]
	mov al,[eax]
    mov ecx,0
    mov ebx,0
    cmp al,bl
    setne cl
    cmp ecx,0
    je .ifOther      ; if *s = ' ' goto
    mov eax,[ebp-4]
    add eax,1
    mov [ebp-4],eax
    jmp .ifHead
.ifOther:
    mov eax,[ebp-8]
    mov ebx,4
    imul ebx
    lea ebx,[ebp-20]
    add eax, ebx        ; eax = &a[i]
    mov ebx,[ebp-4]
    add ebx, 1          ; s=> '\0' , s+1 => next head
    mov [ebp-4],ebx
    mov [eax],ebx       ; a[i] = *s
    mov eax,[ebp-8]     ; i = i + 1
    add eax,1
    mov [ebp-8],eax
    jmp .forHead
.forEnd:
    lea eax [edx]       
    push eax        ; push &argv
    mov eax,[ebp+4]
    push eax        ; push argv
    call main