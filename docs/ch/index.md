Lsc项目开发文档
=============

一种高级语言从源代码到目标代码的编译过程需要经过编译,汇编和链接三个主要阶段. 在每个阶段中,又有词法分析,语法分析多个子阶段. 点击下面的连接阅读有关步骤的实现细节

- 编译预处理阶段
    - [实现分析](./preproc/Struct.md)
- 编译阶段
    - [词法分析](./compile/LexicalAnalysis.md)
    - [语法分析](./compile/GrammaticalAnalysis.md)
    - [语义分析](./compile/SemanticAnalysis.md)
    - [中间代码生成](./compile/InterCodeGenerate.md)
    - [中间代码转X86汇编](./compile/ToX86.md)
    - 代码优化
- 汇编阶段
    - [工具准备](./ass/tools.md)
    - [GDB调试](./ass/gdb.md)
    - [X86指令结构](./ass/X86.md)
    - [ELF文件结构](./ass/ELF.md)
    - [词法分析](./ass/LexicalAnalysis.md)
    - [语法分析](./ass/GrammaticalAnalysis.md)
    - [代码生成与符号管理](./ass/Symbol.md)
- 链接阶段
	- [ELF文件读取](./lit/ReadElf.md)
	- [符号检验与重定位地址计算](./lit/Sym.md)
- 标准库
    - [Lsc语言标准库](./stdlib/stdlib.md)
    - [动态内存分配](./stdlib/alloc.md)
- 运行环境
    - [在Win10子系统下运行程序](./win10.md)