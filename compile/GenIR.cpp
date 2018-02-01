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
            //TODO: 处理ret是*p的情况
        }
        symtab.addInst(new InterInst(OP_RETV,returnPoint,result));
    }
    
}

bool GenIR::genVarInit(Var* var)
{
    if(var->getName()[0] == '<'){
        return false;
    }

    symtab.addInst(new InterInst(OP_DEC,var,nullptr));
    if(var->setInit()){
        genTwoOp(var,ASSIGN,var->getInitData());
    }

    return true;
}

Var* GenIR::genPtr(Var* val)
{
    if(val->isBase()){
        Error::semError(EXPR_IS_BASE,"");
        return val;
    }

    Var* tmp = new Var(symtab.getScopePath(),val->getType(),false);
    tmp->setLeft(true);
    tmp->setPoint(val);
    symtab.addVar(tmp);
    return tmp;
}

Var* GenIR::genLea(Var* val)
{
    if(!val->getLeft()){
        Error::semError(EXPR_NOT_LEFT_VAL,"");
        return val;
    }

    if(val->isRef()){
        return val->getPointer();
    }
    else{
        Var* tmp = new Var(symtab.getScopePath(),val->getType(),true);
        symtab.addVar(tmp);
        symtab.addInst(new InterInst(OP_LEA,tmp,val));
        return tmp;
    }

}

Var* GenIR::genAssign(Var* val)
{
    Var* tmp = new Var(symtab.getScopePath(),val);
    symtab.addVar(tmp);
    if(val->isRef()){
        symtab.addInst(new InterInst(OP_GET,tmp,val->getPointer()));
    }
    else{
        symtab.addInst(new InterInst(OP_AS,tmp,val));
    }
    return tmp;
}

Var* GenIR::genAssign(Var* lval,Var* rval)
{
    if(!lval->getLeft()){
        Error::semError(EXPR_NOT_LEFT_VAL,"");
        return rval;
    }

    if(!typeCheck(lval,rval)){
        Error::semError(ASSIGN_TYPE_ERR,"");
        return rval;
    }

    if(rval->isRef()){
        // r = (*r)  OR r = r
        rval = genAssign(rval);
    }

    if(lval->isRef()){
        //  (*l) = r
        symtab.addInst(new InterInst(OP_SET,rval,lval->getPointer()));
    }
    else{
        //  l = r
        symtab.addInst(new InterInst(OP_AS,lval,rval));
    }

    return lval;
}

Var* GenIR::genTwoOp(Var* lval,Symbol op,Var* rval)
{
    if(!lval || !rval ){
        return nullptr;
    }

    if(lval->isVoid() || rval->isVoid()){
        Error::semError(EXPR_IS_VOID,"");
        return nullptr;
    }

    if(op == ASSIGN) return genAssign(lval,rval);
    if(lval->isRef()) lval = genAssign(lval);
    if(rval->isRef()) rval = genAssign(lval);
    if(op == ADD) return genAdd(lval,rval);
    if(op == SUB) return genSub(lval,rval);
    if(op == MUL) return genMul(lval,rval);
    return lval;
}

Var* GenIR::genOneLeftOp(Symbol op,Var* val)
{
    return val;
}

Var* GenIR::genOneRightOp(Var* val,Symbol op)
{
    return val;
}

Var* GenIR::genArray(Var* val,Var* index)
{
    //TODO: 数组代码
    return val;
}



Var* GenIR::genCall(Fun* fun, std::vector<Var*>& args)
{
    //TODO: 函数调用代码
    return nullptr;
}


bool GenIR::typeCheck(Var* lval,Var* rval)
{
    if(!rval || !lval){
        return false;
    }
    if(lval->isBase() && rval->isBase()){
        // 都是基本类型
        return true;
    }
    else if(!lval->isBase() && rval->isBase()){
        // 都是指针
        return true;
    }

    return false;
}

Var* GenIR::genAdd(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),lval->getType(),false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_ADD,tmp,lval,rval));
    return tmp;
}

Var* GenIR::genSub(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),lval->getType(),false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_SUB,tmp,lval,rval));
    return tmp;
}

Var* GenIR::genMul(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),lval->getType(),false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_MUL,tmp,lval,rval));
    return tmp;
}