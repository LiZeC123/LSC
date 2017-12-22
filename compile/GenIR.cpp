#include "GenIR.h"
#include "SymTab.h"
#include <sstream>
#include "Error.h"

using namespace std;

int GenIR::lbNum = 0;

GenIR::GenIR(SymTab& tab): symtab(tab)
{
    symtab.setGenIR(this);
}

string GenIR::genLb()
{
    lbNum++;
	string lb=".L";//为了和汇编保持一致！
	stringstream ss;
	ss<<lbNum;
	return lb+ss.str();
}

bool GenIR::checkTypeMatch(Var* lval,Var* rval)
{
    if(!lval || !rval){
        return false;
    }
    else if(lval->isBase() && rval->isBase()){
        // 由于参数不能为void 且 char与int可以相互转化
        // 所以只要都是基本类型,就认为相同
        return true;
    }
	else if(!lval->isBase() && !rval->isBase()){
        // 如果都是非基本类型,则只要基本类型相同
        // 例如int* 与int[] 兼容
        return (rval->getType()==lval->getType());
    }
    else{
        return false;
    }
}

void GenIR::genFunHead(Fun* fun)
{
    fun->enterScope();
    symtab.addInst(new InterInst(OP_ENTRY,fun));
    fun->setReturnPoint(new InterInst());
}


void GenIR::genFunTail(Fun* fun)
{
    symtab.addInst(fun->getReturnPoint());
    symtab.addInst(new InterInst(OP_EXIT,fun));
    fun->leaveScope();
}

void GenIR::genReturn(Var* result)
{
    if(result == nullptr){
        return;
    }

    Fun* fun = symtab.getCurrFun();
    
    if((result->isVoid() && fun->getType() != KW_VOID) || 
        (result->isBase() && fun->getType() == KW_VOID)){
        
        Error::semError(RETURN_ERR,fun->getName());
        return;
    }

    InterInst* returnPoint = fun->getReturnPoint();

    if(result->isVoid()){
        symtab.addInst(new InterInst(OP_RET,returnPoint));
    }
    else{
        if(result->isBase()){
            //TODO: 处理ret是*p的q情况
        }
        symtab.addInst(new InterInst(OP_RETV,returnPoint,result));
    }
    
}