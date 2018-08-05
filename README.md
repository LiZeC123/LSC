Lsc(Lsc iS Compile)
==========================
[![AUR](https://img.shields.io/aur/license/yaourt.svg)](https://github.com/LiZeC123/LSC/blob/master/LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com) 
[![Build Status](https://api.travis-ci.org/LiZeC123/LSC.svg?branch=master)](https://travis-ci.org/LiZeC123/LSC)

Lsc是Lsc语言的编译系统，包含此语言编译过程中用到的编译器、汇编器和链接器，并且还以`.o`文件的形式提供了相关的函数库。此系统的目标是接收若干Lsc语言的源文件，编译、汇编成相应的`.o`文件，最后通过链接器组成一个可在Linux系统上执行的二进制文件。

Lsc语言是一种类C的语言，目前已经支持C语言的大部分特性。希望通过此项目的开发，可以学习和实践编译原理、操作系统和汇编语言的有关知识和理论，进而对整个计算机体系有一个全面、贯穿的了解。


相关文档
------------
- [支持特性概述](./docs/feature.md)
- [开发文档](./docs/index.md)


编译方法
------------

在项目根目录下执行`./makeProject install`，编译并安装程序。安装过程需要root权限。
安装完成后可在命令行下执行`lsc -v`查看程序有关信息。


参考文献
-------------
- [1] 范志东, 张琼声. 自己动手构造编译系统:编译、汇编与链接[M]. 机械工业出版社, 2016.
- [2] Stanley B. Lippman, Josée Lajoie, Barbara E. Moo. C++ Primer中文版[M]. 电子工业出版社, 2013.
- [3] RandalE.Bryant, DavidR.O'Hallaron. 深入理解计算机系统[M]. 机械工业出版社, 2011.

