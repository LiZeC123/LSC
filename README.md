Lsc(Lsc iS Compile)
==========================
[![AUR](https://img.shields.io/aur/license/yaourt.svg)](https://github.com/LiZeC123/LSC/blob/master/LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com) 
[![Build Status](https://api.travis-ci.org/LiZeC123/LSC.svg?branch=master)](https://travis-ci.org/LiZeC123/LSC)

Lsc是与其同名的Lsc语言的编译系统,此编译系统的目标是接受一个Lsc语言的源文件,编译产生可以在Linux系统上执行的二进制文件.
Lsc语言目前定义为一个在C语言的子集上扩展的语言.希望通过此项目可以学习和实践编译原理的有关知识和理论

------------------------------------------------------------------------------------------------

文档
------------
- [中文文档](./docs/index.md)


开发阶段
-------------------
1. 编译器阶段
	- ~~词法分析~~ 
	- ~~语法分析~~
	- ~~语义分析~~
	- ~~符号表管理~~
	- ~~错误处理~~
	- ~~中间代码生成~~
	- 中间代码优化(暂跳过)
2. 汇编器阶段
	- ~~词法分析~~ 
	- ~~语法分析~~
	- ~~语义分析~~
	- ~~符号表管理~~
	- 代码生成(开发中)
3. 链接器阶段


编译方法
------------
- 在根目录下执行`./makeProject`

使用方法
------------
- 编译系统程序名为`lsc`,编译器为`lscc`,汇编器为`lsca`,链接器为`lscl`
- 编译格式为`lsc a.c b.c ... `
- 编译完成后在当前目录下生成`z.out`


参考文献
-------------
[1] 范志东, 张琼声. 自己动手构造编译系统:编译、汇编与链接[M]. 机械工业出版社, 2016.
