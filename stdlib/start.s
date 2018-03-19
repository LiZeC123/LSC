section .data
section .text
global @start
@start:
    pop esi
    mov ecx,esp
    and esp,4294967280
    push ecx
    push esi
    call main
    mov eax,1
    mov ebx,0
    int 128