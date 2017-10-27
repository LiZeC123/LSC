#ifndef COMMON_H
#define COMMON_H
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
    KW_IN,KW_OUT,KW_STRING               // 其他关键字
} Symbol;

extern FILE * fin;                      //全局文件输入指针

// 词法分析扫描器
char scan();
// 词法分析获得标记
int getSym();
// 处于while循环中的代码为了确保一定能退出,使用这个宏函数
// 其他情况下可以直接使用scan函数
#define checkedScan  if(-1==scan()){return -1;}



#endif // COMMON_H
