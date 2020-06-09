section .data
section .text
global random
random:
    push ebp
	mov ebp,esp
	mov eax,1
	mov ebx,[ebp+8]
	int 128
	mov esp,ebp
	pop ebp
	ret
