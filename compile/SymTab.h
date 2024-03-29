#pragma once
#include <map>
#include <vector>
#include <string>
#include "common.h"
#include "Token.h"
#include "InterInst.h"
#include "GenIR.h"


class Struct;
class Type;
class Var;
class Fun;


class Struct
{
public:
    Struct(std::vector<Var*> members);
    int getSize();
    int getOffset(std::string name);
    Var* getMember(std::string name);
    void printSelf();
private:
    void addVar(Var* member);
private:
    std::vector<Var*> members;
    int size;
};

class Type
{
public:
    // 创建一个表示基本类型的Type
    Type(Symbol s);

    // 创建一个表示结构体类型的Type
    Type(std::string structName);

    bool isBaseType();
    bool isStructType();
    

    Symbol getType();
    std::string getName();
    int getSize();

    std::string getShowName();

    // 结构体管理
    static void decStruct(std::string structName);
    static void defStruct(std::string structName, std::vector<Var*> members);
    static Struct* getStruct(std::string name);
    static int getOffset(Type* base, std::string member);
    static Var* getMember(Type* base, std::string member);
    static void printStruct();
private:
    Symbol type;
    std::string name;

    static std::map<std::string, Struct*> structTab; // 结构体表
};


// 变量  值 类型
//
// 变量： 类型， 值指针， 是否外部
// 值： 类型，数值
// 类型： 数据类型， 指针等级， 是否数组
// 表示字面量的部分肯定是最容易拆分出来的，但是重构过程可以后续在做

// 因为结构体可以嵌套， 所以不适合放入type中分析，否则无法判断当前是定义阶段还是使用阶段
// 或者就不检查有没有定义，反正指针类型不影响内存分配，等定义完成，类型也有了


class Var
{
public:
    // 初始化一个特殊的void变量
    Var();

    // 初始化一个字面值常量
    Var(Token* literal);

    // 初始化一个数组
    Var(std::vector<int> scopePath, bool isExtern, Type* s, int ptrLevel, std::string name, int len, Var* init=nullptr);
    
    // 初始化普通的变量,指针
    Var(std::vector<int> scopePath, bool isExtern, Type* s, int ptrLevel, std::string name, Var* init);

    // 临时变量
    Var(std::vector<int> scopePath, Type* s,int ptrLevel);

    // 变量拷贝
    Var(std::vector<int> scopePath, Var* val);

    // 特殊整数常量
    Var(int val);
 
    std::string getName();
    std::vector<int>& getPath();
    int getSize();
    bool getLeft();
    Type* getType();
    Var* getInitData();
    std::vector<Var*>& getInitArray();
    int getOffset();
    Var* getStep();         // 获得++操作实际需要移动的长度， 返回一个表示该长度的特殊整数变量
    Var* getPointer();      // 获得当前的指针变量对应的实际变量
    int getPtrLevel();      // 获得指针等级
    string getPtrVal();     // 获得初始值变量
    string getStrVal();     // 获得初始字符串的值
    int getVal();
    string getRawStr();
    bool isBase();          // 是否是基本类型
    bool isVoid();          // 是否是Void,特殊类型,表示空
    bool isRef();           // 是否是引用
    bool isChar();          // 是否是字符类型
    bool isCastType();      // 是否为强制类型转换标志
    bool notConst();        // 是否不是常量
    bool isArray();         // 是否是数组
    bool getIsPtr();        // 是否是指针类型
    bool isInit();
    bool hasInitArr();
 

    bool setInit();         // 生成器调用的初始化方法
    // 注意:由于每个函数并不只初始化一个属性,且部分属性相互依赖
    // 所以必须按照下面的顺序依次使用以下函数(不需要的函数不用调用)
    void baseInit();                            // 默认初始化
    void setExterned(bool isExtern);
    void setType(Type* s);
    void setPtr(int ptrLevel);
    void setName(std::string name);
    void setArray(int len);
    void setLeft(bool isLeft);
    void setPoint(Var* ptr);
    void setOffset(int offset);
    void setPtrLevel(int level);
    
    void setCast(bool isCast);
    void cast(Var* castype);

    void value();
    void printSelf();


    // 数据流分析相关接口
    void setIndex(int index);
    int getIndex();
    
               
private:
    bool literal;                   // 是否是字面常量值
    std::vector<int> scopePath;     // 作用域路径


    bool externed;                  // 是否有extern声明
    Type* type;                     // 变量类型
    std::string name;               // 变量名
    bool isCast;                    // 是否为强制类型转换标志
    int arraySize;                  // 如果变量是数组类型时的数组长度
    bool isLeft;                    // 是否是左值


    Var* initData;                  // 初始数据
    bool inited;                    // 是否初始化
    union{                          // int和char类型初始值
        int intVal;
        char charVal;
    };
    std::string strVal;             // 字符串常量初始值
    std::vector<Var*> arrVal;       // 数组初始值

    
    std::string ptrVal;             // 字符指针初始值
    Var* ptr;                       // 变量的指针变量
    int ptrLevel;                   // 指针等级,例如int**等级为2


    int offset;                     // 变量的栈帧偏移值

    // 数据流分析相关
    int index;
    bool live;

};

class Fun
{
public:
    Fun(bool isExtern, Type* type, int ptrLevel, std::string name, std::vector<Var*> para);

    void enterScope();              // 进入一个新的作用域
    void leaveScope();              // 退出作用域并计算栈帧大小

    bool match(Fun* f);                     // 比较两个函数是否匹配(函数声明和函数定义)
    bool match(std::vector<Var*>& paraVar); // 比较参数列表是否匹配
    void define(Fun* f);                    // 将函数声明转化为函数定义
    void locate(Var* var);                  // 根据变量更新栈帧偏移值

    void addInst(InterInst* inst);  // 添加一条中间代码
    
    void setReturnPoint(InterInst* inst);   // 设置函数返回点
    InterInst* getReturnPoint();            // 获得函数返回点

    void setPtrLevel(int level);
    int getPtrLevel();

    bool getExtern();
    bool hasVarArg();
    void setExtern(bool isExtern);
    Type* getType();
    std::string getName();
    int getMaxDep();
    int getParaValSize();           // 获得函数参数占用的空间大小

    void optimize(SymTab* tab);

    void toX86(FILE* file);

    void printSelf();

private:
    bool externed;                  // 是否有extern声明
    bool vararg;                    // 是否包含可变参数
    Type* type;                     // 返回类型
    int ptrLevel;                   // 指针等级
    std::string name;               // 函数名
    std::vector<Var*> paraVar;      // 参数列表

    int maxDepth;                   // 栈最大深度
    int curEsp;                     // 当前栈指针位置
    bool relocated;                 // 栈帧重定位标记

    std::vector<int> scopeEsp;      // 作用域栈指针位置
    InterCode intercode;            // 中间代码
    InterCode optCode;              // 代码优化后的中间代码
    InterInst* returnPoint;         // 函数返回点
};

class SymTab
{
public:
    SymTab();
    SymTab(SymTab&) = delete;  // 禁用拷贝
    SymTab& operator=(const SymTab&) = delete;
    // 作用域管理
    void enter();
    void leave();
    std::vector<int>& getScopePath();

    // 变量管理
    void addVar(Var* var);
    void addStr(Var*& var);
    Var* getVal(std::string name);

    // 函数管理
    void decFun(Fun* f);
    void defFun(Fun* f);
    void endDefFun();
    Fun* getFun(std::string name, std::vector<Var*>& args);

    // 输出表,调试用
    void printValTab();
    void printFunTab();
    void printStrTab();

    // 添加代码生成器
    void setGenIR(GenIR* ir);

    // 中间代码管理
    void addInst(InterInst* interInst);

    // 外部接口
    Fun* getCurrFun();

    // 执行代码优化操作
    void optimize();

    void toX86(FILE* file);

public:
    static Var* varVoid;
    static Var* zero;
    static Var* one;
    static Var* four;
    
    static Var* getVoid();  // 获得一个共用的表示Void的符号
    static Var* getTrue();  // 获得一个共用的表示True的符号

    std::vector<Var*> getGlobalVars();
    void genData(FILE* file);

private:
    std::map<std::string,std::vector<Var*>*> varTab;    // 变量表
    std::map<std::string,Var*> strTab;                  // 字符串表
    std::map<std::string,Fun*> funTab;                  // 函数表
    
    Fun* currFun;                   // 当前分析的函数
    
    int scopeId;                    // 作用域ID,依次递增
    std::vector<int> scopePath;     // 当前作用域路径

    GenIR* ir;                      // 中间代码生成器
};