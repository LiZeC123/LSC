#include "GenIR.h"
#include "SymTab.h"
#include <sstream>

using namespace std;

int GenIR::lbNum = 0;

GenIR::GenIR(SymTab tab): symtab(tab)
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