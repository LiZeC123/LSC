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
#endif // COMMON_H
