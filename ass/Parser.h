#pragma once
#include "common.h"
#include "Token.h"
#include "Lexer.h"
#include "SymTab.h"
#include <string>
using std::string;

class Generator;

class Parser
{
public:
    Parser(Lexer& lex,SymTab& tab);
    void analyse();

    void setGen(Generator* g);
private:
    Token* look;
    Lexer& lexer;
    SymTab& symtab;
    Generator* gen;    
private:
    void move();
    bool match(Symbol need);

    void program();
    void segment();
    void lbtail(string name);
    void basetail(string name,int times);
    int len();
    void value(string name,int times,int size);
    void type(string name,std::vector<int>& cont);
    void valtail(string name,std::vector<int>& cont,int size);
    void inst();
    void oprand(OpType& type,int& len,int& regNum);
    void men();
    int reg(int& len);
    void addr();
    void regaddr(int regCode,int len);
    bool off();
    void regaddrtail(int regCode,bool isSub);

    void tempSave(Label* label);    // 临时保存标签,以便于后续可能的重定位处理
};