#pragma once
#include <map>
#include <vector>
#include <string>
#include "common.h"
#include "Token.h"

class Var
{
public:
    Var(Token* literal);

    Var(std::vector<int> scopePath,bool isExtern,Symbol s,bool isPtr,std::string name,int len);
    Var(std::vector<int> scopePath,bool isExtern,Symbol s,bool isPtr,std::string name,Var* init);
 
    std::string getName();
    std::vector<int>& getPath();
    int getSize();
    Symbol getType();
 
    void printSelf();
private:
    // 注意:由于每个函数并不只初始化一个属性,且部分属性相互依赖
    // 所以必须按照下面的顺序依次使用以下函数(不需要的函数不用调用)
    void baseInit();                            // 默认初始化
    void setExterned(bool isExtern);
    void setType(Symbol s);
    void setPtr(bool isPtr);
    void setName(std::string name);
    void setArray(int len);
               
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
public:
    Fun(bool isExtern,Symbol type,std::string name,std::vector<Var*> para);
    void enterScope();              // 进入一个新的作用域
    void leaveScope();              // 退出作用域并计算栈帧大小

    bool match(Fun* f);
    void define(Fun* f);

    bool getExtern();
    void setExtern(bool isExtern);
    std::string getName();

private:
    bool externed;                  // 是否有extern声明
    Symbol type;                    // 返回类型
    std::string name;               // 函数名
    std::vector<Var*> paraVar;      // 参数列表
    int maxDepth;                   // 栈最大深度
    int curEsp;                     // 当前栈指针位置
    std::vector<int> scopeEsp;      // 作用域栈指针位置
};

class SymTab
{
public:
    SymTab();
    // 作用域管理
    void enter();
    void leave();
    std::vector<int>& getScopePath();
    // 变量管理
    void addVar(Var* var);
    void addStr(Var* var);
    Var* getVal(std::string name);

    // 函数管理
    void decFun(Fun* f);
    void defFun(Fun* f);
    void endDefFun(Fun* f);

    // 输出变量表,调试用
    void printValTab();
private:
    std::map<std::string,std::vector<Var*>*> varTab;
    std::map<std::string,Var*> strTab;
    std::map<std::string,Fun*> funTab;
    Fun* currFun;
    int scopeId;
    std::vector<int> scopePath;
};