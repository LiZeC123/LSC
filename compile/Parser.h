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

    // 函数
    void para(std::vector<Var*>& para);
    Var* paradata(Symbol s);
    Var* paradatatail(Symbol s,std::string name);
    void paralist(std::vector<Var*>& para);
    void funtail(Fun* fun);
    void block();
    void subprogram();
    void localdef();

    // 语句
    void statement();
    void whilestat();
	void dowhilestat();
	void forstat();
	void forinit();
	void ifstat();
	void elsestat();
	void switchstat();

    void altexpr();





private:
    // 错误恢复
    void recovery(bool cond,SynError lost,SynError wrong);
    // 检查非终结符<statement>末尾分号
    void statCheckSemicon();
};