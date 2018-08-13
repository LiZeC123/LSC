编译预处理实现分析
----------------------

由于对编译预处理阶段的特性支持并不多, 且编译预处理阶段的多数操作相当于文本替换,因此可以将编译预处理的功能合并到编译器模块之中,从而避免重复的进行词法分析,语法分析等操作.

出于减少实现难度的考虑,可以假定仅支持include指令和define指令,且define指令不支持宏函数. 按照上述规则简化后,所有的指令都是文本替换,因而可以在词法分析阶段直接完成相应的替换.


### include指令

include文件分为两种,一种是系统头文件,一种是用户自定义头文件. 按照惯例,系统头文件放置在/usr/include/目录之下,lsc语言提供的头文件存放在/usr/include/lsc/目录之下,该操作通过安装脚本实现. 对于用户定义头文件,则通过分析文件的相对路径关系即可获得相应的文件.

在编译器模块之中,通过Scanner获得源文件对应的Token,因此在读取到include指令以后,只需要将当前的Scanner保存到栈内,并且使用新的文件对应的Scanner替换当前的Scanner即可. 由于直接更换了Scanner,因此对于其他模块,不需要进行任何的修改, 就好像源文件已经导入了include的文件一样. 因为使用了堆栈的结构, 因此可以递归的分析导入文件, 但目前不支持对循环导入的检测.


### define指令

词法分析阶段读取到define关键字后,提取该行定义的宏名和内容,并且将其保存在一个map之中.当后续再一次读取到有关的宏名后,直接使用相应的内容进行替换. 因此在词法分析阶段即可实现宏的替换.

通过define,可以将一个Token,替换为多个Token,只要这多个Token都位于同一行中即可,目前不支持续行符.出于简化问题的考虑,暂不考虑支持多次的宏替换,所有的宏只会被替换一次.


### 预定义宏

提供了两个预定义宏,即 __FILE__ 和 __LINE__,这两个宏分别可以获得当前的文件名以及当前的行号.