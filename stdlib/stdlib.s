section .text
global lscPrints
lscPrints:
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

