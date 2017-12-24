#include "InterInst.h"
#include "GenIR.h"

using namespace std;

void InterInst::init()
{
    label = "";
    op = OP_NOP;
    result = nullptr;
    arg1 = nullptr;
    arg2 = nullptr;
    fun = nullptr;
    target = nullptr;
}

InterInst::InterInst(Operator op, Var* result, Var* arg1, Var* arg2)
{
    init();
    this->op = op;
    this->result = result;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

InterInst::InterInst(Operator op, Fun* fun, Var* result)
{
    init();
    this->op = op;
    this->fun = fun;
    this->result = result;
}

InterInst::InterInst()
{
    init();
    this->label = GenIR::genLb();
}

InterInst::InterInst(Operator op,InterInst* tar,Var* arg1,Var* arg2)
{
    init();
    this->op = op;
    this->target = tar;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

void InterInst::printSelf()
{
    //TODO: 打印指令
}

void InterCode::addInst(InterInst* inst)
{
    code.push_back(inst);
}

vector<InterInst*>& InterCode::getCode()
{
    return code;
}

void InterCode::printSelf()
{
    for(auto&i:code){
        i->printSelf();
    }
}

InterCode::~InterCode()
{
    for(auto& i:code){
        delete i;
    }
}

