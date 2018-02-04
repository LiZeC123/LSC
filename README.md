Lsc(Lsc iS Compile)
==========================
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/LiZeC123/LSC/blob/master/LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com) 
[![Build Status](https://api.travis-ci.org/LiZeC123/LSC.svg?branch=master)](https://travis-ci.org/LiZeC123/LSC)

Lsc是与其同名的Lsc语言的编译系统,此编译系统的目标是接受一个Lsc语言的源文件,编译产生可以在Linux系统上执行的二进制文件.
Lsc语言目前定义为一个在C语言的子集上扩展的语言.希望通过此项目可以学习和实践编译原理的有关知识和理论

------------------------------------------------------------------------------------------------

文档
------------
- [中文文档](./docs/index.md)

------------------------------------------------------------------------------------------------

开发阶段
-------------------
1. 编译器阶段
	- ~~词法分析~~ 
	- ~~语法分析~~
	- ~~语义分析~~
	- 符号表管理(开发中)
	- 错误处理(开发中)
	- 中间代码生成(开发中)
	- 中间代码优化
2. 汇编器阶段
3. 链接器阶段


---------------------------------------------------------------------------------------------

#### 编译方法
- 在根目录下执行`./makeProject`