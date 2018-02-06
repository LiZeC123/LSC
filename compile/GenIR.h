#pragma once
#include <string>
#include <vector>
#include "common.h"

class SymTab;
class Var;
class Fun;
class InterInst;
class GenIR
{
public:
    GenIR(SymTab& tab);

    void genFunHead(Fun* fun);
    void genFunTail(Fun* fun);
    void genReturn(Var* result);

    bool genVarInit(Var* var);


    Var* genTwoOp(Var* lval,Symbol op,Var* rval);
    Var* genOneLeftOp(Symbol op,Var* val);
    Var* genOneRightOp(Var* val,Symbol op);
    Var* genArray(Var* array,Var* index);
    Var* genCall(Fun* fun, std::vector<Var*>& args);

    void genIfHead(Var* cond,InterInst*& _else);
    void genIfTail(InterInst*& _else);
    void genElseHead(InterInst* _else,InterInst*& _exit);
    void genElseTail(InterInst*& _exit);
    void genWhileHead(InterInst*&_while,InterInst*& _exit);
    void genWhileCond(Var* cond,InterInst* _exit);
    void genWhileTail(InterInst*& _while,InterInst*& _exit);

    void genBreak();
    void genContinue();
    
    static std::string genLb(bool isStr = false);                         //生成唯一的字符串表示
    static bool checkTypeMatch(Var* lval,Var* rval);    // 检查两个变量类型是否匹配


private:
    bool typeCheck(Var* lval,Var* rval);

    Var* genLea(Var* val);          // 生成地址,例如&a
    Var* genPtr(Var* val);          // 生成指针,相当于执行 tmp = *p
    Var* genIncL(Var* val);         // 前缀的++
    Var* genDecL(Var* val);         // 前缀的--
    Var* genIncR(Var* val);         // 后缀的++
    Var* genDecR(Var* val);         // 后缀的--
    Var* genNot(Var* val); 
    Var* genMinus(Var* val); 

    Var* genAssign(Var* val);
    Var* genAssign(Var* lval,Var* rval);

    Var* genOr(Var* lval,Var* rval);
    Var* genAnd(Var* lval,Var* rval);
    Var* genEqu(Var* lval,Var* rval);
    Var* genNequ(Var* lval,Var* rval);
    Var* genAdd(Var* lval,Var* rval);
    Var* genSub(Var* lval,Var* rval);

    Var* genGt(Var* lval,Var* rval);
    Var* genGe(Var* lval,Var* rval);
    Var* genLt(Var* lval,Var* rval);
    Var* genLe(Var* lval,Var* rval);
    Var* genMul(Var* lval,Var* rval);
    Var* genDiv(Var* lval,Var* rval);
    Var* genMod(Var* lval,Var* rval);

    void genPara(Var* arg);         // 生成函数参数入栈代码

    void push(InterInst* head,InterInst* tail);
    void pop();



private:
    static int lbNum;
    SymTab& symtab;

    std::vector<InterInst*> headlist;
    std::vector<InterInst*> taillist;

};