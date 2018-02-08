语法分析
=============

汇编语言语法结构
-----------------

从宏观来看,汇编语言可以分为两个主要部分,声明和指令. 汇编语言可以看作这两个语言的无限组合. 即
```
<program> -> <segment><program>
<segment> -> <dec> | <inst>
```

对于声明,包括以下三种情况
1. 段声明,例如`section .text`
2. 全局符号声明,例如`global main`
3. 数据声明,例如`array times 256 db 0`

其中段声明总是以`section`开始,全局符号声明总是以`global`开始,而数据声明以标识符开始,因此有
```
<dec> -> section <IDENT>
<dec> -> global <IDENT>
<dec> -> <IDENT> <lbtail>
```
由于上述三者没有公因子,为了减少求First集的过程,可以将这些提升到上一级的产生式中,即
```
<program> -> section <IDENT> <program>
<program> -> global <IDENT> <program>
<program> -> <IDENT> <lbtail> <program>
<program> -> <inst> <program>
<program> -> e
```

数据定义方式
--------------

数据定义有以下几种类型
1. 纯标签,例如`main:`
2. 宏定义,例如`len equ 100`
3. 数据,例如`x dd 100`
4. 重复填充数据,例如`array times 100 db 0`

其中3和4尾部是一样的,因此可以使用`<basetail>`表示,则有
```
<lbtail> -> COLON
<lbtail> -> equ <NUM>
<lbtail> -> <basetail>
<lbtail> -> times <NUM> <basetail>
```

对于公共尾部,可以分成两个部分,一个表示内存长度的`<len>`,以及使用逗号分割的任意多个常量的`<value>`,即
```
<basetail> -> <len> <value>
<len> -> db | dw | dd
<value> -> <type> <valtail>
<valtail> -> , <type><valtail> | e
```

对于其中的常量,可以是数字,字符串和标号,且数字前可以由一个符号引导,例如`+5`或`-2`,即
```
<type> -> <NUM> | <off> <NUM> | STR | IDENT
<off> -> + | -
```

操作指令结构
-------------

对于操作指令,可以分成以下三类
1. 双操作数指令,例如`mov eax,ebx`
2. 单操作数指令,例如`call fun`
3. 无操作数指令,例如`ret`

因此对于`<inst>`可以分成如下的几种,即
```
<inst> -> <doubleop> <oprand> , <oprand>
<inst> -> <singleop> <oprand>
<inst> -> <noneop>
```

上述`<doubleop>`, `<singleop>`,`<noneop>`分别表示双操作数指令,单操作数指令和无操作数指令. 将指令分类到上述三个集合即可完成上述三个非终结符的识别.

对于`<oprand>`,汇编语言中,有以下几种类型
1. 立即数,例如`2`
2. 标号,例如`array`
3. 寄存器,例如`eax`
4. 内存,例如`[ebp-12]`

因此,对于`<oprand>`有
```
<oprand> -> <NUM> | <IDENT> | <reg> | <men>
<men> -> [ <addr> ]
```
其中,对于`<reg>`,就是各种寄存器的名字. `<addr>`表示具体的内存寻址方式.具体可以分成以下几类
1. 直接寻址,例如`[80400]`
2. 寄存器间接寻址,例如`[eax]`
3. 寄存器相对寻址,例如`[eax+12]`
4. 基址变址寻址,例如`[epb+eax]`
5. 相对基址变址寻址,例如`[ebp+eax+5]`

有关寻址的细节可以参阅我之前写的博客的[数据寻址方式](http://lizec.top/2017/12/05/%E6%B1%87%E7%BC%96%E8%AF%AD%E8%A8%80%E7%AC%94%E8%AE%B0/#data_addressing)章节

对于上述寻址方式,处理直接寻址是数字意外,其他的都是以寄存器开始,且由于我们的编译器不产生相对基址变址寻址,因此有
```
<addr> -> <NUM> | <ID> | <reg> <regaddr>
<regaddr> -> <off> <regaddrtail> | e
<regaddrtail> -> <NUM> | <reg>
```

