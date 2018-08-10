Lsc Compiler Collection
==========================
[![AUR](https://img.shields.io/aur/license/yaourt.svg)](https://github.com/LiZeC123/LSC/blob/master/LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com) 
[![GitHub issues](https://img.shields.io/github/issues/badges/shields.svg)](https://github.com/LiZeC123/LSC/issues)
[![Build Status](https://api.travis-ci.org/LiZeC123/LSC.svg?branch=master)](https://travis-ci.org/LiZeC123/LSC)

The Lsc Compiler Collection is the compiler system supporting a C-like programming language, which includes compiler, assembler, linker for lsc, as well as libraies for this language(lscio, lsclib, ...). And lsc is a recursive acronym meaning of "LSC iS Compiler". 

The lsc language is a C-like language that currently supports most of the features of the C language. It is hoped that through the development of this project, the knowledge and theory of the compilation principle, operating system and assembly language can be learned and practiced. And we can have a comprehensive and thorough understanding of the whole computer architecture.


Documents
------------
- [支持特性概述](./docs/feature.md)
- [开发文档](./docs/index.md)


Build and Run
-------------------

### Getting the source
``` bash
git clone git@github.com:LiZeC123/LSC.git
```

### Prerequisites
- Git
- Make
- GCC

This project can only be compiled on linux.

### Build

From a terminal, where you have cloned the repository, execute the following command to build and install the project:

```
./makeProject install
```
In order to copy the head files to `/usr/include/lsc/`, root permission is required.

### Automated Testing

Run the tests directly from a terminal by running `./runProjectTest` from the project folder. Those tests are also the examples of how to use Lsc Compiler Collection.



References
-------------
- [1] 范志东, 张琼声. 自己动手构造编译系统:编译、汇编与链接[M]. 机械工业出版社, 2016.
- [2] Stanley B. Lippman, Josée Lajoie, Barbara E. Moo. C++ Primer中文版[M]. 电子工业出版社, 2013.
- [3] RandalE.Bryant, DavidR.O'Hallaron. 深入理解计算机系统[M]. 机械工业出版社, 2011.

