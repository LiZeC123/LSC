#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
//所有的词法记号
typedef enum {
    ERR,                            //错误，异常
    END,                            //文件结束标记
    IDENT,                          //标识符
    KW_INT,KW_CHAR,KW_VOID,         //数据类型
    KW_EXTERN,                      //extern
    NUM,CH,STR,                     //字面量
    NOT,LEA,                        //单目运算 ! - & *
    ADD,SUB,MUL,DIV,MOD,            //算术运算符
    INC,DEC,                        //++ --
    GT,GE,LT,LE,EQU,NEQU,           //比较运算符
    AND,OR,                         //逻辑运算
    LPAREN,RPAREN,                  //()
    LBRACK,RBRACK,                  //[]
    LBRACE,RBRACE,                  //{}
    COMMA,COLON,SEMICON,            //逗号,冒号,分号
    ASSIGN,                         //赋值
    KW_IF,KW_ELSE,                  //if-else
    KW_SWITCH,KW_CASE,KW_DEFAULT,   //swicth-case-deault
    KW_WHILE,KW_DO,KW_FOR,          //循环
    KW_BREAK,KW_CONTINUE,KW_RETURN  //break,continue,return
} Symbol;

// 所有的词法错误码
enum LexErr
{
    STR_NO_R_QUTION,            // 字符串缺少右引号        
    NUM_BIN_TYPE,               // 二进制数字缺少数据实体
    NUM_HEX_TYPE,               // 十六进制数字缺少数据实体
    CHAR_NO_R_QUTION,           // 字符缺少右引号
    CHAR_NO_DATA,               // 不支持空字符
    OR_NO_PAIR,                 // 不支持的"或"运算符
    COMMENT_NO_END,             // 多行注释缺少结束符
    TOKEN_NO_EXIST,             // 未定义的符号
};

// 所有的词法警告码
enum LexWarn
{
    FILE_NOT_FINISH,            // 文件未扫描结束
};

// 所有的语法错误码
enum SynError
{
    TYPE_LOST,                  // 类型
    TYPE_WRONG,                    
    ID_LOST,                    // 标志符
    ID_WRONG,                        
    NUM_LOST,                   // 数组长度
    NUM_WRONG,
    LITERAL_LOST,               // 常量
    LITERAL_WRONG,
    COMMA_LOST,                 // 逗号
    COMMA_WRONG,
    SEMICON_LOST,               // 分号
    SEMICON_WRONG,
    ASSIGN_LOST,                // =
    ASSIGN_WRONG,
    COLON_LOST,                 // 冒号
    COLON_WRONG,
    WHILE_LOST,                 // while
    WHILE_WRONG,
    LPAREN_LOST,                // (
    LPAREN_WRONG,
    RPAREN_LOST,                // )
    RPAREN_WRONG,
    LBRACK_LOST,                // [
    LBRACK_WRONG,
    RBRACK_LOST,                // ]
    RBRACK_WRONG,
    LBRACE_LOST,                // {
    LBRACE_WRONG,
    RBRACE_LOST,                // }
    RBRACE_WRONG
};

// 所有的语义警告
enum SemWarn
{
	FUN_DEC_CONFLICT,            //函数参数列表类型冲突
	FUN_RET_CONFLICT             //函数返回值类型冲突
};


// 所有的语义错误
enum SemError
{
    VAR_RE_DEF,                    // 变量重定义
    FUN_RE_DEF,                    // 函数重定义
    VAR_UN_DEC,                    // 变量未声明
    FUN_UN_DEC,                    // 函数未声明
    FUN_DEC_ERR,                   // 函数声明与定义不匹配
    FUN_CALL_ERR,                  // 行参实参不匹配
    DEC_INIT_DENY,                 // 声明不允许初始化
    EXTERN_FUN_DEF,                // 函数声明不能使用extern
    ARRAY_LEN_INVALID,             // 数组长度无效
    VAR_INIT_ERR,                  // 变量初始化类型错误
    GLB_INIT_ERR,                  // 全局变量初始化值不是常量
    VOID_VAR,                      // void变量
    EXPR_NOT_LEFT_VAL,             // 无效的左值表达式
    ASSIGN_TYPE_ERR,               // 赋值类型不匹配
    EXPR_IS_BASE,                  // 表达式不能是基本类型
    EXPR_NOT_BASE,                 // 表达式不是基本类型
    ARR_TYPE_ERR,                  // 数组运算类型错误
    EXPR_IS_VOID,                  // 表达式不能是VOID类型
    BREAK_ERR,                     // break不在循环或switch-case中
    CONTINUE_ERR,                  // continue不在循环中
    RETURN_ERR                     // return语句和函数返回值类型不匹配
};

#endif // COMMON_H
