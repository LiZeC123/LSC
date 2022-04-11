#pragma once
#include "common.h"
#include <string>
#include <vector>

// 直接声明,避免两个头文件相互引用导致编译错误
class Var;
class Fun;
class InterCode;
class Block;
using std::string;

/**
 * 表示一条中间指令的类
 * 使用四元式,即 op result arg1 arg2
*/
class InterInst
{

public:
    InterInst(Operator op, Var* result, Var* arg1, Var* arg2 = nullptr);    // 一般函数指令
    InterInst(Operator op, Fun* fun, Var* result = nullptr);                // 函数调用
    InterInst(Operator op,Var* arg1 = nullptr);                             // 声明,返回
    InterInst();                                                            // 构造唯一标签
    InterInst(Operator op,InterInst* tar,Var* arg1 = nullptr,Var* arg2 = nullptr); // 跳转,return

    void loadVar(string reg32,string reg8,Var* var,FILE* file);
    void leaVar(string reg32,Var* var,FILE* file);
    void storeVar(string reg32,string reg8,Var* var,FILE* file);
    void initVar(Var* var,FILE* file);
    void toX86(InterCode* inst,FILE* file);

    void setFirst();
    bool isFirstOp();
    InterInst* getTarget();
    void setBlock(Block* block);
    Block* getBlock();

    bool isJumpOp();
    bool isCondOp();



    void printSelf();
private:
    std::string label;      // 用于表示是指令还是标签
    Operator op;
    Var* result;
    Var* arg1;
    Var* arg2;
    Fun* fun;               // 如果是函数调用指令,则保存被调函数
    InterInst* target;      // 如果是跳转指令,则保存跳转的目标地址

    bool isfirstOp;         // 是否为首指令
    Block* block;           // 此代码的所处的数据流块

    void init();
};



class InterCode
{
public:
    InterCode(Fun* currFun);
    InterCode(const InterCode&) = delete;
    InterCode& operator=(const InterCode&) = delete;

    void addInst(InterInst* inst);

    void markFirstOp();

    Fun* getFun();
    const std::vector<InterInst*> getCode();
    void printSelf();

    ~InterCode();

private:
	std::vector<InterInst*>code;
    Fun* fun;
};