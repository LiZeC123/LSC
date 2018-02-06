#include "InterInst.h"
#include "GenIR.h"
#include "SymTab.h"

#define emit(fmt,args...) fprintf(file,"\t");fprintf(file,fmt, ##args);fprintf(file,"\n")
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

void InterInst::loadVar(string reg32,string reg8,Var* var,FILE* file)
{
	if(!var){
		return;
	}
	const char* reg = var->isChar()?reg8.c_str():reg32.c_str();

	if(var->isChar()){
		emit("mov %s,0",reg32.c_str());			// 寄存器清零
	}
	const char* name = var->getName().c_str();

	if(var->notConst()){
		int offset = var->getOffset();
		if(offset == 0){						// 全局变量
			if(!var->getArray()){				
				emit("mov %s,[%s]",reg,name);	// 普通变量,例如 mov eax,[var]
			}
			else{
				emit("mov %s,%s",reg,name);		// 数组,例如 mov eax, array
			}
		}
		else{									// 局部变量
			if(!var->getArray()){
				emit("mov %s,[ebp%+d]",reg,offset);		// 普通变量,例如 mov eax,[ebp-4]
			}
			else{
				emit("lea %s,[ebp%+d]",reg,offset);		// 数组,例如 lea eax,[ebp-4]
			}
		}
	}
	else{											// 常量
		if(var->isBase()){
			emit("mov %s,%d",reg,var->getVal());	// 基本类型,例如 mov eax,10
		}
		else{
			emit("mov %s,%s",reg,name);				//字符串,直接使用名称,例如 mov eax,str
		}
	}
}

void InterInst::leaVar(string reg32,Var* var,FILE* file)
{
	if(!var){
		return;
	}

	const char* reg = reg32.c_str();
	const char* name = var->getName().c_str();
	int offset = var->getOffset();

	if(offset==0){
		emit("mov %s,%s",reg,name);	
	}
	else{
		emit("lea %s,[ebp%+d]",reg,offset);
	}
}

void InterInst::storeVar(string reg32,string reg8,Var* var,FILE* file)
{
	if(!var){
		return;
	}

	const char* reg = var->isChar()?reg8.c_str():reg32.c_str();
	const char* name = var->getName().c_str();
	int offset = var->getOffset();

	if(offset==0){
		emit("mov [%s],%s",name,reg);	
	}
	else{
		emit("mov [ebp%+d],%s",offset,reg);
	}
}

void InterInst::initVar(Var* var,FILE* file)
{
	if(!var){
		return;
	}

	if(var->isInit()){
		if(var->isBase()){
			emit("mov eax,%d",var->getVal());
		}
		else{
			emit("mov eax,%s",var->getPtrVal().c_str());
		}
		storeVar("eax","al",var,file);
	}
}

void InterInst::toX86(InterCode* inst,FILE* file)
{
	if(!label.empty()){
		fprintf(file,"%s:\n",label.c_str());
		return;
	}

	switch (op)
	{
	case OP_NOP:
		emit("nop");
		break;
	case OP_DEC:
		initVar(arg1, file);
		break;
	case OP_ENTRY:
		emit("push ebp");
		emit("mov ebp,esp");
		emit("sub esp,%d", inst->getFun()->getMaxDep());
		break;
	case OP_EXIT:
		emit("mov esp,ebp");
		emit("pop ebp");
		emit("ret");
		break;
	case OP_AS:
		loadVar("eax", "al", arg1, file);
		storeVar("eax", "al", result, file);
		break;
	case OP_ADD:
		loadVar("eax", "al", arg1, file);
		loadVar("ebx", "bl", arg2, file);
		emit("add eax,ebx");
		storeVar("eax", "al", result, file);
		break;
	case OP_SUB:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("sub eax,ebx");
		storeVar("eax","al",result,file);
		break;
	case OP_MUL:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("imul ebx");
		storeVar("eax","al",result,file);
		break;
	case OP_DIV:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("idiv ebx");
		storeVar("eax","al",result,file);
		break;
	case OP_MOD:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("idiv ebx");
		storeVar("edx","dl",result,file);
		break;
	case OP_NEG:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("neg eax");
		storeVar("eax","al",result,file);
		break;
	case OP_GT:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("mov ecx,0");
		emit("cmp eax,ebx");
		emit("setg cl");
		storeVar("ecx","cl",result,file);
		break;
	case OP_GE:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("mov ecx,0");
		emit("cmp eax,ebx");
		emit("setge cl");
		storeVar("ecx","cl",result,file);
		break;
	case OP_LT:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("mov ecx,0");
		emit("cmp eax,ebx");
		emit("setl cl");
		storeVar("ecx","cl",result,file);
		break;
	case OP_LE:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("mov ecx,0");
		emit("cmp eax,ebx");
		emit("setle cl");
		storeVar("ecx","cl",result,file);
		break;
	case OP_EQU:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("mov ecx,0");
		emit("cmp eax,ebx");
		emit("sete cl");
		storeVar("ecx","cl",result,file);
		break;
	case OP_NE:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("mov ecx,0");
		emit("cmp eax,ebx");
		emit("setne cl");
		storeVar("ecx","cl",result,file);
		break;
	case OP_NOT:
		loadVar("eax","al",arg1,file);
		emit("mov ebx,0");
		emit("cmp eax,0");
		emit("sete bl");
		storeVar("ebx","bl",result,file);
		break;
	case OP_AND:
		loadVar("eax","al",arg1,file);
		emit("cmp eax,0");
		emit("setne cl");
		loadVar("ebx","bl",arg2,file);
		emit("cmp ebx,0");
		emit("setne bl");
		emit("and eax,ebx");
		storeVar("eax","al",result,file);
		break;
	case OP_OR:
		loadVar("eax","al",arg1,file);
		emit("cmp eax,0");
		emit("setne cl");
		loadVar("ebx","bl",arg2,file);
		emit("cmp ebx,0");
		emit("setne bl");
		emit("or eax,ebx");
		storeVar("eax","al",result,file);
		break;
	case OP_JMP:
		emit("jmp %s",target->label.c_str());
		break;
	case OP_JT:
		loadVar("eax","al",arg1,file);
		emit("cmp eax,0");
		emit("jne %s",target->label.c_str());
		break;
	case OP_JF:
		loadVar("eax","al",arg1,file);
		emit("cmp eax,0");
		emit("je %s",target->label.c_str());
		break;
	case OP_JNE:
		loadVar("eax","al",arg1,file);
		loadVar("ebx","bl",arg2,file);
		emit("cmp eax,ebx");
		emit("jne %s",target->label.c_str());
		break;
	case OP_ARG:
		loadVar("eax","al",arg1,file);
		emit("push eax");
		break;
	case OP_PROC:
		emit("call %s",fun->getName().c_str());
		emit("add esp,%d",fun->getParaValSize());
		break;
	case OP_CALL:
		emit("call %s",fun->getName().c_str());
		emit("add esp,%d",fun->getParaValSize());
		storeVar("eax","al",result,file);
		break;
	case OP_RET:
		emit("jmp %s",target->label.c_str());
		break;
	case OP_RETV:
		loadVar("eax","al",arg1,file);
		emit("jmp %s",target->label.c_str());
		break;
	case OP_LEA:
		leaVar("eax",arg1,file);
		storeVar("eax","al",result,file);
		break;
	case OP_SET:
		loadVar("eax","al",result,file);
		loadVar("ebx","bl",arg1,file);	
		emit("mov [ebx],eax");
		break;	
	case OP_GET:
		loadVar("eax","al",arg1,file);
		emit("mov eax,[eax]");
		storeVar("eax","al",result,file);
		break;
	}
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

InterCode::InterCode(Fun* currFun)
{
	this->fun = currFun;
}

void InterCode::addInst(InterInst* inst)
{
    code.push_back(inst);
}

Fun* InterCode::getFun()
{
	return fun;
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

