#pragma once
#include "common.h"
#include "Token.h"
#include "Lexer.h"
#include "SymTab.h"
#include <string>
using std::string;

class Parser
{
public:
    Parser(Lexer& lex,SymTab& tab);

    void analyse();

private:
    Token* look;
    Lexer& lexer;
    SymTab& symtab;
private:
    void move();
    bool match(Symbol need);

    void program();
    void segment();
    void lbtail(string name);
    void basetail(string name,int times);
    int len();
    void value(string name,int times,int size);
    void type(std::vector<int>& cont,int size);
    void valtail(std::vector<int>& cont,int size);
    void inst();
};