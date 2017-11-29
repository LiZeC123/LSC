Lsc(Lsc iS Compile)
==========================

Lsc是与其同名的Lsc语言的编译系统,此编译系统的目标是接受一个Lsc语言的源文件,编译产生可以在Linux系统上执行的二进制文件.
Lsc语言目前定义为一个在C语言的子集上扩展的语言.希望通过此项目可以学习和实践编译原理的有关知识和理论

------------------------------------------------------------------------------------------------

词法分析
--------------

#### 词法记号的分类
按照类型含义可以分成如下的几种
1. 类型系统
	- `int` `char` `void` `double`
	- 一维指针,一维数组 包括 `*` `&` `[` 和 `]`
2. 常量
	- 二/八/十进制整数
	- 字符常量
	- 字符串常量
3. 表达式
	- 算数运算符 `+` `-` `*` `/` `%` `++` `--`
	- 关系运算符 `>` `<` `==` `>=` `<=` `!=`
	- 逻辑运算符 `&&` `||` `!`
4. 语句
	- 赋值语句
	- 循环语句 for,while, do-while
	- 条件语句 if-else switch-case
	- 函数调用
	- 控制语句 return  break continue
5. 定义和声明
	- extern语句
	- 函数声明
	- 函数定义
	- 变量声明
6. 其他
	- 单行注释
	- 多行注释

具体到编写的程序中,可以分成如下的几类
1. 关键字/标识符
	- 这两个都是按照某种规则定义的字符串,其中某些特殊的字符串会视为关键字
2. 数字
	- 包括常见的10进制,16进制,8进制和2进制数字
	- 后续还会添加浮点数的支持
3. 字符常量
	- 通常的字符常量处理较为简单,主要是对转移字符的判别
4. 字符串常量
	- 字符串常量也包括转移字符的处理
5. 注释
	- 包括单行注释和多行注释
	- 对于多行注释的识别略微需要一些技巧,但本质上还是有限自动机
6. 空白符号
	- 对于去除所有的空白字符,也可以设计一个一个对应的有限自动机
7. 其他简单符号
	- 包括各种的界符和运算符,基本上都是一个或者两个符号,只需要简单匹配即可完成识别

 
#### 正则表达式
词法分析可以使用有限自动机完成,而有限自动机又对应正则语言,因此只要得到各个成分的正则表达式,就可以完成词法分析
1. 标识符 `[_a-zA-Z][_a-zA-Z0-9]*`
2. 常量
	1. 十进制整数 `[1-9][0-9]*`
	2. 八进制整数 `0[1-7]*`
	3. 二进制整数 `0b[0-1]+`
	4. 十六进制整数 `0x[0-9a-fA-F]+`
	5. 浮点数 `[1-9][0-9]*[.][0-9]+`
3. 界符
	- 界符有两类,单符号界符和双符号界符,注意区分公共前缀即可,例如对于`>`和`>=`,读入`>`需要确定之后的符号不是`=`才能结束
4. 注释
	- 单行注释
	- 多行注释 `/\*.*\*/` 
	- 注意:多行注释对应一个NFA,如何将其转化为等价的DFA作为该知识点的课后作业留给同学们练习~

注意:单独的0视为8进制数,而单独的0b或者0x不视为一个合法的符号

#### 词法分析部分的错误处理
主要的错误包括如下的几类
1. 需要成对出现的符号尾成对出现
	- 包括字符串,字符常量的符号
2. 数字识别中出现错误
	- 例如不合法的二进制或16进制数字
3. 长度超过限制
	- 包括标识符,数字常量,字符串常量

对于这些错误,目前处理方案是仅仅报告错误的位置,然后继续进行词法分析

--------------------------------------------------------------------------------------------

语法分析
-------------

#### Lsc语言语法结构
Lsc语言是可以认为是一个C语言的子集上的扩展语言,基本结构与C语言是一致的,一个完整的程序是由若干程序片段组成的,因此有如下结构(e表示空)
```
<program> -> <segment><program>
<program> -> e
```

对于程序片段,可以分为变量定义,变量声明和函数定义,函数声明,其中变量声明还可以使用extern关键字引导,例如
```
int a;
extern int b;
int f();
int g(){ }
```

因此可以有如下的产生式
```
<segment> -> extern <type><def>
<segment> -> <type><def>
<type>    -> int | void | char | double
```

#### 变量定义与声明的识别
对于`<def>` 部分,首先分析变量的定义和声明,在上面的产生式识别了类型声明以后,还可以分成两类
1. 以标识符开始的普通变量
2. 以`*`开始的指针

上述两类有可以声明对应的数组类型,因此有如下的产生式
```
<def> -> <ID><defvar>
<def> -> * <ID><defvar>

<defvar> -> [ <NUM> ]
<defvar> -> <init>

<init> -> = <expr>
<init> -> e
```
在上面的产生式中,`<ID>`表示一个在词法分析中定义的标识符,`<NUM>`表示一个在词法分析中定义的数字

为了便于处理,不支持对数组的初始化.因此如果不是定义为数组,则可以加上一个初始化的部分呢,初始化部分可以是一个由等号开始的表达式或者为空.

由于一次可以声明多个变量,这些变量之间,使用逗号分割,因此可以有如下产生式
```
<deflist> -> ,<defvar><deflist>
<deflist> -> ;
```
#### 函数定义与声明的识别
对于`<def>` 的函数部分,可以直接的划分成以下的两种
```
<fun> -> <ID>( <para> );
<fun> -> <ID>( <para> ) <block>
```
由于上述表达式中,存在左公因子,从而不符合LL(1)文法,可以将其中的左因子进行提取,即
```
<fun>     -> <ID>( <para> ) <funtail>
<funtail> -> ;
<funtail> -> <block>
```

#### 变量定义和函数定义
对比变量定义和函数定义后,可以发现,虽然函数定义里面没有左公因子了,但函数定义和变量定义之间又存在了公共因子

同样可以提取左公因子,提取后完整的`<def>`定义如下所示
```
<def>     -> <ID> <idtail>
<def>     -> * <ID> <idtail>

<idtail>  -> <defvar><deflist>
<idtail>  -> ( <para> ) <funtail>

<defvar> -> [ <NUM> ]
<defvar> -> <init>

<init>    -> = <expr>
<init>    -> e

<deflist> -> ,<defvar><deflist>
<deflist> -> ;

<funtail> -> ;
<funtail> -> <block>
```
#### 函数参数列表
在上述的产生式中,使用`<para>`表示函数的参数列表,参数列表和变量定义大致相同,因此直接给出产生式
```
<para> -> <type><paradata><paralist>
<para> -> e

<paradata> -> * <ID>
<paradata> -> <ID> <paradatatail>

<paradatatail> -> [ <num> ]
<paradatatail> -> e

<paralist> -> ,<paradata><paralist>
<paralist> -> e
```

#### 函数体
函数体是由花括号包含的局部变量定义和语句的组合,所以有如下产生式
```
<block>      -> { <subprogram> }
<subprogram> -> <localdef><subprogram>
<subprogram> -> <statement><subprogram>
<subprogram> -> e
```
其中`<localdef>`的定义与全局变量的定义没有区别,产生式如下
```
<localdef> -> <type><defvar><deflist>
```

#### 表达式
语句的定义比较复杂,且各种表达式中涉及运算符优先级的问题,构造保持运算符优先级特训的文法的核心思想是:**将高优先级运算符形成的表达式整体作为低优先级形成的表达式的操作数**

此外,各种运算符之间还有结合性的区别,对于这一点,在后续的语义分析中再进行讨论
以下给出本语言中所有运算符的优先级关系

运算符                       | 含义                              | 优先级 | 结合性
:---------------------------|:---------------------------------|------|-----
`=`                         | 赋值                               | 10   |**右**
`丨丨`                       | 逻辑或                             |  9   | 左
`&&`                        | 逻辑与                             |  8   | 左
`>`,`<`,`>=`,`<=`,`==`,`!=` | 大于,小于,大于等于,小于等于,等于,不等于 |  7   | 左
`+`,`-`                     | 加,减                              |  6   | 左
`*`,`/`,`%`                 | 乘,除,取余                          |  5   | 左
`!`,`-`,`&`,`*`,`++`,`--`   | 逻辑非,取负,取地址,指针,前置++,前置--  |  4   |**右**
`++`,`--`                   | 后置++,后置--                       |  3   |**右**
`()`                        | 括号                               |  2   | 左
`[]`,`()`                   | 数组索引,函数调用                    |  1   | 左

由于Lsc语言目前是C系列的语言,因此上述的运算符的优先级和结合性与C语言的定义基本一致

下面给出语句的产生式
```
// 赋值表达式
<assexp> -> <orexpr><asstail>
<asstail> -> = <orexpr><asstail>
<asstail> -> e

//逻辑或表达式
<orexpr> -> <andexpr><ortail>
<ortail> -> || <andexpr><ortail>
<ortail> -> e

//逻辑与表达式
<andexpr> -> <comexpr><andtail>
<andtail> -> && <comexpr><andtail>
<andtail> -> e

//关系表达式
<comexpt> -> <aloexpr><comtail>
<comtail> -> <cmps><aloexpr><coptail>
<comtail> -> e
<cmps>    -> >= | <= | > | < | == | !=

// 算数表达式
<aloexpr> -> <item><alotail>
<alotail> -> <adds><item><alotail>
<alotail> -> e
<ands>    -> + | -

//项表达式
<item>     -> <factor><itemtail>
<itemtail> -> <muls><factor><itemtail>
<itemtail> -> e
<muls> -> * | / | %

//因子表达式
<factor> -> <lop><factor>
<factor> -> <val>
<lop>    -> ! | - | & | * | ++ | --
<val>    -> <elem><rop>       
<rop>    -> ++ | --                // 后置的++和--
```

最后是元素表达式
```
<elem> -> <ID>
		| <ID> [ <expr> ]
		| <ID> ( <realarg> )
		| ( <exp> )
		| <literal>
```
由于上述产生式中由于前三项有左因子,因此可以对其进行提取,提取后有如下表达式
```
<elem> -> <ID><idexpr>
<elem> -> ( <exp> )
<elem> -> <literal>
<idexpr> [ <expr> ] | ( <realarg> )

// 常量
<literal> -> <NUM> | <CH> | <STR> 

// 函数实参
<realarg> -> <arg><arglist>
<arg>     -> <exp>
<arglist> -> , <arg><arglist>
<arglist> -> e
```
以上便是全部的表达式的产生式,由于没有比赋值运算等级更低的表达式,所以最后可以得到
```
<exp> -> <assexpr>
```
考虑到,部分语句中可以使用空语句(例如for语句的条件部分),因此额外的定义
```
<altexpr> -> <expr>
<altexpr> -> e
```
关于以上的产生式还有以下几点补充
1. 在`<assexpr>`语句中,实际上逻辑或表达式是不能作为左值出现的,但是在当前阶段并不检查这种问题,留到语义分析的时候在进一步检查

#### 语句
总体来看,语句包含表达式,while,for,dowhile构成的循环语句,if,switch构成的分支语句,break,continue,return构成的流程控制语句,所以语句的产生式如下
```
<statement> -> <altexpr>;
             | <whilestat> | <forstat> | <dowhilestat>
			 | <ifstat>    | <switchstat>
			 | break;
			 | continue;
			 | return <altexpr>;

```

下面依次给出各种语句的产生式
```
<whilestat>     -> while ( <altexpr> ) <block>
<dowhilestat>   -> do <block> while ( <altexpr> )
<forsata>       -> for(<forinit><altexpr>;<altexpr>) <block>
<forinit>       -> <localdef> | <altexpr> ;  // for的初始语句可以是局部变量定义,或者一个表达式

<ifstat> -> if ( <expr> ) <block> <elsestat>
<elsestat> -> else <block> | e

<switchstat> -> switch ( <expr> ) { <casestat> }
<casestat>   -> case <caselable> : <subprogram><casetate>
              | default: <subprigram>
<caselable> -> <literal>
```
以上Lsc语言全部的产生式,根据以上的表达式结合递归下降方法即可完成对Lsc语言的语法分析,对于此文法的SELECT集计算就作为作业留给同学们作为练习啦~


