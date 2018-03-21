标准库与运行时介绍
===============

本节介绍程序入口设置,lsc语言标准库内容,以及lsc语言运行时.


程序入口点
------------

在链接阶段,会强制加入一个START符号,其值对应为`@start`,并且将最后的可执行文件的起始位置直接指向这个符号的位置. 
而实际上该符号来自lsc语言提供的start.o文件,该文件对应的汇编代码内容如下
``` assembly
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
```
start.o的主要内容就是调用main函数,并且在main函数结束后,通过系统调用结束整个程序. 根据 #3 的讨论,按照与GCC相同的方法,start.o在调用main函数前对命令行参数进行了处理并将栈指针对齐.

标准I/O库
---------

在任何一个语言中,标准库都是一个重要的部分. 由于lsc语言并不内置输入输出有关的语法元素, 因此关于输入和输出的函数作为标准库的一部分提供给用户.

在项目的stdlib文件夹下,提供了lscio.h和stdlib.o两个文件,其中头文件定义了所有的标准I/O库函数,而.o文件提供了实现的代码. 

实际上,lsc的标准I/O库是由汇编和lsc语言构成的,其中
``` c
void lscRead(char* buf,int maxLen);
void lscWrite(char* buf,int len);
```
是两个基础的输入输出函数,它们是直接由汇编语言编写的,而其他的
``` c
int lscstr2int(char* str);
void lscint2str(char* buf,int n);

void lscPrintStr(char* str);
void lscPrintInt(int n);

int lscReadInt();
```
等函数,是由lsc语言编写的,他们都是输入输出相关的函数,并且最后都直接或间接的调用最初由汇编编写的两个函数.

标准库的stdlib.o文件生成过程如下
1. 使用汇编编写基础函数 `base.s`
2. 使用lsc语言编写基础函数上的高级函数 `basefun.c`
3. 使用编译器`lscc`将`basefun.c`编译成汇编语言 `basefun.s`
4. 合并`base.s`和`basefun.s`为`stdlib.s`
5. 使用汇编器`lsca`将`stdlib.s`汇编成`stdlib.o`

对于任何用户编写的程序,如果直接调用`lsc`指令,则在链接阶段,除了链接用户的文件以外,还会自动链接前面提到的入口程序`start.o`和标准库`stdlib.o`
