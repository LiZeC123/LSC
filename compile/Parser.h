#pragma once
#include "common.h"
#include "Token.h"
#include "Lexer.h"
#include "SymTab.h"
#include <string>

class Parser
{
public:
    Parser(Lexer& lex,SymTab& tab,GenIR& iir);
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;
    
    void setPrintToken();
    void analyse();

private:
    Token* look;
    Lexer& lexer;
    SymTab& symtab;
    GenIR& ir;
    bool printToken;
private:
    void move();
    bool match(Symbol need);

    void program();
    void segment();
    Type* type();

    // 定义变量有关的函数
    // 上层函数获得信息后,作为参数传递给下面的语句,下面的函数,实际创建变量并返回
    void def(bool isExtern, Type* s);
    void  idtail(bool isExtern, Type* s, int ptrLevel, std::string name);
    Var* defvar(bool isExtern, Type* s, int ptrLevel, std::string name);
    Var* init(bool isExtern, Type* s, int ptrLevel, std::string name);
    Var* initArray();
    void initArraytail(std::vector<Var*>& initVals);
    void deflist(bool isExtern, Type* s);

    // 函数参数
    void para(std::vector<Var*>& para);
    Var* paradata(Type* s);
    Var* paradatatail(Type* s,string name,int ptrLevel);
    void paralist(std::vector<Var*>& para);
    
    // 结构体
    void member(std::vector<Var*>& members);
    void membertail(std::vector<Var*>& members);

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
    void iftail();
    void elseifstat();
	void switchstat();
    void casestat();
    void caselable();

    // 表达式
    Var* altexpr();
    Var* altexprtail();
    Var* expr();
    Var* assexpr();
    Var* asstail(Var* lval);
    Var* orexpr();
    Var* ortail(Var* lval);
    Var* andexpr();
    Var* andtail(Var* lval);
    Var* comexpr();
    Var* comtail(Var* lval);
    Symbol cmps();
    Var* aloexpr();
    Var* alotail(Var* lval);
    Symbol ands();
    Var* item();
    Var* itemtail(Var* lval);
    Symbol muls();
    Var* factor();
    Symbol lop();
    Var* castype();
    Var* val();
    Symbol rop();
    Var* elem();
    Var* idexpr(std::string name, Var* base);
    Var* member(Var* base);
    void realarg(std::vector<Var*>& vec);
    Var* arg();
    void arglist(std::vector<Var*>& vec);
    Var* literal();

    // 错误恢复
    void recovery(bool cond,SynError lost,SynError wrong);
    // 检查非终结符<statement>末尾分号
    void statCheckSemicon();
};