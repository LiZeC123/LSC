section .data
	@L10 db 10,0
	@L11 db 10,0
	@L12 db "Finish!",0
	@L8 db 10,0
	@L9 db "MAX_SIZE",0
section .text
global main
main:
	push ebp
	mov ebp,esp
	sub esp,72
	mov eax,0
	mov ebx,4
	imul ebx
	mov [ebp-44],eax
	lea eax,[ebp-40]
	mov ebx,[ebp-44]
	add eax,ebx
	mov [ebp-48],eax
	mov eax,[ebp-48]
	mov eax,[eax]
	mov [ebp-52],eax
	mov eax,10
	mov ebx,[ebp-48]
	mov [ebx],eax
	mov eax,0
	mov ebx,4
	imul ebx
	mov [ebp-56],eax
	lea eax,[ebp-40]
	mov ebx,[ebp-56]
	add eax,ebx
	mov [ebp-60],eax
	mov eax,[ebp-60]
	mov eax,[eax]
	mov [ebp-64],eax
	mov eax,[ebp-64]
	push eax
	call lscPrintInt
	add esp,4
	mov eax,@L8
	push eax
	call lscPrintStr
	add esp,4
	mov eax,@L9
	mov [ebp-68],eax
	mov eax,[ebp-68]
	push eax
	call lscPrintStr
	add esp,4
	mov eax,@L10
	push eax
	call lscPrintStr
	add esp,4
	mov eax,12
	mov [ebp-72],eax
	mov eax,[ebp-72]
	push eax
	call lscPrintInt
	add esp,4
	mov eax,@L11
	push eax
	call lscPrintStr
	add esp,4
	mov eax,@L12
	push eax
	call lscPrintStr
	add esp,4
.L1:
	mov esp,ebp
	pop ebp
	ret
