set disassembly-flavor intel
display /x $eax
display /x $ebx
display /x $ecx
display /x $edx
display /x $ebp
display /x $esp
display /i $pc
b *0x8048080
r