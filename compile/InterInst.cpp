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

InterInst::InterInst(Operator op,Var* arg1)
{
    init();
    this->op = op;
    this->arg1 = arg1;
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
    printf("\t");
	if(!label.empty()){
		printf("%s:\n",label.c_str());
		return;
	}

	switch(op)
	{
		case OP_NOP:printf("nop");break;
		case OP_DEC:printf("dec ");arg1->value();break;
		case OP_ENTRY:printf("entry");break;
		case OP_EXIT:printf("exit");break;
		case OP_AS:result->value();printf(" = ");arg1->value();break;
		case OP_ADD:result->value();printf(" = ");arg1->value();printf(" + ");arg2->value();break;
		case OP_SUB:result->value();printf(" = ");arg1->value();printf(" - ");arg2->value();break;
		case OP_MUL:result->value();printf(" = ");arg1->value();printf(" * ");arg2->value();break;
		case OP_DIV:result->value();printf(" = ");arg1->value();printf(" / ");arg2->value();break;
		case OP_MOD:result->value();printf(" = ");arg1->value();printf(" %% ");arg2->value();break;
		case OP_NEG:result->value();printf(" = ");printf("-");arg1->value();break;
		case OP_GT:result->value();printf(" = ");arg1->value();printf(" > ");arg2->value();break;
		case OP_GE:result->value();printf(" = ");arg1->value();printf(" >= ");arg2->value();break;
		case OP_LT:result->value();printf(" = ");arg1->value();printf(" < ");arg2->value();break;
		case OP_LE:result->value();printf(" = ");arg1->value();printf(" <= ");arg2->value();break;
		case OP_EQU:result->value();printf(" = ");arg1->value();printf(" == ");arg2->value();break;
		case OP_NE:result->value();printf(" = ");arg1->value();printf(" != ");arg2->value();break;
		case OP_NOT:result->value();printf(" = ");printf("!");arg1->value();break;
		case OP_AND:result->value();printf(" = ");arg1->value();printf(" && ");arg2->value();break;
		case OP_OR:result->value();printf(" = ");arg1->value();printf(" || ");arg2->value();break;
		case OP_JMP:printf("goto %s",target->label.c_str());break;
		case OP_JT:printf("if( ");arg1->value();printf(" )goto %s",target->label.c_str());break;
		case OP_JF:printf("if( !");arg1->value();printf(" )goto %s",target->label.c_str());break;
		case OP_JNE:printf("if( ");arg1->value();printf(" != ");arg2->value();printf(" )goto %s",
			target->label.c_str());break;
		case OP_ARG:printf("arg ");arg1->value();break;
		case OP_PROC:printf("%s()",fun->getName().c_str());break;
		case OP_CALL:result->value();printf(" = %s()",fun->getName().c_str());break;
		case OP_RET:printf("return goto %s",target->label.c_str());break;
		case OP_RETV:printf("return ");arg1->value();printf(" goto %s",target->label.c_str());break;
		case OP_LEA:result->value();printf(" = ");printf("&");arg1->value();break;
		case OP_SET:printf("*");arg1->value();printf(" = ");result->value();break;
		case OP_GET:result->value();printf(" = ");printf("*");arg1->value();break;
	}
	printf("\n");
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

