#pragma once
#include <string>

class SymTab;
class Var;
class Fun;

class GenIR
{
public:
    GenIR(SymTab tab);


    //TODO: 两个静态函数
    static std::string genLb();
    static bool checkTypeMatch(Var* lval,Var* rval);
private:
    static int lbNum;
    SymTab& symtab;


};