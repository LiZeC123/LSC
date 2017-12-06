#include "SymTab.h"
#include "Error.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace std;

// 常量初始化,字符串储存在字符串表中,其他常量仅用于初始化,使用后调用者负责删除
Var::Var(Token *literal)
{
    baseInit();
    this->literal = true;
    isLeft = false;
    switch (literal->sym)
    {
    case NUM:
        setType(KW_INT);
        name = "<int>";
        intVal = ((Num*)literal)->val;
        break;
    case CH:
        setType(KW_CHAR);
        name = "<char>";
        charVal = ((Char*)literal)->ch;
        break;
    case STR:
        setType(KW_CHAR);
        //TODO: 生成唯一ID
        name = "<>";
        strVal = ((Str*)literal)->str;
        setArray(strVal.size()+1);
    default:
        // 正常情况下,调用此函数时,必然没有其他类型
        // 如果到达这里,说明存在程序逻辑错误,可以直接中止程序
        throw runtime_error("Var(Token) miss type");
    }
}

Var::Var(std::vector<int> scopePath,bool isExtern,Symbol s,bool isPtr,std::string name,int len)
{
    baseInit();
    this->scopePath = scopePath;
    setExterned(isExtern);
    setType(s);
    setPtr(isPtr);
    setName(name);
    setArray(len);
}

Var::Var(std::vector<int> scopePath,bool isExtern,Symbol s,bool isPtr,std::string name,Var* init)
{
    baseInit();
    this->scopePath = scopePath;
    setExterned(isExtern);
    setType(s);
    setPtr(isPtr);
    setName(name);
    initData = init;
}

string Var::getName()
{
    return name;
}

vector<int>& Var::getPath()
{
    return scopePath;
}

int Var::getSize()
{
    return size;
}


void Var::baseInit()
{
    literal = false;
    scopePath.push_back(-1);
    externed = false;
    isPtr = false;
    isArray = false;
    arraySize = 0;
    isLeft = true; // 变量默认是可以作为左值
    initData = nullptr;
    inited = false;
    ptr = nullptr;
    size = 0;
    offset = 0;
}

void Var::setExterned(bool isExtern)
{
    if(isExtern){
        externed = true;
        size = 0;
    }
}

void Var::setType(Symbol s)
{
    type = s;
    if(type == KW_VOID){
        // 变量类型不能为void
        Error::semError(VOID_VAR,"");
        type = KW_INT;
    }
    if(!externed && type==KW_INT){
        size = 4;
    }
    else if(!externed && type==KW_CHAR){
        size = 1;
    }
}


void Var::setPtr(bool isPtr)
{
    if(isPtr){
        this->isPtr = true;
        if(!externed){
            // 指针全部都是4字节
            size = 4;
        }
    }
}


void Var::setName(std::string name)
{
    this->name = name;
}


void Var::setArray(int len)
{
    if(len < 0){
        Error::semError(ARRAY_LEN_INVALID,name);
    }
    else{
        isArray = true;
        isLeft = false;
        arraySize = len;
        if(!externed){
            size = size*len;
        }
    }
}


string Var::toString()
{
    stringstream ss;
    ss << "路径:";
    for(unsigned int i=0;i<scopePath.size();i++){
        ss << "/" << i;
    }
    ss << "\n变量名: " << this->name << endl;
    return ss.str();
}

Fun::Fun()
{
}

void Fun::enterScope()
{
    scopeEsp.push_back(0);
}

void Fun::leaveScope()
{
    maxDepth = (curEsp > maxDepth) ? curEsp : maxDepth; // 更新最大栈帧值
    curEsp -= scopeEsp.back();                          // 恢复栈指针
    scopeEsp.pop_back();
}

SymTab::SymTab()
{
    scopeId = 0;
    currFun = nullptr;
    scopePath.push_back(0); // 初始为全局作用域
}

void SymTab::enter()
{
    scopeId++;
    scopePath.push_back(scopeId);
    // 如果当前分析的是函数,则让函数执行进入作用域操作
    if (currFun)
    {
        currFun->enterScope();
    }
}

void SymTab::leave()
{
    scopePath.pop_back();
    // 如果当前分析的是函数,则让函数执行离开作用域操作
    if (currFun)
    {
        currFun->leaveScope();
    }
}

void SymTab::addVar(Var* var)
{
    // 如果变量未定义,直接存入
    if(varTab.find(var->getName()) == varTab.end()){
        varTab[var->getName()] = new vector<Var*>();
        varTab[var->getName()]->push_back(var);
    }
    else{
        // 如果存在同名符号,检查作用域
        vector<Var*>& list = *varTab[var->getName()];
        unsigned int i;
        for(i=0;i<list.size();i++){
            if(list[i]->getPath().back() == var->getPath().back()){
                break;
            }
        }

        if(i==list.size() || var->getName()[0] == '<'){
            list.push_back(var);
        }
        else{
            // 变量重定义,只报错,不需要额外处理
            Error::semError(VAR_RE_DEF,var->getName());
            delete var;
            return;
        }
    }
}

void SymTab::addStr(Var* var)
{
    strTab[var->getName()] = var;
}

Var* SymTab::getVal(std::string name)
{
    Var* r = nullptr;
    if(varTab.find(name) != varTab.end()){
        vector<Var*>& list = *varTab[name];
        int maxLen = 0;
        int pathLen = scopePath.size();
        for(unsigned int i=0;i<list.size();i++){
            int len = list[i]->getPath().size();
            if(len<=pathLen && list[i]->getPath()[len-1] == scopePath[len-1]){
                if(len > maxLen){
                    maxLen = len;
                    r = list[i];
                }
            }
        }
    }

    if(!r){
        Error::semError(VAR_UN_DEC,name);
        // 语义错误,未定义的变量
    }
    return r;
}

vector<int>& SymTab::getScopePath()
{
    return scopePath;
}


void SymTab::printValTab()
{
    auto it = varTab.begin();
    for(;it!=varTab.end();it++){
        for(const auto&i:*(it->second)){
            cout << i->toString() << endl;
        }
        
    }
}