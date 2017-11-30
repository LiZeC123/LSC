#pragma once
#include <map>
#include <vector>
#include <string>
#include "common.h"

class Var
{
private:
    bool literal;                   // 是否是字面常量值
    std::vector<int> scopePath;     // 作用于路径
    bool externed;                  // 是否有extern声明
    Symbol type;                    // 变量类型
    std::string name;               // 变量名
    bool isPtr;                     // 是否是指针
    bool isArray;                   // 是否是数组
    int arraySize;                  // 数组长度
    bool isLeft;                    // 是否是左值
    Var* initData;                  // 初始数据
    bool inited;                    // 是否初始化
    union{                          // int和char类型初始值
        int intVal;
        char charVal;
    };
    std::string strVal;             // 字符串常量初始值
    std::string ptrVal;             // 字符指针初始值
    Var* ptr;                       // 变量的指针变量
    int size;                       // 变量大小
    int offset;                     // 变量的栈帧偏移值
};

class Fun
{
private:
    bool externed;                  // 是否有extern声明
    Symbol type;                    // 返回类型
    std::string name;               // 函数名
    std::vector<Var*> paraVar;      // 参数列表
    int maxDepth;                   // 栈最大深度
    int curEsp;                     // 当前栈指针位置
    std::vector<int> scopeEsp;      // 作用于栈指针位置
};

class SymTab
{
public:
    SymTab();
    void enter();
    void leave();



private:
    std::map<std::string,std::vector<Var*>*> varTab;
    std::map<std::string,Var*> strTab;
    std::map<std::string,Fun*> funTab;
    Fun* currFun;
    int scopeId;
    std::vector<int> scopePath;
};