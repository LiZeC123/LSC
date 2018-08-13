动态内存分配原理与实现
=======================

lsc语言目前以标准库函数的形式提供动态内存分配和释放功能. 本文介绍如何从汇编层面实现动态此功能.


内存分配原理
---------------

在linux系统调用之中, 提供了内存映射函数, 可以使用缺页中断技术将一个文件加载到内存之中, 从而如同访问内存一般访问文件. 当指定一个不存在的文件时, 就相当于在内存中开辟了一块制定大小的内存空间. 关于此函数的基本介绍可以参考这篇博文: [内存分配、释放以及内存分配系统调用](https://blog.csdn.net/xygl2009/article/details/47006225)

使用linux系统调用中提供的取消内存映射的函数即可释放相应的空间.


实现过程
---------------------

首先,通过查阅[Linux Syscall Reference](http://syscalls.kernelgrok.com/)可以知道有关的系统调用的调用号,各寄存器的取值或含义. 结果如下所示:

No. | name          | eax  | ebx                            | ecx | edx
----|---------------|------|--------------------------------|-----|------
91  | sys_munmap    | 0x5b | unsigned long addr size_t len  | -   |  -     
192 | sys_mmap_pgoff| 0xc0 | -                              | -   |  -

由于`sys_mmap_pgoff`调用比较特殊,此网站上并未给出具体的调用方法,但查阅有关资料还是可以获得关系的信息. 例如这个网页给出了汇编代码调用`sys_mmap_pgoff`的例子:[如何在Linux下使用汇编和系统调用动态分配内存](https://code.i-harness.com/zh-CN/q/2a733a)

关于分配内存为什么是`sys_mmap_pgoff`而不是`mmap`可以查看这篇博文[linux mmap内核实现](https://blog.csdn.net/u014089131/article/details/54574964)

根据上述的提示,略加修改即可完成内存的申请和释放.


代码的接口处理
-------------

注意到,上述解除映射的系统调用需要指定内存空间的大小, 而C语言提供的free函数并不需要提供内存空间的大小. 因此在上述函数调用的基础上, 还可以对接口进行适当的调整.

首先, 在用户申请的内存大小的基础上多申请额外的8字节空间,其中保存了此块内存分配的信息以及空间的大小. 然后将内存地址+8,并将此地址返回给用户.

当调用free进行内存释放时,只需要将给定的地址-8,即可恢复真实的内存块大小并且获得内存空间大小的信息.

最后的代码如下所示
``` asm
global lscmalloc
lscmalloc:
    push ebp
    mov ebp,esp
    
    mov eax, 192        ; sys_mmap_pgoff  
    xor ebx, ebx        ; addr = NULL
    mov ecx, [ebp+8]    ; len
    add ecx, 8          ; 申请额外的空间
    mov edx, 0x7        ; prot = PROT_READ|PROT_WRITE|PROT_EXEC
    mov esi, 0x22       ; flags = MAP_PRIVATE|MAP_ANONYMOUS
    mov edi, -1         ; fd = -1
    push ebp            ; 接下来要使用ebp,因此先保存到栈
    xor ebp, ebp        ; offset = 0
    int 128             ; 系统调用申请内存,返回值保存到eax
    pop ebp             ; 

    mov ebx, 0x4d43534c ; magin number LSCM
    mov [eax],ebx
    mov [eax+4],ecx     ; 分配的内存大小
    add eax,8
   
    mov esp,ebp
    pop ebp
    ret
global lscfree
lscfree:
    push ebp
    mov ebp,esp

    mov eax, 91         ; munmap
    mov ebx, [ebp+8]    ; addr
    sub ebx,8           ; 恢复首地址位置
    mov ecx, [ebx+4]    ; 取出保存的大小
    int 128             ; 系统调用释放内存
    
    mov esp,ebp
    pop ebp
    ret
```