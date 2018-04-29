Lsc(Lsc iS Compile)
==========================
[![AUR](https://img.shields.io/aur/license/yaourt.svg)](https://github.com/LiZeC123/LSC/blob/master/LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com) 
[![Build Status](https://api.travis-ci.org/LiZeC123/LSC.svg?branch=master)](https://travis-ci.org/LiZeC123/LSC)

Lsc是与其同名的Lsc语言的编译系统,此编译系统的目标是接受一个Lsc语言的源文件,编译产生可以在Linux系统上执行的二进制文件.

Lsc语言目前定义为一个在C语言的子集上扩展的语言.希望通过此项目可以学习和实践编译原理,操作系统,汇编语言的有关知识和理论.

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
	- 中间代码优化(开发中)
2. 汇编器阶段
	- ~~词法分析~~ 
	- ~~语法分析~~
	- ~~语义分析~~
	- ~~符号表管理~~
	- ~~代码生成~~
3. 链接器阶段
	- ~~ELF文件读取~~
	- ~~符号检验与重定位地址计算~~
	- ~~代码生成~~
4. 标准库
	- ~~基本IO库~~
	- 函数库(开发中)
5. 编译预处理
	- ~~词法分析~~
	- ~~指令执行~~


编译方法
------------

在根目录下执行`./makeProject`即可完成整个系统的编译, 编译完成后生成名为lsc的文件, 此文件即为Lsc语言的编译器.


参考文献
-------------
- [1] 范志东, 张琼声. 自己动手构造编译系统:编译、汇编与链接[M]. 机械工业出版社, 2016.
- [2] Stanley B. Lippman, Josée Lajoie, Barbara E. Moo. C++ Primer中文版[M]. 电子工业出版社, 2013.
- [3] RandalE.Bryant, DavidR.O'Hallaron. 深入理解计算机系统[M]. 机械工业出版社, 2011.

