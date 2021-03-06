标准库与运行时介绍
===============

本节介绍程序入口设置,以及lsc语言标准库.


程序入口点
------------

在链接阶段,会强制加入一个START符号,其值对应为`@start`,并且将最后的可执行文件的起始位置直接指向这个符号的位置. 
该符号来自lsc语言提供的start.o文件,该文件对应的汇编代码内容如下
``` assembly
section .data
section .text
global @start
@start:
    call main
    mov eax,1
    mov ebx,0
    int 128
```
start.o的主要内容就是调用main函数,并且在main函数结束后,通过系统调用结束整个程序. 

标准I/O库
---------

在任何一个语言中,标准库都是一个重要的部分. 由于lsc语言并不内置输入输出有关的语法元素, 因此关于输入和输出的函数作为标准库的一部分提供给用户.

在项目的stdlib文件夹下,提供了lscio.h和lscio.c文件,其中定义了所有的标准I/O库函数. 所有函数最终都依赖两个基本函数,即
``` c
void lscRead(char* buf,int maxLen);
void lscWrite(char* buf,int len);
```

上述两个函数由汇编语言编写,通过linux系统调用实现相应的功能. 

最终在stdlib文件夹下,提供了lscio.s文件, 此文件包含两个部分内容
1. 上述的基本函数的汇编代码
2. lscio.c文件被lsc语言的编译器lscc编译后汇编代码

因此再通过lsc语言的汇编器lsca进行汇编,即可完成IO库的编译


标准函数库
--------------

在基本的lsc语言的基础上,系统还通过`lsclib.o`提供了一系列的函数,包括exit()函数, 动态内存分配和释放, 字符串基础工具等.


默认链接过程
---------------

对于任何用户编写的程序,如果直接调用`lsc`指令,则在链接阶段,除了链接用户的文件以外,还会自动链接stdlib文件夹下所有的`.o`文件, 即入口程序`start.o`和标准库`lscio.o`,`lsclib.o`

