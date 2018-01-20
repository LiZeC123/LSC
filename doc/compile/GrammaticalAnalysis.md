
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
<type>    -> int | void | char 
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
<deflist> -> ,<def>
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

<deflist> -> ,<def>
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
<localdef> -> <type><def>
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
<comtail> -> <cmps><aloexpr><comtail>
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
<rop>    -> ++ | -- | e              // 后置的++和--
```

最后是元素表达式
```
<elem> -> <ID>
		| <ID> [ <expr> ]
		| <ID> ( <realarg> )
		| ( <expr> )
		| <literal>
```
由于上述产生式中由于前三项有左因子,因此可以对其进行提取,提取后有如下表达式
```
<elem>   -> <ID><idexpr>
<elem>   -> ( <expr> )
<elem>   -> <literal>
<idexpr> -> [ <expr> ] | ( <realarg> ) | e

// 常量
<literal> -> <NUM> | <CH> | <STR> 

// 函数实参
<realarg> -> <arg><arglist>
<arg>     -> <expr>
<arglist> -> , <arg><arglist>
<arglist> -> e
```
以上便是全部的表达式的产生式,由于没有比赋值运算等级更低的表达式,所以最后可以得到
```
<expr> -> <assexpr>
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
<dowhilestat>   -> do <block> while ( <altexpr> ) ;
<forsata>       -> for(<forinit><altexpr>;<altexpr>) <block>
<forinit>       -> <localdef> | <altexpr> ;  // for的初始语句可以是局部变量定义,或者一个表达式

<ifstat>     -> if ( <expr> ) <block> <elsestat>
<elsestat>   -> else <block> | e

<switchstat> -> switch ( <expr> ) { <casestat> }
<casestat>   -> case <caselable> : <subprogram><casetate>
              | default: <subprigram>
<caselable> -> <literal>
```
以上Lsc语言全部的产生式,根据以上的表达式结合递归下降方法即可完成对Lsc语言的语法分析,对于此文法的SELECT集计算就作为作业留给同学们练习啦~

### 错误恢复
在上述递归下降过程中,如果在某个产生式中,如果存在某个符号缺失,则会导致该处匹配失败. 而且, 即使后续符号是正确的顺序,也会因为当前符号的缺少,而导致后续所有符号的错误.

对于上述这种情况,可以有一个简便的处理方法,以产生式`A->BCDE`为例,如果在B处符号缺失,则可以先尝试读取一个后续符号,并判断是否在B后续符号的的First集中,即First(CDE)中. 如果后续符号在First(CDE)中,则说明此处可能是符号B缺失,否在此处的语句存在语法错误.

上述方案中,读取一个符号后,也可能不能正确的恢复. 但是考虑到实际编程过程中,不容易出现大幅度的语法错误,而更容易出现部分符号的遗漏,因此上述方法应该能较好的处理大部分情况. 无论上述错误恢复方法能否正确的恢复,但第一条语法错误总是可以保证正确性.

