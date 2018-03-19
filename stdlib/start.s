section .data
section .text
global @start
@start:
    pop esi
    mov ecx,esp
    and esp,0xfffffff0
    push ecx
    push esi
    call main
    mov eax,1
    mov ebx,0
    int 128