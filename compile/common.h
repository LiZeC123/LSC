#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
//所有的词法记号
typedef enum {
    ERR,                                // 错误
    END,                                // 文件结束
    IDENT,                              // 标识符
    kW_INT,KW_CHAR,KW_VOID,KW_DOUBLE,   // 数据类型
    KW_EXTERN,                          //extern
    NUM,CH,STR,                         //常量
    NOT,LEA,                            // 单目运算符 ! & - *
    ADD,SUB,MUL,DIV,MOD,                //算数运算符
    INC,DEC,                            // ++ --
    GT,GE,LT,LE,EQU,NEQU,               // 关系运算符
    AND,OR,                             // 逻辑运算符
    LPAREN,RPAREN,                      // ()
    LBRACK,RBRACK,                      // []
    LBRACE,RBRACE,                      // {}
    COMMA,COLON,SEMICON,                // , : ;
    ASSIGN,                             // 赋值
    KW_IF,KW_ELSE,                      // if-else
    KW_SWITCH,KW_CASE,KW_DEFAULT,       // switch-case
    KW_WHIILE,KW_DO,KE_FOR,             // 循环语句
    KW_BREAK,KW_CONTINUE,KW_RETURN,     // 流程控制
    KW_IN,KW_OUT,KW_STRING              // 其他关键字
} Symbol;

// 所有的错误码
typedef enum{
    STR_NO_R_QUTION,            // 字符串缺少右引号        
    NUM_BIN_TYPE,               // 二进制数字缺少数据实体
    NUM_HEX_TYPE,               // 十六进制数字缺少数据实体
    CHAR_NO_R_QUTION,           // 字符缺少右引号
    CHAR_NO_DATA,               // 不支持空字符
    OR_NO_PAIR,                 // 不支持的"或"运算符
    COMMENT_NO_END,             // 多行注释缺少结束符
    TOKEN_NO_EXIST,             // 未定义的符号
    ID_TOO_LOOG,                // 标识符过长
    NUM_TOO_LONG,               // 数字常量过长
    STR_TOO_LONG,               // 字符串常量过长
} ErrCode;

extern FILE * fin;                      //全局文件输入指针

// 词法分析扫描器
char scan();
// 词法分析获得标记
int getSym();
// 处于while循环中的代码为了确保一定能退出,使用这个宏函数
// 其他情况下可以直接使用scan函数
#define checkedScan  if(-1==scan()){return -1;}


/*********************************错误处理*******************************************/

void lexError(ErrCode code);

/*********************************全局变量声明****************************************/
extern char* filename;
extern int lineNum;    // 行号
extern int colNum;     // 列号

#endif // COMMON_H
