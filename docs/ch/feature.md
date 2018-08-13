Lsc支持特性概述
===================

### 编译预处理的支持
指令		 | 说明
------------|------------------------------
include		|支持导入库文件和用户自定义头文件
define		|支持任意长度的文本替换,不支持宏函数

目前暂不支持其他未提及的编译预处理指令。

### 类型系统

类型		|
-----------|---------------------
基本类型	| 包括 void，int，char
数组类型	| 包括基本类型对应的一维数组，例如 int[]
指针类型	| 包括任意层数的基本类型对应的指针，例如int**

目前不支持struct和enum，该特性在后续开发中添加

<!-- 

### 基本语句

### 逻辑控制

### 算法库 

-->