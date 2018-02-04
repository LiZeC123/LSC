#include "SymTab.h"
#include "Error.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace std;


// 引用Token.cpp中的符号数组,用于打印符号名
extern const char * tokenName[];

Var* SymTab::varVoid = nullptr;
Var* SymTab::one = nullptr;
Var* SymTab::four = nullptr;

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
        name = GenIR::genLb();
        strVal = ((Str*)literal)->str;
        setArray(strVal.size()+1);
        break;
    default:
        // 正常情况下,调用此函数时,必然没有其他类型
        // 如果到达这里,说明存在程序逻辑错误,可以直接中止程序
        throw runtime_error("Var(Token) miss type,this type is"+literal->toString());
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

Var::Var(std::vector<int> scopePath,Symbol s,bool isPtr)
{
    baseInit();
    this->scopePath = scopePath;
    setType(s);
    setPtr(isPtr);
    setName(GenIR::genLb());
    setLeft(false);         // 临时变量默认是右值
}

Var::Var(std::vector<int> scopePath, Var* val)  
{
    baseInit();
    this->scopePath = scopePath;
    setType(val->getType());
    setPtr(val->isArray || val->isPtr);
    setName(GenIR::genLb());
    setLeft(false);
}

Var::Var(int val)
{
	baseInit();
	setName("<int>");//特殊变量名字
	literal=true;
	setLeft(false);
	setType(KW_INT);
	intVal=val;//记录数字数值
}

bool Var::setInit()
{
    if(!initData){
        return false;
    }
    inited = false;

    if(externed){
        Error::semError(DEC_INIT_DENY,name);
    }
    else if(!GenIR::checkTypeMatch(this,initData)){
        Error::semError(VAR_INIT_ERR,name);
    }
    else if(initData->literal){
        // 如果是常量,直接进行初始化
        inited = true;
        if(initData->isArray){
            // 如果既是常量又是数组,则必定为字符串
            // 字符串指针初始值等于常量串名(eg .L2)
            ptrVal = initData->name;
        }
        else{
            intVal = initData->intVal;
        }
    }
    else{
        // 否在说明是表达式,需要生成代码进行计算
        if(scopePath.size() == 1){
            Error::semError(GLB_INIT_ERR,name);
        }
        else{
            return true;
        }
    }

    return false;
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

Var* Var::getInitData()
{
    return initData;
}

int Var::getOffset()
{
    return offset;
}

Var* Var::getStep()
{
    if(this->isBase()){
        return SymTab::one;
    }
    else if(this->type == KW_CHAR){
        return SymTab::one;
    }
    else if(this->type==KW_INT){
        return SymTab::four;
    }
    else{
        return nullptr;
    }
}

Var* Var::getPointer()
{
    return ptr;
}

string Var::getPtrVal()
{
    return ptrVal;
}

bool Var::getLeft()
{
    return isLeft;
}

int Var::getVal()
{
    return intVal;
}

bool Var::isBase()
{
    return (!isPtr)&&(!isArray);
}

bool Var::isVoid()
{
    return (type == KW_VOID);
}

bool Var::isRef()
{
    // 是不是一个类似与(*p)的变量
    return (ptr != nullptr);
}

bool Var::isChar()
{
    return (type == KW_CHAR) && isBase();
}

// bool Var::isConst()
// {
//     return literal && isBase();
// }

bool Var::notConst()
{
    return !literal;
}

bool Var::getArray()
{
    return isArray;
}

bool Var::isInit()
{
    return inited;
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

void Var::setLeft(bool isLeft)
{
    this->isLeft = isLeft;
}

void Var::setPoint(Var* ptr)
{
    this->ptr = ptr;
}

void Var::setOffset(int offset)
{
    this->offset = offset;
}

void Var::value()
{
    if(literal){
        if(type == KW_INT){
            printf("%d",intVal);
        }
        else if(type == KW_CHAR){
            if(isArray){
                printf("%s",name.c_str());
            }
            else{
                printf("%d",charVal);
            }
        }
    }
    else{
        printf("%s",name.c_str());
    }
}

string Var::getRawStr()
{
	string raw;
	for(unsigned int i=0;i<strVal.size();i++){
		switch(strVal[i])
		{
			case '\n':raw.append("\\n");break;
			case '\t':raw.append("\\t");break;
			case '\0':raw.append("\\000");break;
			case '\\':raw.append("\\\\");break;
			case '\"':raw.append("\\\"");break;
			default:raw.push_back(strVal[i]);
		}
	}
	raw.append("\\000");//结束标记
	return raw;
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

Fun::Fun(bool isExtern,Symbol type,std::string name,std::vector<Var*> para) :
    intercode(this)
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

Symbol Fun::getType()
{
    return type;
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

int Fun::getMaxDep()
{
    return maxDepth;
}

int Fun::getParaValSize()
{
    return paraVar.size() * 4;
}

bool Fun::match(Fun* f)
{
    // Fun的函数,可以直接访问参数f的私有变量
    if(f->name != this->name){
        return false;
    }

    if(f->type != this->type){
        // 报告语义警告,返回类型不匹配
        Error::semWarm(FUN_RET_CONFLICT,f->getName());
    }

    return match(f->paraVar);
}

bool Fun::match(std::vector<Var*>& paraVar)
{
    if(paraVar.size() != this->paraVar.size()){
        return false;
    }

    unsigned int len = paraVar.size();
    for(unsigned int i=0;i<len;i++){
        // 部分类型可以兼容使用,例如int* 与int[]
        if(GenIR::checkTypeMatch(paraVar[i],this->paraVar[i])){
            if(paraVar[i]->getType() != this->paraVar[i]->getType()){
                Error::semWarm(FUN_DEC_CONFLICT,name);
            }
        }
        else{
            return false;
        }
    }

    return true;
}

void Fun::define(Fun* f)
{
    externed = false;
    paraVar  = f->paraVar;
}

void Fun::locate(Var* var)
{
    int size = var->getSize();
    size = size + ((4 - size % 4) % 4); // 使size变为4的倍数(对齐)
    scopeEsp.back() += size;
    curEsp += size;
    var->setOffset(-curEsp);    // 局部变量的偏移地址为负数
}

void Fun::addInst(InterInst* inst)
{
    intercode.addInst(inst);
}

void Fun::setReturnPoint(InterInst* inst)
{
    returnPoint = inst;
}

InterInst* Fun::getReturnPoint()
{
    return returnPoint;
}

void Fun::toX86(FILE* file)
{
    auto v = intercode.getCode();
    for(auto& i:v){
        i->toX86(&intercode,file);
    }
}

void Fun::printSelf()
{
    //输出type
	printf("%s",tokenName[type]);
	//输出名字
	printf(" %s",name.c_str());
	//输出参数列表
	printf("(");
	for(unsigned int i=0;i<paraVar.size();i++){
		printf("<%s>",paraVar[i]->getName().c_str());
		if(i!=paraVar.size()-1)printf(",");
	}
	printf(")");

	if(externed)printf(";\n");
	else{
		printf(": maxDepth=%d\n",maxDepth);
	}

    auto v = intercode.getCode();
    for(auto& i:v){
        i->printSelf();
    }


}

SymTab::SymTab()
{
    varVoid = new Var();
    one = new Var(1);
    four = new Var(4);
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
        // 如果没有同作用域的同名变量或者是常量,则存入符号表中
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

    if(ir){
        bool flag = ir->genVarInit(var);
        if(currFun && flag){
            currFun->locate(var);
        }
    }
    else{
        // 调用此函数添加变量并生成中间代码
        // 但此时中间代码生成器未初始化,是代码逻辑错误,因此直接抛出异常,中止程序
        throw runtime_error("addVar without GenIR");
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
        // 函数可以重复声明,出现过的同名函数可以直接忽略
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
        else{
            Error::semError(FUN_RE_DEF,f->getName());      // 函数重定义
        }
        delete f;
        f = last;
    }
    currFun = f;
    ir->genFunHead(currFun);   // 产生当前函数的函数头中间代码
}

void SymTab::endDefFun()
{
    ir->genFunTail(currFun);  // 产生当前函数结束的中间代码 
    currFun = nullptr;
}

Fun* SymTab::getFun(std::string name, std::vector<Var*>& args)
{
    if(funTab.find(name) != funTab.end()){
        Fun* last = funTab[name];
        if(last->match(args)){
            return last;
        }
        else{
            // 调用不匹配
            Error::semError(FUN_CALL_ERR,name);
        }
    }
    else{
        // 函数未声明
        Error::semError(FUN_UN_DEC,name);
    }

    return nullptr;
}

void SymTab::printValTab()
{
    for(auto it = varTab.begin();it!=varTab.end();it++){
        for(const auto&i:*(it->second)){
            i->printSelf();
        }
        
    }
}

void SymTab::printFunTab()
{
    for(auto it = funTab.begin();it!=funTab.end();it++){
        it->second->printSelf();
    }
}

void SymTab::printStrTab()
{
    for(auto it = strTab.begin();it!=strTab.end();it++){
        it->second->printSelf();
    }
}

Var* SymTab::getVoid()
{
    return SymTab::varVoid;
}

void SymTab::setGenIR(GenIR* ir)
{
    this->ir = ir;
}

void SymTab::addInst(InterInst* interInst)
{
    if(currFun){
        currFun->addInst(interInst);
    }
    else{
        /*
         * 由于添加变量时会无差别的产生添加赋值语句的操作
         * 因此读入函数声明时的函数参数也会产生空的赋值语句操作
         * 而此时并未进入函数,因此currFun为nullptr,因此可以直接忽略此请求并释放有关内存
         */
        delete interInst;
    }
}

Fun* SymTab::getCurrFun()
{
    return currFun;
}

void SymTab::toX86(FILE* file)
{
    fprintf(file,"section .data\n");
    genData(file);
    fprintf(file,"section .text\n");
    for(auto it = funTab.begin();it!=funTab.end();it++){
        Fun* fun = it->second;
        fprintf(file,"global %s\n",fun->getName().c_str());
        fprintf(file,"%s:\n",fun->getName().c_str());
        it->second->toX86(file);
    }
}

std::vector<Var*> SymTab::getGlobalVars()
{
    vector<Var*> glbVars;;
    for(auto it = varTab.begin();it!=varTab.end();it++){
        string varName = it->first;
        if(varName[0]=='<'){
            continue;
        }

        for(const auto&i:*(it->second)){
            if(i->getPath().size() == 1){
                glbVars.push_back(i);
                break;
            }
        }
    }

    return glbVars;
}


void SymTab::genData(FILE* file)
{
    vector<Var*> glbVars = getGlobalVars();
    for(Var* var:glbVars){
        fprintf(file,"global %s\n",var->getName().c_str());
        fprintf(file,"\t%s " ,var->getName().c_str());
        int typeSize = var->getType() == KW_CHAR ? 1 : 4;
        if(var->getArray()){
            fprintf(file,"times %d",var->getSize()/typeSize);
        }
        const char* type = var->getType() == KW_CHAR ? "db" : "dd";
        fprintf(file,"%s " ,type);

        if(var->isInit()){
            if(var->isBase()){
                fprintf(file,"%d\n",var->getVal());
            }
            else{
                fprintf(file,"%s\n",var->getPtrVal().c_str());
            }
        }
        else{
            fprintf(file,"0\n");
        }
    }

    for(auto it = strTab.begin();it!=strTab.end();it++){
        Var* str = it->second;
        fprintf(file,"\t%s db %s\n",str->getName().c_str(),str->getRawStr().c_str());
    }
}