#ifndef COMMON_H
#define COMMON_H
enum Symbol{
    ERR,                                // 错误
    END,                                // 文件结束
    ID,                                 // 标识符
    kW_INT,KW_CHAR,KW_VOID,KW_DOUBLE,   // 数据类型
    KE_EXTERN,                          //extern
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
    KW_BREAK,KW_CONTINUE,KW_RETURN      // 流程控制
};

#define ID_LEN 30                       //定义标识符最大长度
#define NUM_LEN                         //定义整数数字最大长度
#define STR_LEN 255                     //定义字符串常量最大长度

// 词法分析扫描器
char scan(FILE* file);
// 词法分析获得标记
int getSym();



#endif // COMMON_H
