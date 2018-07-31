#pragma once
#include <string>
#include <vector>
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
                                    宏名表
*******************************************************************************/
class Macros
{
public:
    //TODO 从词法分析阶段替换有关的宏名
    Macros(Scanner& sc);
    bool isMacro(std::string name);
    std::vector<Token*>* getRealToken(std::string name);
    void addMacro(std::string name,std::vector<Token*>* list);
private:
    Scanner& scan;
    std::map<std::string,std::vector<Token*>*> macros;
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
    Macros macros;
    char ch;
    bool scan(char need = 0);

    Token * token;
    unsigned int macroTokenIndex;
    std::vector<Token*>* macroTokenList;

private:
    Token* getIdent();
    Token* getStr();
    Token* getChar();
    Token* getNum();
    void getMacro();
};