#include "GenIR.h"
#include "SymTab.h"
#include <sstream>
#include "Error.h"
#include "InterInst.h"
using namespace std;

int GenIR::lbNum = 0;

GenIR::GenIR(SymTab& tab): symtab(tab)
{
    symtab.setGenIR(this);
}

string GenIR::genLb(bool isStr)
{
    lbNum++;
    string lb = isStr ? "@L" : ".L";
    stringstream ss;
    ss << lbNum;
    return lb + ss.str();
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
        // 如果都是指针,若指针等级相同则类型相同视为相同
        int llv = lval->getPtrLevel()+lval->getArray();
        int rlv = rval->getPtrLevel()+rval->getArray();
        if(llv==rlv){
            return (rval->getType()==lval->getType());
        }
        else{
            return false;
        }        
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
        if(result->isRef()){
            result = genAssign(result);
        }
        symtab.addInst(new InterInst(OP_RETV,returnPoint,result));
    }
    
}

bool GenIR::genVarInit(Var* var)
{
    if(var->getName()[0] == '<'){
        return false;
    }

    symtab.addInst(new InterInst(OP_DEC,var));
    if(var->setInit()){
        genTwoOp(var,ASSIGN,var->getInitData());
    }

    return true;
}

/**
 *  genPtr仅仅在符号表上产生了一个变量
 *  并且将该变量的ptr字段指向输入的变量p, 其本身并没有保存*p的值
 *  因此在后续的赋值操作中需要对此变量进行特殊的判断
 *  即如果需要使用此变量的值, 则临时的加入OP_GET指令获得值, 然后再参与运行
 *  如果没有用到此变量的值, 则不产生任何代码
 */
Var* GenIR::genPtr(Var* val)
{
    if(val->isBase()){
        Error::semError(EXPR_IS_BASE,"");
        return val;
    }

    Var* tmp = new Var(symtab.getScopePath(),val->getType(),false);
    tmp->setLeft(true);
    tmp->setPoint(val);
    tmp->setPtrLevel(val->getPtrLevel()+val->getArray()-1);
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
        tmp->setPtrLevel(val->getPtrLevel()+val->getArray()+1);
        symtab.addVar(tmp);
        symtab.addInst(new InterInst(OP_LEA,tmp,val));
        return tmp;
    }

}

Var* GenIR::genIncL(Var* val)
{
    if(!val->getLeft()){
        Error::semError(EXPR_NOT_LEFT_VAL,"");
        return val;
    }

    if(val->isRef()){
        Var* t1 = genAssign(val);               // t1 = (*p)
        Var* t2 = genAnd(t1,val->getStep());    // t2 = t1 + sizeof(t1)
        genAssign(val,t2);                      // (*p) = t2
    }
    else{
        symtab.addInst(new InterInst(OP_ADD,val,val,val->getStep()));
    }

    return val;
}

Var* GenIR::genDecL(Var* val)
{
    if(!val->getLeft()){
        Error::semError(EXPR_NOT_LEFT_VAL,"");
        return val;
    }

    if(val->isRef()){
        Var* t1 = genAssign(val);               // t1 = (*p)
        Var* t2 = genSub(t1,val->getStep());    // t2 = t1 - sizeof(t1)
        genAssign(val,t2);                      // (*p) = t2
    }
    else{
        symtab.addInst(new InterInst(OP_SUB,val,val,val->getStep()));
    }

    return val;
}

Var* GenIR::genIncR(Var* val)
{
    Var* tmp = genCopy(val);                  // tmp = val
    if(val->isRef()){
        Var* t2 = genAdd(tmp,val->getStep());   // t2 = tmp + sizeof(tmp)
        genAssign(val,t2);                      // (*p) = t2
    }
    else{
        symtab.addInst(new InterInst(OP_ADD,val,val,val->getStep()));
    }
    return tmp;                                 // return tmp
}   

Var* GenIR::genDecR(Var* val)
{
    Var* tmp = genCopy(val);
    if(val->isRef()){
        Var* t2 = genSub(tmp,val->getStep());
        genAssign(val,t2);
    }
    else{
        symtab.addInst(new InterInst(OP_SUB,val,val,val->getStep()));
    }
    return tmp;
}

Var* GenIR::genNot(Var* val)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_NOT,tmp,val));
    return tmp;
} 

Var* GenIR::genMinus(Var* val)
{
    if(!val->isBase()){
        Error::semError(EXPR_NOT_BASE,"");
        return val;
    }

    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_NEG,tmp,val));
    return tmp;
}

/*
 * 由于genPtr不生成代码, 因此通过genPtr得到的变量中不包含实际的值
 * 即val变量可能只有ptr字段指向了变量p,但本身没有保存*p的内容
 * 因此需要判断val是否为引用类型(ptr字段是否有效), 亦即val是否是通过genPtr运算得到的
 * 是则, 则通过OP_GET获得*p实际的值
 * 否则, 说明val是普通变量,直接使用值即可
*/
Var* GenIR::genAssign(Var* val)
{
    if(val->isRef()){
        Var* before = genAssign(val->getPointer());
        symtab.addInst(new InterInst(OP_GET,val,before));
        return val;
    }
    else{
        return val;
    }
}

Var* GenIR::genCopy(Var* val)
{
    Var* tmp = new Var(symtab.getScopePath(),val);
    symtab.addVar(tmp);     // 任何创建的变量都需要保存到变量表中
    if(val->isRef()){
        // 如果是引用类型, 需要先取出真实值,然后再复制
        val = genAssign(val);
    }

    symtab.addInst(new InterInst(OP_AS,tmp,val));

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
        // 如果rval是引用类型,产生一个等价的右值
        // 即通过OP_GET获得实际的值
        rval = genAssign(rval);
    }
    if(lval->isRef()){
        lval = genAssign(lval);
        // 如果lva是引用类型,直接执行 OP_SET
        // 此时说明lval是某个变量通过*运算得到的, 此时操作实际与lval本身无关
        // 可以直接将数据写入lval执行的位置
        // OP_SET rval, lval->getPoinger() 等价与 *(&lval) = rval
        symtab.addInst(new InterInst(OP_SET,rval,lval->getPointer()));
    }
    else{
        // 否则lval是普通变量,直接产生赋值语句
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

    if(op == ASSIGN){
        return genAssign(lval,rval);
    } 

    // 如果是引用类型,取出相应的值
    if(lval->isRef()) lval = genAssign(lval);
    if(rval->isRef()) rval = genAssign(rval);

    switch (op)
    {
    case OR:
        return genOr(lval, rval);
    case AND:
        return genAnd(lval, rval);
    case EQU:
        return genEqu(lval, rval);
    case NEQU:
        return genNequ(lval, rval);
    case ADD:
        return genAdd(lval, rval);
    case SUB:
        return genSub(lval, rval);
    default:
        break;
    }

    // 后续运算只有基本类型可以参与,生成代码前先进行判断
    if (!lval->isBase() || !rval->isBase()){
        Error::semError(EXPR_NOT_BASE, "");
        return lval;
    }

    switch (op)
    {
    case GT:
        return genGt(lval, rval);
    case GE:
        return genGe(lval, rval);
    case LT:
        return genLt(lval, rval);
    case LE:
        return genLe(lval, rval);
    case MUL:
        return genMul(lval, rval);
    case DIV:
        return genDiv(lval, rval);
    case MOD:
        return genMod(lval, rval);
    default:
        return lval;
    }

    return lval;
}

Var* GenIR::genOneLeftOp(Symbol op,Var* val)
{
    if(!val){
        return nullptr;
    }
    if(val->isVoid()){
        Error::semError(EXPR_IS_VOID,"");
        return nullptr;
    }

    switch (op)
    {
    case LEA:
        return genLea(val);
    case MUL:
        return genPtr(val);
    case INC:
        return genIncL(val);
    case DEC:
        return genDecL(val);
    default:
        break;
    }

    if (val->isRef()){
        val = genAssign(val);
    }

    switch (op)
    {
    case NOT:
        return genNot(val);
    case SUB:
        return genMinus(val);
    default:
        break;
    }
    return val;
}

Var* GenIR::genOneRightOp(Var* val,Symbol op)
{
    if(!val){
        return nullptr;
    }
    else if(val->isVoid()){
        Error::semError(EXPR_IS_VOID,"");
        return nullptr;
    }
    else if(!val->getLeft()){
        Error::semError(EXPR_NOT_LEFT_VAL,"");
        return val;
    }

    switch (op)
    {
    case INC:
        return genIncR(val);
    case DEC:
        return genDecR(val);
    default:
        return val;
    }
}

Var* GenIR::genArray(Var* array,Var* index)
{
    if(!array || ! index) {
        return nullptr;
    }

    if(array->isVoid() || index->isVoid()){
        Error::semError(EXPR_IS_VOID,"");
        return nullptr;
    }

    if(array->isBase() || !index->isBase()){
        Error::semError(ARR_TYPE_ERR,"");
        return index;
    }

    return genPtr(genAdd(array,index));
}



Var* GenIR::genCall(Fun* fun, std::vector<Var*>& args)
{
    if(!fun){
        return nullptr;
    }

    for(int i=args.size()-1;i>=0;i--){
        genPara(args[i]); //反向压入函数参数
    }

    if(fun->getType() == KW_VOID){
        symtab.addInst(new InterInst(OP_PROC,fun));
        return SymTab::getVoid();
    }
    else{
        Var* ret = new Var(symtab.getScopePath(),fun->getType(),false);
        symtab.addInst(new InterInst(OP_CALL,fun,ret));
        symtab.addVar(ret); // 将返回值的声明延迟到函数调用以后 TODO: why?
        return ret;
    }
}

void GenIR::genIfHead(Var* cond,InterInst*& _else)
{
    _else = new InterInst();
    if(cond){
        if(cond->isRef()){
            cond = genAssign(cond);
        }
        symtab.addInst(new InterInst(OP_JF,_else,cond));
    }
}

void GenIR::genIfTail(InterInst*& _else)
{
    symtab.addInst(_else);
}

void GenIR::genElseHead(InterInst* _else,InterInst*& _exit)
{
    _exit = new InterInst();
    symtab.addInst(new InterInst(OP_JMP,_exit));
    symtab.addInst(_else);
}

void GenIR::genElseTail(InterInst*& _exit)
{
    symtab.addInst(_exit);
}

void GenIR::genWhileHead(InterInst*&_while,InterInst*& _exit)
{
    _while = new InterInst();
    symtab.addInst(_while);
    _exit = new InterInst();
    push(_while,_exit);
}

void GenIR::genWhileCond(Var* cond,InterInst* _exit)
{
    if(cond){
        if(cond->isVoid()){
            // TODO: getTrue();
        }
        else if(cond->isRef()){
            cond = genAssign(cond);
        }

        symtab.addInst(new InterInst(OP_JF,_exit,cond));
    }
}

void GenIR::genWhileTail(InterInst*& _while,InterInst*& _exit)
{
    symtab.addInst(new InterInst(OP_JMP,_while));
    symtab.addInst(_exit);
    pop();
}

void GenIR::genBreak()
{
    InterInst* tail = taillist.back();
    if(tail){
        symtab.addInst(new InterInst(OP_JMP,tail));
    }
    else{
        Error::semError(BREAK_ERR,"");
    }
}

void GenIR::genContinue()
{
    InterInst* head = headlist.back();
    if(head){
        symtab.addInst(new InterInst(OP_JMP,head));
    }
    else{
        Error::semError(CONTINUE_ERR,"");
    }
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
    else if(!lval->isBase() && !rval->isBase()){
        // 都是指针
        return true;
    }

    return false;
}

Var* GenIR::genOr(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_OR,tmp,lval,rval));
    return tmp;
}
Var* GenIR::genAnd(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_AND,tmp,lval,rval));
    return tmp;
}
Var* GenIR::genEqu(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_EQU,tmp,lval,rval));
    return tmp;
}
Var* GenIR::genNequ(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_NE,tmp,lval,rval));
    return tmp;
}

Var* GenIR::genAdd(Var* lval,Var* rval)
{
    Var* tmp = nullptr;
    // 对于指针运算,p+3 <==> p + 3*sizeof(*p)
    // 因此先使用乘法运算计算实际偏移量
    if(!lval->isBase() && rval->isBase()){
        tmp = new Var(symtab.getScopePath(),lval);
        rval = genMul(rval,lval->getStep());
    }
    else if(lval->isBase() && !rval->isBase()){
        tmp = new Var(symtab.getScopePath(),rval);
        lval = genMul(lval,rval->getStep());
    }
    else if(lval->isBase() && rval->isBase()){
        tmp = new Var(symtab.getScopePath(),KW_INT,false);
    }
    else{
        Error::semError(EXPR_NOT_BASE,"");
        return lval;
    }

    // 完成可能的偏移量计算以后,直接产生加法指令即可完成计算
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_ADD,tmp,lval,rval));
    return tmp;
}

Var* GenIR::genSub(Var* lval,Var* rval)
{
    Var* tmp = nullptr;
    if(!rval->isBase()){
        // 类似于 2-p 的操作是非法的
        Error::semError(EXPR_NOT_BASE,"");
        return lval;
    }

    if(!lval->isBase()){
        tmp = new Var(symtab.getScopePath(),lval);
        rval = genMul(rval,lval->getStep());
    }
    else{
        tmp = new Var(symtab.getScopePath(),KW_INT,false);
    }

    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_SUB,tmp,lval,rval));
    return tmp;
}

Var* GenIR::genGt(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_GT,tmp,lval,rval));
    return tmp;
}
Var* GenIR::genGe(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_GE,tmp,lval,rval));
    return tmp;
}
Var* GenIR::genLt(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_LT,tmp,lval,rval));
    return tmp;
}
Var* GenIR::genLe(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_LE,tmp,lval,rval));
    return tmp;
}

Var* GenIR::genMul(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_MUL,tmp,lval,rval));
    return tmp;
}

Var* GenIR::genDiv(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_DIV,tmp,lval,rval));
    return tmp;
}
Var* GenIR::genMod(Var* lval,Var* rval)
{
    Var* tmp = new Var(symtab.getScopePath(),KW_INT,false);
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_MOD,tmp,lval,rval));
    return tmp;
}

void GenIR::genPara(Var* arg)
{
    if(arg->isRef()){
        arg = genAssign(arg);
    }

    symtab.addInst(new InterInst(OP_ARG,arg));
}

void GenIR::push(InterInst* head,InterInst* tail)
{
    headlist.push_back(head);
    taillist.push_back(tail);
}

void GenIR::pop()
{
    headlist.pop_back();
    taillist.pop_back();
}


