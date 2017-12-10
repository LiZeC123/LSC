#include "SymTab.h"
#include "Error.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace std;

// 引用Token.cpp中的符号数组,用于打印符号名
extern const char * tokenName[];
Var* SymTab::varVoid = nullptr;

Var::Var()
{
    baseInit();
    literal = false;
    type = KW_VOID;
    name = "<void>";
    isLeft = false;
    intVal = 0;
    isPtr = true;
}

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

Symbol Var::getType()
{
    return type;
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


void Var::printSelf()
{
	if(externed)printf("externed ");
	//输出type
	printf("%s",tokenName[type]);
	//输出指针
	if(isPtr)printf("*");
	//输出名字
	printf(" %s",name.c_str());
	//输出数组
	if(isArray)printf("[%d]",arraySize);
	//输出初始值
	if(inited){
		printf(" = ");
		switch(type){
			case KW_INT:printf("%d",intVal);break;
			case KW_CHAR:
				if(isPtr)printf("<%s>",ptrVal.c_str());
				else printf("%c",charVal);
				break;
            default:
                break;
		}
	}
	printf("; size=%d scope=\"",size);	
	for(unsigned int i=0;i<scopePath.size();i++){
		printf("/%d",scopePath[i]);
	}
	printf("\" ");
	if(offset>0)
		printf("addr=[ebp+%d]",offset);
	else if(offset<0)
		printf("addr=[ebp%d]",offset);
	else if(name[0]!='<')
		printf("addr=<%s>",name.c_str());
	else
		printf("value='%d'",this->intVal);
    
    printf("\n");
}

Fun::Fun(bool isExtern,Symbol type,std::string name,std::vector<Var*> para)
{
    externed = isExtern;
    this->type = type;
    this->name = name;
    this->paraVar = para;
    curEsp = 0;
    maxDepth = 0;
}

bool Fun::getExtern()
{
    return externed;
}

void Fun::setExtern(bool isExtern)
{
    externed = isExtern;
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

string Fun::getName()
{
    return name;
}

bool Fun::match(Fun* f)
{
    // Fun的函数,可以直接访问参数f的私有变量
    if(f->name != this->name){
        return false;
    }

    if(f->paraVar.size() != this->paraVar.size()){
        return false;
    }
    unsigned int len = f->paraVar.size();
    // MoreTODO: 编写函数使兼容类型匹配
    // 例如 int[] 与int*
    for(unsigned int i=0;i<len;i++){
        if(f->paraVar[i]->getType() != this->paraVar[i]->getType()){
            return false;
        }
    }


    // 最后检查返回值
    // MoreTODO: 返回值兼容问题
    if(f->type != this->type){
        // 报告语义警告,返回类型不匹配
        Error::semWarm(FUN_RET_CONFLICT,f->getName());
        return false;
    }

    return true;
}

void Fun::define(Fun* f)
{
    externed = false;
    paraVar  = f->paraVar;
}

SymTab::SymTab()
{
    varVoid = new Var();
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


vector<int>& SymTab::getScopePath()
{
    return scopePath;
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
        // 语义错误,未定义的变量
        Error::semError(VAR_UN_DEC,name);
    }
    return r;
}



void SymTab::decFun(Fun* f)
{
    f->setExtern(true);
    if(funTab.find(f->getName()) == funTab.end()){
        funTab[f->getName()] = f;
    }    
    else{
        Fun* last = funTab[f->getName()];
        if(!last->match(f)){
            Error::semError(FUN_DEC_ERR,f->getName());
        }
        // 函数已经出现并存入表中了,这个重复的可以直接delete
        delete f;
    }
}

void SymTab::defFun(Fun* f)
{
    if(f->getExtern() == true){
        Error::semError(EXTERN_FUN_DEF,f->getName());
    }

    if(funTab.find(f->getName()) == funTab.end()){
        funTab[f->getName()] = f;
    }    
    else{
        Fun* last = funTab[f->getName()];
        if(last->getExtern() == true){                      // 如果之前存入的是函数声明
            if(!last->match(f)){
                Error::semError(FUN_DEC_ERR,f->getName());
            }
            last->define(f);                                // 用函数定义覆盖以前的函数声明
        }
        delete f;
        f = last;
    }
    currFun = f;
}

void SymTab::endDefFun(Fun* f)
{
    currFun = nullptr;
}

void SymTab::printValTab()
{
    auto it = varTab.begin();
    for(;it!=varTab.end();it++){
        for(const auto&i:*(it->second)){
            i->printSelf();
        }
        
    }
}

Var* SymTab::getVoid()
{
    return SymTab::varVoid;
}