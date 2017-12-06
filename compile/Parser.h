#pragma once
#include "common.h"
#include "Token.h"
#include "Lexer.h"
#include "SymTab.h"
#include <string>

class Parser
{
public:
    Parser(Lexer& lex,SymTab& tab);

    void analyse();


private:
    Token* look;
    Lexer& lexer;
    SymTab & symtab;
private:
    void move();
    bool match(Symbol need);

    void program();
    void segment();
    Symbol type();

    // 定义变量有关的函数
    // 上层函数获得信息后,作为参数传递给下面的语句,下面的函数,实际创建变量并返回
    void def(bool isExtern,Symbol s);
    void  idtail(bool isExtern,Symbol s,bool isPtr,std::string name);
    Var* defvar(bool isExtern,Symbol s,bool isPtr,std::string name);
    Var* init(bool isExtern,Symbol s,bool isPtr,std::string name);
    void deflist(bool isExtern,Symbol s);




private:
    void recover(bool cond,SynError lost,SynError wrong);
};