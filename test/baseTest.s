section .data
global globalChar
	globalChar dd 0
	@L11 db "5 + 4 * 3 = ",0
	@L12 db 10,0
	@L2 db "Hello World!",10,0
section .text
global main
main:
	push ebp
	mov ebp,esp
	sub esp,4
	mov eax,@L2
	mov [globalChar],eax
	mov eax,[globalChar]
	push eax
	call lscPrintStr
	add esp,4
	call testPrintNum
	add esp,0
	mov [ebp-4],eax
	mov eax,0
	jmp .L1
.L1:
	mov esp,ebp
	pop ebp
	ret
global testAssgin
testAssgin:
	push ebp
	mov ebp,esp
	sub esp,32
	mov eax,8
	mov [ebp-4],eax
	lea eax,[ebp-4]
	mov [ebp-8],eax
	mov eax,[ebp-8]
	mov [ebp-12],eax
	mov eax,[ebp-12]
	mov eax,[eax]
	mov [ebp-20],eax
	mov eax,[ebp-20]
	mov [ebp-24],eax
	mov eax,[ebp-24]
	mov ebx,2010
	add eax,ebx
	mov [ebp-28],eax
	mov eax,[ebp-28]
	mov [ebp-24],eax
	mov eax,[ebp-24]
	mov ebx,[ebp-12]
	mov [ebx],eax
	mov eax,[ebp-4]
	jmp .L4
.L4:
	mov esp,ebp
	pop ebp
	ret
global testPrintNum
testPrintNum:
	push ebp
	mov ebp,esp
	sub esp,52
	mov eax,@L11
	mov [ebp-24],eax
	mov eax,@L12
	mov [ebp-28],eax
	mov eax,5
	mov [ebp-32],eax
	mov eax,4
	mov [ebp-36],eax
	mov eax,3
	mov [ebp-40],eax
	mov eax,[ebp-36]
	mov ebx,[ebp-40]
	imul ebx
	mov [ebp-48],eax
	mov eax,[ebp-32]
	mov ebx,[ebp-48]
	add eax,ebx
	mov [ebp-52],eax
	mov eax,[ebp-52]
	mov [ebp-44],eax
	mov eax,[ebp-44]
	push eax
	lea eax,[ebp-20]
	push eax
	call lscint2str
	add esp,8
	mov eax,[ebp-24]
	push eax
	call lscPrintStr
	add esp,4
	lea eax,[ebp-20]
	push eax
	call lscPrintStr
	add esp,4
	mov eax,[ebp-28]
	push eax
	call lscPrintStr
	add esp,4
.L10:
	mov esp,ebp
	pop ebp
	ret
