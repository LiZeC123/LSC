Lsc项目文档
=============

一种高级语言从源代码到目标代码的编译过程需要经过编译,汇编和链接三个主要阶段. 在每个阶段中,又有词法分析,语法分析多多个子阶段. 点击下面的连接阅读有关步骤的实现细节

- 编译阶段
    - [词法分析](./compile/LexicalAnalysis.md)
    - [语法分析](./compile/GrammaticalAnalysis.md)
    - [语义分析](./compile/SemanticAnalysis.md)
    - [中间代码生成](./compile/InterCodeGenerate.md)
    - 代码优化
    - 中间代码转X86汇编
- 汇编阶段
    - [工具准备](./ass/tools.md)
    - [GDB调试](./ass/gdb.md)
    - [词法分析](./ass/LexicalAnalysis.md)
- 链接阶段(待补充)
