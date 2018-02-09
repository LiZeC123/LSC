ELF文件结构
==============

ELF格式(Executable and Linkable Format)是Linux平台主流的可执行文件格式,除了可执行文件(Executable File)以外, 可重定位目标文件(Relocatable Object File), 共享目标文件(Shared Object File), 核心转储文件(Core Dump File)也都是ELF格式

ELF文件一般结构
-----------------------


|                             |
|-----------------------------|
|文件头(ELF Header)            |
|程序头表(Program Header Table)|
|代码段(.text)                 |
|数据段(.data)                 |
|bss段(.bss)                  |
|段表字符串(.shstrtab)          |
|段表(Section Header Tbale)    |
|符号表(.symbol)               |
|字符串表(.strtab)             |
|重定位表(.rel.text)           |
|重定位表(.rel.data)           |
|                            |


在linux系统的`/esr/include/elf.h`中可以获得ELF文件的全部信息. `elf.h`文件的结构很简单,基本上就是各个模块的结构体以及各个字段合法取值的宏. 每个字段和每个宏都提供了注释,因此阅读起来页没有太多难度,另外在[维基百科](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)上提供了解释.