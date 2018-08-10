汇编阶段的工具准备
======================

在开始汇编器的实现之前,我们需要准备一些辅助的工具. 在编译器阶段, 我们最终生成的是NASM格式的汇编代码,因此出于学习和调试的目的,我们有必要下载NASM的汇编器,并学习汇编器的使用

下载
---------------
可以从[此处](http://www.nasm.us/pub/nasm/releasebuilds/)获得各个版本的NASM,选择其中的最新的稳定版下载即可,例如当前的最新版是[2.13](http://www.nasm.us/pub/nasm/releasebuilds/2.13/). 

进入下载页面后,可以选择.tar.gz后缀的文件下载. 注意,一般有两个.tar.gz后缀的文件,其中一个是文档(以_xdoc.tar.gz结尾),另外一个才是真正的项目源代码. 


解压
--------------

下载好以后需要对文件进行解压,由于.tar.gz文件是linux平台下的常用格式,因此可以直接解压. 具体来说,分为图形界面解压和命令行解压两种方式

#### 图形界面解压
在文件上右击,选择提取到此处即可解压到当前目录

#### 命令行解压
``` shell
tar zxvf nasm-XXX.tar.gz
```
其中XXX替换为具体的版本号


编译和安装
------------------

进入解压后的目录,打开命令行,依次输入以下命令
``` shell
./configure
make
sudo make install
```

如果没有什么异常,经过一小会的编译过程以后,就完成了nasm的安装.

测试
---------------------
在命令行输入
```
nasm -version
```

如果出现版本信息,说明nasm安装正常

nasm汇编过程
------------------------

以下演示一个基本的nasm汇编的操作过程.

#### 1. 编写源代码
在任意目录下建立一个hello.asm文件,并输入以下内容
``` asm
section .text
global main
main:
mov eax,4 ;　　 4号调用, sys_write
mov ebx,1 ;　　 ebx送1表示输出
mov ecx,msge ;　字符串的首地址送入ecx
mov edx,14 ;　　字符串的长度送入edx
int 80h ;　　　 输出字串
mov eax,1 ;　　 1号调用, exit
int 80h ;　　　 结束　
msge:
db "Hello world!",0ah,0dh
```

#### 2. 汇编
在此目录下打开命令行,输入以下代码进行汇编
``` shell
nasm -f elf64(elf32) hello.asm 
```

注意: 
- 汇编时可以通过指定 `-f elf32` 来产生32位的汇编,或者通过 `-f elf64` 来产生64位的汇编
- 执行完毕后,应该产生hello.o文件

#### 3. 链接
最后使用gcc对代码进行链接即可生成可执行程序
如果之前指定的是`-f elf64`,则可直接调用gcc进行链接,即

``` shell
gcc hello.o -o hello
```

否则需要对gcc使用`-m32`来制定按照32位进行链接,即

``` shell
gcc -o hello hello.o -m32
```

如果没有错误,进行此步操作以后,即可生成可执行文件hello

#### 4. 执行
输入`./hello`, 此时如果命令行输出 `Hell Wordl!` 则说明程序生成正确

