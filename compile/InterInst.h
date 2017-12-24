#pragma once
#include "common.h"
#include <string>
#include <vector>

// 直接声明,避免两个头文件相互引用导致编译错误
class Var;
class Fun;


/**
 * 表示一条中间指令的类
 * 使用四元式,即 op result arg1 arg2
*/
class InterInst
{

public:
    InterInst(Operator op, Var* result, Var* arg1, Var* arg2 = nullptr);    // 一般函数指令
    InterInst(Operator op, Fun* fun, Var* result = nullptr);                // 函数调用
    InterInst();                                                            // 构造唯一标签
    InterInst(Operator op,InterInst* tar,Var* arg1 = nullptr,Var* arg2 = nullptr); // 跳转,return
    void printSelf();
private:
    std::string label;      // 用于表示是指令还是标签
    Operator op;
    Var* result;
    Var* arg1;
    Var* arg2;
    Fun* fun;               // 如果是函数调用指令,则保存被调函数
    InterInst* target;      // 如果是跳转指令,则保存跳转的目标地址

    void init();
};



class InterCode
{

public:
    void addInst(InterInst* inst);

    void printSelf();

    std::vector<InterInst*>& getCode();

    ~InterCode();


private:
	std::vector<InterInst*>code;
};