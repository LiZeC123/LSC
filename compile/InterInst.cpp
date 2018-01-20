#include "InterInst.h"
#include "GenIR.h"
#include "SymTab.h"

using namespace std;

const char* OPNameTable[] = 
{
	"OP_NOP",   
	"OP_DEC",                                 
	"OP_ENTRY","OP_EXIT",                      
	"OP_AS",                                  
	"OP_ADD","OP_SUB","OP_MUL","OP_DIV","OP_MOD",     
	"OP_NEG",                                 
	"OP_GT","OP_GE","OP_LT","OP_LE","OP_EQU","OP_NE",   

	"OP_NOT",                                
	"OP_AND","OP_OR",                           

	"OP_LEA",                                 
	"OP_SET",                                 
	"OP_GET",                                

	"OP_JMP",                                 
	"OP_JT",                                  
	"OP_JF",                                  
	"OP_JNE",                                 

	"OP_ARG",                                 
	"OP_PROC",                                
	"OP_CALL",                                
	"OP_RET",                               
	"OP_RETV"                    
};

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
    if(!label.empty()){
        printf("\t%s",label.c_str());
    }
    else{
        printf("\t %s",OPNameTable[op]);
        if(result) printf(" %s",result->getName().c_str());
        if(arg1) printf(" %s", arg1->getName().c_str());
        if(arg2) printf(" %s", arg2->getName().c_str());
        if(fun) printf(" %s",fun->getName().c_str());   
    }
    printf("\n");
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

