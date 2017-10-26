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
    KW_IN,KW_OUT,KW_STRIN               // 其他关键字
} Symbol;

extern FILE * fin;                      //全局文件输入指针

#define ID_LEN 30                       //定义标识符最大长度
#define STR_LEN 255                     //定义字符串常量最大长度

#define checkedScan  if(-1==scan()){return -1;}

// 词法分析扫描器
char scan();
// 词法分析获得标记
int getSym();

// 检查是否是关键字
void checkKeyWord();

//获得数字
int getNumber();


#endif // COMMON_H
