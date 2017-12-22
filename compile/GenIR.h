#pragma once
#include <string>
#include "common.h"

class SymTab;
class Var;
class Fun;

class GenIR
{
public:
    GenIR(SymTab& tab);

    void genFunHead(Fun* fun);
    void genFunTail(Fun* fun);
    void genReturn(Var* result);

    Var* genTwoOp(Var* lval,Symbol op,Var* rval);

    
    static std::string genLb();                         //生成唯一的字符串表示
    static bool checkTypeMatch(Var* lval,Var* rval);    // 检查两个变量类型是否匹配
private:
    static int lbNum;
    SymTab& symtab;


};