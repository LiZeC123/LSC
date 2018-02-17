#pragma once
#include "common.h"
#include "Token.h"
#include "Lexer.h"
#include "SymTab.h"
#include "OpInfo.h"
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

    Inst instr;
    ModRM modrm;
    SIB sib;
    
private:
    void move();
    bool match(Symbol need);

    void program();
    void segment();
    void lbtail(string name);
    void basetail(string name,int times);
    int len();
    void value(string name,int times,int size);
    void type(string name,std::vector<int>& cont,int size);
    void valtail(string name,std::vector<int>& cont,int size);
    void inst();
    void oprand(OpType& type,int& len,bool isDstReg);
    void men();
    int reg(int& len);
    void addr();
    void regaddr();
    void regaddrtail();

    void tempSave(Label* label);    // 临时保存标签,以便于后续可能的重定位处理
};