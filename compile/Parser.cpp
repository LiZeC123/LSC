#include "Parser.h"
#include "Error.h"

#define firstIs(C) look->sym == C       // 判断向前读取的一个符号是不是指定的符号
#define _OR_(T) || look->sym == T       // 与上面的宏连用,组成更复杂的条件语句

//类型的First集
#define FIRST_TYPE firstIs(KW_INT)_OR_(KW_CHAR)_OR_(KW_VOID)_OR_(KW_STRUCT)
//表达式First集
#define FIRST_EXPR firstIs(LPAREN)_OR_(NUM)_OR_(CH)_OR_(STR)_OR_(IDENT)_OR_(NOT)         \
_OR_(SUB)_OR_(LEA)_OR_(MUL)_OR_(INC)_OR_(DEC)_OR_(KW_SIZEOF)
//左值First集
#define LVAL_OP firstIs(ASSIGN)_OR_(OR)_OR_(AND)_OR_(GT)_OR_(GE)_OR_(LT)_OR_(LE)_OR_(EQU) \
_OR_(NEQU)_OR_(AND)_OR_(SUB)_OR_(MUL)_OR_(DIV)_OR_(MOD)_OR_(INC)_OR_(DEC)
// 右值First
#define RVAL_OP firstIs(OR)_OR_(AND)_OR_(GT)_OR_(GE)_OR_(LT)_OR_(LE)_OR_(EQU) \
_OR_(NEQU)_OR_(AND)_OR_(SUB)_OR_(MUL)_OR_(DIV)_OR_(MOD)

//语句First集
#define FIRST_STATEMENT (FIRST_EXPR)_OR_(SEMICON)_OR_(KW_WHILE)_OR_(KW_FOR)              \
_OR_(KW_DO)_OR_(KW_IF)_OR_(KW_SWITCH)_OR_(KW_RETURN)_OR_(KW_BREAK)_OR_(KW_CONTINUE)

using namespace std;

Parser::Parser(Lexer& lex,SymTab& tab,GenIR& iir) : lexer(lex),symtab(tab),ir(iir) 
{ 
    printToken = false;
}

void Parser::analyse()
{
    move();
    program();
}

void Parser::setPrintToken()
{
    printToken = true;
}

void Parser::move()
{
    look = lexer.nextToken();

    if(printToken){
        printf("%s\n",look->toString().c_str());
    }
}

bool Parser::match(Symbol need)
{
    if(look->sym == need){
        move();
        return true;
    }
    else{
        return false;
    }
}

// <program> -> <segment><program>
// <program> -> e
void Parser::program()
{
    if(look->sym == END){
        return;
    }
    else{
        segment();
        program();
    }

}

// <segment> -> extern <type><def>
// <segment> -> <type><def>
void Parser::segment()
{
    bool isExtern = match(KW_EXTERN);
    Type* s = type();
    def(isExtern,s);
}

// <type>    -> int | void | char 
//           -> struct <ident>
Type* Parser::type()
{
    if(firstIs(KW_INT)_OR_(KW_CHAR)_OR_(KW_VOID)){
        Symbol t = look->sym;
        move();
        return new Type(t);
    } else if(match(KW_STRUCT)) {
        if(firstIs(IDENT)) {
            string structName = ((ID*)look)->name;
            move();
            return new Type(structName);
        } else {
            // 如果是*, 说明声明变量是缺少了结构体类型
            // 如果是{. 说明声明结构体时缺少了结构体名, 而目前不支持匿名结构体
            recovery(firstIs(MUL)_OR_(LBRACE),TYPE_LOST,TYPE_WRONG);
        }
    }
    else{
        recovery(firstIs(MUL)_OR_(IDENT),TYPE_LOST,TYPE_WRONG);
        
    }

    return nullptr;  
}

// <def>    -> { <memberlist> };       # 结构体定义
//          -> <mulss><ID><idtail>
// <mulss>  -> * <mulss>
//          -> e
void Parser::def(bool isExtern, Type* s)
{
    if (match(LBRACE)) {
        vector<Var*> memberList;
        member(memberList);

        if(!match(RBRACE)) {
            recovery(firstIs(SEMICON), RBRACE_LOST, RBRACE_WRONG);
        }

        Type::defStruct(s->getName(), memberList);

        if(!match(SEMICON)) {
            recovery(false, SEMICON_LOST, SEMICON_WRONG);
        }
        
    } else {
        int ptrLevel = 0;
        while(match(MUL)){
            ptrLevel++;
        }

        string name;
        if(firstIs(IDENT)){
            name = ((ID*)look)->name;
            move();
        }  else{
            recovery(firstIs(SEMICON)_OR_(ASSIGN)_OR_(COMMA), ID_LOST,ID_WRONG);
        }

        idtail(isExtern, s, ptrLevel, name);
    }
}

// <idtail>  -> ( <para> ) <funtail>    # 函数声明 / 函数调用
//           -> <defvar><deflist>       # 变量声明
void Parser::idtail(bool isExtern, Type* s, int ptrLevel, string name)
{
    if(match(LPAREN)){
        symtab.enter();
        vector<Var*> paraList;
        para(paraList);
        if(!match(RPAREN)){
            recovery(firstIs(LBRACE)_OR_(SEMICON),RPAREN_LOST,RPAREN_WRONG);
        }
        // 创建函数
        Fun* fun = new Fun(isExtern, s, ptrLevel, name, paraList);
        funtail(fun);
        symtab.leave();
    } else {
        Var* var = defvar(isExtern,s,ptrLevel,name);
        symtab.addVar(var);
        deflist(isExtern,s);
    }
}

// <memberlist> -> <type><paradata>; <membertail> 
void Parser::member(vector<Var*>& members) 
{
    Type* s = type();
    Var* var = paradata(s);
    members.push_back(var);
    
    if(!match(SEMICON)){
        recovery(FIRST_TYPE, SEMICON_LOST, SEMICON_WRONG);
    }

    membertail(members);
}

// <membertail> -> <type><def><membertail>
//              -> e
void Parser::membertail(vector<Var*>& members)
{
    if(FIRST_TYPE) {
        member(members);
    } 
}

// <defvar> -> [ <NUM> ] <initArray>
// <defvar> -> <init>
Var* Parser::defvar(bool isExtern, Type* s, int ptrLevel, string name)
{
    if(match(LBRACK)){
        int len = 0;
        if(firstIs(NUM)){
            len = ((Num*)look)->val;
            move();
        }
        else{
            recovery(firstIs(RBRACK),NUM_LOST,NUM_WRONG);
        }

        if(!match(RBRACK)){
            recovery(firstIs(COMMA)_OR_(SEMICON),RBRACK_LOST,RBRACK_WRONG);
        }

        Var* initVal = initArray();    
        return new Var(symtab.getScopePath(),isExtern, s, ptrLevel,name,len, initVal);
    }

    return init(isExtern,s,ptrLevel,name);
    
}


// <init>    -> = <expr> | = {<initlist>}
// <init>    -> e
Var* Parser::init(bool isExtern, Type* s, int ptrLevel, string name)
{
    Var* init = nullptr;
    if(match(ASSIGN)){
        init = expr();
    }
    return new Var(symtab.getScopePath(),isExtern,s,ptrLevel,name,init);
}

// <initArray>  -> = {<exp><initArraytail>}
Var* Parser::initArray()
{
    if(match(ASSIGN)) {
        if(match(LBRACE)) {
            Var* init = new Var();
            init->setName("<array>");
            init->setLeft(true);
            
            Var* value = expr();
            init->getInitArray().push_back(value);
            init->setType(value->getType());
            initArraytail(init->getInitArray());

            if(!match(RBRACE)) {
                recovery(true, RBRACE_LOST,RBRACE_WRONG);
            }

            return init;
        }
    }
    return nullptr;
}

// <literaltail> -> , <exp><literaltail> | e
void Parser::initArraytail(std::vector<Var*>& initVals)
{
    if(match(COMMA)){
        Var * value = expr();
        initVals.push_back(value);
        initArraytail(initVals);
    }
}

// <deflist> -> ,<def>
// <deflist> -> ;
void Parser::deflist(bool isExtern, Type* s)
{
    if(match(COMMA)){
        def(isExtern,s);
    }
    else if(match(SEMICON)){
        return;
    }
    else{
        if(firstIs(MUL)){
            recovery(true,COMMA_LOST,COMMA_WRONG);
            // 虽然此处缺少逗号,但还是继续编译
            def(isExtern,s);
        }
        else{
            recovery(FIRST_TYPE || FIRST_STATEMENT || firstIs(KW_EXTERN)_OR_(RBRACE),
                SEMICON_LOST,SEMICON_WRONG);
        }
    }
}


// <funtail> -> ;
// <funtail> -> <block>
void Parser::funtail(Fun* fun)
{
    if(match(SEMICON)){
        symtab.decFun(fun);
    }
    else{
       symtab.defFun(fun);
       block();
       symtab.endDefFun();
    }
}


// <para> -> <type><paradata><paralist>
// <para> -> e
void Parser::para(std::vector<Var*>& para)
{
    if(firstIs(RPAREN)){
        return;
    }
    
    Type* s = type();
    Var* var = paradata(s);
    symtab.addVar(var);
    para.push_back(var);
    paralist(para);
    
}

// <paradata> -> <mulss> <ID> <paradatatail>
// <mulss> ->  * <mulss>
// <mulss> -> e
Var* Parser::paradata(Type* s)
{
    string name;
    int ptrLevel = 0;
    while(match(MUL)){
        ptrLevel++;
    }

    if(firstIs(IDENT)){
        name = ((ID*)look) -> name;
        move();
        return paradatatail(s,name,ptrLevel);
    }
    else{
        recovery(firstIs(COMMA)_OR_(RPAREN)_OR_(LBRACK), ID_LOST,ID_WRONG);
    }

    // 无论何种错误,最后都需要返回一个Var*
    return new Var(symtab.getScopePath(),false,s,false,name,nullptr);    
}

// <paradatatail> -> [ <num> ]
// <paradatatail> -> e
Var* Parser::paradatatail(Type* s,string name,int ptrLevel)
{
    if(match(LBRACK)){
        // 函数参数列表中的数组可以没有指定长度, 即使指定长度也忽略
        if(firstIs(NUM)){
            move();
        }

        if(!match(RBRACK)){
            recovery(firstIs(COMMA),RBRACK_LOST,RBRACK_WRONG);
        }
        // 由于数组作为参数传入函数时已经取地址, 因此这里再定义为数组类型, 否则访问时会再次取地址
        // 定义成相应的指针类型即可
        return new Var(symtab.getScopePath(),false,s,ptrLevel+1,name,nullptr);
    }
    else{
        return new Var(symtab.getScopePath(),false,s,ptrLevel,name,nullptr);
    }
}


// <paralist> -> ,<type><paradata><paralist> | , ...
// <paralist> -> e
void Parser::paralist(std::vector<Var*>& para)
{
    if(match(COMMA)){
        if(FIRST_TYPE){
            Type* s = type();
            Var* var = paradata(s);
            symtab.addVar(var);
            para.push_back(var);
            paralist(para);
        } else if(match(POINT) && match(POINT) && match(POINT)) {
            Var* var = new Var();
            var->setName("<vararg>");
            symtab.addVar(var);
            para.push_back(var);
        }
    }
}

// <block>      -> { <subprogram> }
void Parser::block()
{
    if(!match(LBRACE)){
       recovery(FIRST_TYPE||FIRST_STATEMENT||firstIs(RBRACE),LBRACE_LOST,LBRACE_WRONG);
    }

    subprogram();

    if(!match(RBRACE)){
        recovery(FIRST_TYPE||FIRST_STATEMENT||firstIs(KW_EXTERN)_OR_(KW_ELSE)
            _OR_(KW_CASE)_OR_(KW_DEFAULT),RBRACE_LOST,RBRACE_WRONG);
    }

    
}

// <subprogram> -> <localdef><subprogram>
// <subprogram> -> <statement><subprogram>
// <subprogram> -> e
void Parser::subprogram()
{
    if(FIRST_TYPE){
        localdef();
        subprogram();
    }
    else if(FIRST_STATEMENT){
        statement();
        subprogram();
    }
    else{
        return;
    }
}

// <localdef> -> <type><def>
void Parser::localdef()
{
    string name;
    Type* s = type();
    def(false, s);
}

// <statement> -> <altexpr>;
//              | <whilestat> | <forstat> | <dowhilestat>
//              | <ifstat>    | <switchstat>
//              | break;
//              | continue;
//              | return <altexpr>;
void Parser::statement()
{
    switch(look->sym){
    case KW_WHILE:
        whilestat();
        break;
    case KW_FOR:
        forstat();
        break;
    case KW_DO:
        dowhilestat();
        break;
    case KW_IF:
        ifstat();
        break;
    case KW_SWITCH:
        switchstat();
        break;
    case KW_BREAK:
        move();
        ir.genBreak();
        statCheckSemicon();
        break;
    case KW_CONTINUE:
        move();
        ir.genContinue();
        statCheckSemicon();
        break;
    case KW_RETURN:
        move();
        ir.genReturn(altexpr());
        statCheckSemicon();
        break;
    default:
        altexpr();
        statCheckSemicon();
    }
}

// <whilestat>     -> while ( <altexpr> ) <block>
void Parser::whilestat()
{
    symtab.enter();
    InterInst* _while,*_exit;
    ir.genWhileHead(_while,_exit);
    match(KW_WHILE);
    if(!match(LPAREN)){
        recovery(FIRST_EXPR||firstIs(RPAREN),LPAREN_LOST,LPAREN_WRONG);
    }
    Var* cond = altexpr();
    ir.genWhileCond(cond,_exit);
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    block();
    ir.genWhileTail(_while,_exit);
    symtab.leave();
}

// <forsata>       -> for(<forinit><altexpr>;<altexpr>) <block>
void Parser::forstat()
{
    symtab.enter();
    InterInst *_for, *_exit, *_step, *_block;
    match(KW_FOR);
    if(!match(LPAREN)){
        recovery(FIRST_EXPR||firstIs(RPAREN),LPAREN_LOST,LPAREN_WRONG);
    }
    forinit();
    ir.genForHead(_for,_exit);
   
    Var* cond = altexpr();
    ir.genForCondBegin(cond, _step, _block, _exit);
    if(!match(SEMICON)){
        recovery(FIRST_EXPR||firstIs(RPAREN),SEMICON_LOST,SEMICON_WRONG);
    }
    altexpr();    
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    ir.genForCondEnd(_for, _block);
   
    block();
    ir.genForTail(_step, _exit);
   
    symtab.leave();
}


//  <forinit>       -> <localdef> | <altexpr> ;
void Parser::forinit()
{
    if(FIRST_TYPE){
        localdef();
    }
    else{
        altexpr();
        if(!match(SEMICON)){
            recovery(FIRST_EXPR||firstIs(RPAREN),SEMICON_LOST,SEMICON_WRONG);
        }
    }
}


// <dowhilestat>   -> do <block> while ( <altexpr> );
void Parser::dowhilestat()
{
    // do-while循环的条件部分不属于循环体,而其他语句可以将条件合并到循环体中
    symtab.enter();
    match(KW_DO);
    block();
    if(!match(KW_WHILE)){
        recovery(firstIs(LPAREN),WHILE_LOST,WHILE_WRONG);
    }
    symtab.leave();

    if(!match(LPAREN)){
        recovery(FIRST_EXPR||firstIs(RPAREN),LPAREN_LOST,LPAREN_WRONG);
    }
    altexpr();
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    if(!match(SEMICON)){
        recovery(FIRST_TYPE||FIRST_STATEMENT||firstIs(RBRACE),SEMICON_LOST,SEMICON_WRONG);
    }
}


// <ifstat>     -> if ( <expr> ) <block> <iftail>
void Parser::ifstat()
{
    symtab.enter();
    
    match(KW_IF);
    if(!match(LPAREN)){
        recovery(FIRST_EXPR,LPAREN_LOST,LPAREN_WRONG);
    } 

    Var* cond = expr();
    InterInst* _else;
    ir.genIfHead(cond,_else);

    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    block();
    symtab.leave();

    InterInst* _exit;
    ir.genElseHead(_else,_exit);
    
    iftail();

    ir.genElseTail(_exit);
    
}

// <iftail>     -> else <elseifstat> | e
void Parser::iftail()
{
    if(match(KW_ELSE)){
        symtab.enter();
        elseifstat();
        symtab.leave();
    }
}

// <elseifstat> -> <ifstat> | <block>
void Parser::elseifstat()
{
    if(look->sym == KW_IF){
        ifstat();
    }
    else{
        block();
    }
}

// <switchstat> -> switch ( <expr> ) { <casestat> }
void Parser::switchstat()
{
    symtab.enter();
    match(KW_SWITCH);
    if(!match(LPAREN)){
        recovery(FIRST_EXPR,LPAREN_LOST,LPAREN_WRONG);
    } 
    expr();
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    casestat();
    symtab.leave();
}

// <casestat>   -> case <caselable> : <subprogram><casetate>
//               | default: <subprigram>
void Parser::casestat()
{
    if(match(KW_CASE)){
        caselable();
        if(!match(COLON)){
            recovery(FIRST_TYPE||FIRST_EXPR,COLON_LOST,COLON_WRONG);
        }
        subprogram();
        casestat();
    }
    else if(match(KW_DEFAULT)){
        if(!match(COLON)){
            recovery(FIRST_TYPE||FIRST_EXPR,COLON_LOST,COLON_WRONG);
        }
    }
}

// <caselable> -> <literal>
void Parser::caselable()
{
    literal();
}



// <altexpr> -> <expr> <altexprtail> | e
Var* Parser::altexpr()
{
    if(FIRST_EXPR){
        Var* value = expr();
        Var* other = altexprtail();
        return other->isVoid()? value : other;
    }
    else{
        // 空语句,返回特殊的Void变量
        return SymTab::getVoid();
    }
}

// <altexprtail> -> , <exp> <altexptail> | e
Var* Parser::altexprtail()
{
    if(match(COMMA)) {
        Var* value = expr();
        Var* other = altexprtail();
        return other->isVoid()? value : other;
    }
    else {
        return SymTab::getVoid();
    }
}

// <expr> -> <assexpr>
Var* Parser::expr()
{
    return assexpr();
}

// <assexp> -> <orexpr><asstail>
Var* Parser::assexpr()
{
    Var* lval = orexpr();
    return asstail(lval);
}

// <asstail> -> = <orexpr><asstail>
// <asstail> -> e
Var* Parser::asstail(Var* lval)
{
    if(match(ASSIGN)){
        Var* var = orexpr();
        Var* rval = asstail(var);
        // 右结合,在递归之后生成代码
        Var* result = ir.genTwoOp(lval,ASSIGN,rval);
        return result;
    }

    return lval;
}

// <orexpr> -> <andexpr><ortail>
Var* Parser::orexpr()
{
    Var* lval = andexpr();
    return ortail(lval);
}

// <ortail> -> || <andexpr><ortail>
// <ortail> -> e
Var* Parser::ortail(Var* lval)
{
    if(match(OR)){
        InterInst* _else; 
        InterInst* _exit = new InterInst(); 

        Var* tmp = new Var(symtab.getScopePath(),new Type(KW_INT),0);
        symtab.addVar(tmp); 

        // 如果为真，直接短路返回真，否则跳转至else分支执行
        ir.genIfHead(lval, _else);
        symtab.addInst(new InterInst(OP_AS,tmp,SymTab::one));
        symtab.addInst(new InterInst(OP_JMP,_exit));   

        // else分支内执行求值操作
        // 当前表达式的值仅取决与右侧值，并递归计算整个表达式的值
        symtab.addInst(_else); 
        Var* rval = andexpr();
        Var* tail = ortail(rval);
        
        // 将计算值赋予tmp变量，使得无论是否短路，均从tmp返回
        symtab.addInst(new InterInst(OP_AS,tmp,tail));

        // 最后插入exit块的代码
        ir.genElseTail(_exit);

        return tmp;
    }

    return lval;
}

// <andexpr> -> <comexpr><andtail>
Var* Parser::andexpr()
{
    Var* lval = comexpr();
    return andtail(lval);
}

// <andtail> -> && <comexpr><andtail>
// <andtail> -> e
Var* Parser::andtail(Var* lval)
{
    if(match(AND)){
        InterInst* _else; 
        InterInst* _exit = new InterInst(); 

        Var* tmp = new Var(symtab.getScopePath(),new Type(KW_INT),0);
        symtab.addVar(tmp); 

        // 如果为假，直接短路返回假，否则跳转至else分支执行
        Var* nlval = ir.genOneLeftOp(NOT, lval);
        ir.genIfHead(nlval, _else);
        symtab.addInst(new InterInst(OP_AS,tmp,SymTab::zero));
        symtab.addInst(new InterInst(OP_JMP,_exit));   

        // else分支内执行求值操作
        // 当前表达式的值仅取决与右侧值，并递归计算整个表达式的值
        symtab.addInst(_else); 
        Var* rval = comexpr();
        Var* tail = andtail(rval);
        
        // 将计算值赋予tmp变量，使得无论是否短路，均从tmp返回
        symtab.addInst(new InterInst(OP_AS,tmp,tail));

        // 最后插入exit块的代码
        ir.genElseTail(_exit);

        return tmp;
    }

    return lval;
}

// <comexpt> -> <aloexpr><comtail>
Var* Parser::comexpr()
{
    Var* lval = aloexpr();
    return comtail(lval);
}

// <comtail> -> <cmps><aloexpr><comtail>
// <comtail> -> e
Var* Parser::comtail(Var* lval)
{
    // 把cmps的判断内容提前,从而可以简单的判定是否结束递归
    if(firstIs(GE)_OR_(LE)_OR_(GT)_OR_(LT)_OR_(EQU)_OR_(NEQU)){
        Symbol op = cmps();
        Var* rval = aloexpr();
        Var* result = ir.genTwoOp(lval,op,rval);
        return comtail(result);
    }

    return lval;
}

// <cmps>    -> >= | <= | > | < | == | !=
Symbol Parser::cmps()
{
    Symbol op = look->sym;
    move();
    return op;
}

// <aloexpr> -> <item><alotail>
Var* Parser::aloexpr()
{
    Var* lval = item();
    return alotail(lval);
}

// <alotail> -> <adds><item><alotail>
// <alotail> -> e
Var* Parser::alotail(Var* lval)
{
    if(firstIs(ADD)_OR_(SUB)){
        Symbol op = ands();
        Var* rval = item();
        Var* result = ir.genTwoOp(lval,op,rval);
        return alotail(result);
    }

    return lval;
}


// <ands>    -> + | -
Symbol Parser::ands()
{
    Symbol op = look->sym;
    move();
    return op;
}

// <item>     -> <factor><itemtail>
Var* Parser::item()
{
    Var* lval = factor();
    return itemtail(lval);
}

// <itemtail> -> <muls><factor><itemtail>
// <itemtail> -> e
Var* Parser::itemtail(Var* lval)
{
    if(firstIs(MUL)_OR_(DIV)_OR_(MOD)){
        Symbol op = muls();
        Var* rval = factor();
        Var* result = ir.genTwoOp(lval,op,rval);
        return itemtail(result);
    }

    return lval;
}


// <muls> -> * | / | %
Symbol Parser::muls()
{
    Symbol op = look->sym;
    move();
    return op;
}

// <factor> -> <lop><factor>
// <factor> -> <val>
Var* Parser::factor()
{
    if(firstIs(NOT)_OR_(SUB)_OR_(LEA)_OR_(MUL)_OR_(INC)_OR_(DEC)){
        Symbol op = lop();
        Var* val = factor();
        return ir.genOneLeftOp(op,val);
    }
    else{
        return val();
    }
}

// <lop>    -> ! | - | & | * | ++ | --
Symbol Parser::lop()
{
    Symbol op = look->sym;
    move();
    return op;
}

// <val>    -> <elem><rop> 
// <val>    -> <elem><aloexpr>     # 强制类型转换   
Var* Parser::val()
{
    Var* val = elem();
    Symbol op;
    if(firstIs(INC)_OR_(DEC)){
        op = rop();
        return ir.genOneRightOp(val,op);
    }

    if(val->isCastType() && (FIRST_EXPR)) {
        // 强制类型转换后只能接一个aloexpr级别的表达式，以免对符号优先级判定产生错误
        Var* rval = aloexpr();
        return ir.genCastOp(val, rval);
    }

    return val;
}

// <rop>    -> ++ | --                // 后置的++和--
Symbol Parser::rop()
{
    Symbol op = look->sym;
    move();
    return op;
}

// <elem>   -> <ID><idexpr>
//          -> ( <expr> ) | ( <castype> )
//          -> <literal>
//          -> sizeof ( <type> | <elem> )
Var* Parser::elem()
{
    Var* val = nullptr;
    if(firstIs(IDENT)){
        string name = ((ID*)look)->name;
        move();
        val = idexpr(name);
    }
    else if(match(LPAREN)){
        if(FIRST_TYPE) {
            val = castype();
        } else {
            val = expr();
        }
        
        if(!match(RPAREN)){
            recovery(LVAL_OP,RPAREN_LOST,RPAREN_WRONG);
        }
    }
    else if(match(KW_SIZEOF)) {
        if(!match(LPAREN)) {
            recovery(FIRST_TYPE, LPAREN_LOST, LPAREN_WRONG);
        }

        if(FIRST_TYPE) {
            Type* t = type();
            val = new Var(t->getSize());
        } else {
            Var* v  = elem();
            val = new Var(v->getSize());
        }
        
        if(!match(RPAREN)){
            recovery(LVAL_OP,RPAREN_LOST,RPAREN_WRONG);
        }
    }
    else{
        val = literal();
    }

    return val;
}

// <castype> -> <type> <mulss>
Var* Parser::castype()
{
    Type* s = type();
    int ptrLevel = 0;
    while(match(MUL)) {
        ptrLevel++;
    }

    Var* type = new Var();
    type->setType(s);
    type->setPtrLevel(ptrLevel);
    type->setCast(true);
    return type;
}

// <idexpr> -> [ <expr> ] 
//          -> ( <realarg> ) 
//          -> <member><membertail>         
//          -> e
Var* Parser::idexpr(string name)
{
    Var* val = nullptr;
    if(match(LBRACK)){
        //方括号,是数组
        Var* index = expr();

        if(!match(RBRACK)){
            recovery(LVAL_OP,RBRACE_LOST,RBRACE_WRONG);
        }

        Var* array = symtab.getVal(name);
        return ir.genArray(array,index);
    }
    else if(match(LPAREN)){
        // 圆括号,是函数调用
        vector<Var*> args;
        realarg(args);

        if(!match(RPAREN)){
            recovery(LVAL_OP,RPAREN_LOST,RPAREN_WRONG);
        }
        Fun* fun = symtab.getFun(name,args);
        return ir.genCall(fun,args);
    }
    else if(firstIs(POINT)_OR_(ARROW)) {
        Var* base = symtab.getVal(name);
        return member(base);
    }
    else{
        // 没有其他后缀,是普通的变量
        val = symtab.getVal(name);
    }

    return val;
}

// <member> -> .<ID><membertail>    # 成员变量访问
//          -> -><ID><membertail>   # 箭头操作符访问 
Var* Parser::member(Var* base)
{
    bool isArrow = false;
    if(firstIs(ARROW)) {
        isArrow = true;
    } 
    move();

    string member = ((ID*)look)->name;
    move();

    Type* baseType = base->getType();
    Var* memberType = Type::getMember(baseType, member);
    int offset = Type::getOffset(baseType, member);
    Var* tmp = ir.genOffset(base, memberType, offset, isArrow);
    return membertail(tmp);
}

// <membertail> -> <member><membertail>
//              -> e
Var* Parser::membertail(Var* base)
{
    if(firstIs(POINT)_OR_(ARROW)) {
        return member(base);
    }

    return base;
}


// <realarg> -> <arg><arglist>

void Parser::realarg(vector<Var*>& vec)
{
    // 如果读取到右括号,说明没有参数,直接返回即可
    if(firstIs(RPAREN)){
        return;
    }

    Var* a = arg();
    vec.push_back(a);
    arglist(vec);
}

// <arg>     -> <expr>
Var* Parser::arg()
{
    return expr();
}

// <arglist> -> , <arg><arglist>
// <arglist> -> e
void Parser::arglist(vector<Var*>& vec)
{
    if(match(COMMA)){
        vec.push_back(arg());
        arglist(vec);
    }
}

// <literal> -> <NUM> | <CH> | <STR> 
Var* Parser::literal()
{
    Var* val = nullptr;
    if(firstIs(NUM)_OR_(CH)_OR_(STR)){
        val = new Var(look);
        if(firstIs(STR)){
            symtab.addStr(val);
        }
        else{
            symtab.addVar(val);
        }
        move();
    }
    else{
        recovery(RVAL_OP,LITERAL_LOST,LITERAL_WRONG);
    }

    return val;
}


void Parser::recovery(bool cond,SynError lost,SynError wrong)
{
    if(cond){
        Error::synError(lost,look);
    }
    else{
        Error::synError(wrong,look);
        move();
    }
}

void Parser::statCheckSemicon()
{
    if(!match(SEMICON)){
        recovery(FIRST_TYPE||FIRST_STATEMENT||firstIs(RBRACE),SEMICON_LOST,SEMICON_WRONG);
    }
}
