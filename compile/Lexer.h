#pragma once
#include <string>
#include <map>
#include "common.h"
#include "Scanner.h"
#include "Token.h"


/*******************************************************************************
                                   关键字表
*******************************************************************************/
class KeyWords
{
public:
    KeyWords();
    Symbol getSym(std::string name);
private:
    std::map<std::string,Symbol> keywords;
};


/*******************************************************************************
                                   词法分析器
*******************************************************************************/
class Lexer
{
public:
    Lexer(Scanner& scanner);
    Token* nextToken();
    ~Lexer();

private:
    static KeyWords key;
    Scanner & scanner;
    char ch;
    bool scan(char need = 0);

    Token * token;

private:
    Token* getIdent();
    Token* getStr();
    Token* getChar();
    Token* getNum();
};