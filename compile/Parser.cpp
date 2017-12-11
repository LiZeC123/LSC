#include "Parser.h"
#include "Error.h"

#define firstIs(C) look->sym == C       // 判断向前读取的一个符号是不是指定的符号
#define _OR_(T) || look->sym == T       // 与上面的宏连用,组成更复杂的条件语句

//类型的First集
#define FIRST_TYPE firstIs(KW_INT)_OR_(KW_CHAR)_OR_(KW_VOID)
//表达式First集
#define FIRST_EXPR firstIs(LPAREN)_OR_(NUM)_OR_(CH)_OR_(STR)_OR_(IDENT)_OR_(NOT)         \
_OR_(SUB)_OR_(LEA)_OR_(MUL)_OR_(INC)_OR_(DEC)
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

Parser::Parser(Lexer& lex,SymTab& tab) : lexer(lex),symtab(tab) { }

void Parser::analyse()
{
    move();
    program();
}

void Parser::move()
{
    look = lexer.nextToken();
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
    Symbol s = type();
    def(isExtern,s);
}

// <type>    -> int | void | char 
Symbol Parser::type()
{
    Symbol temp = KW_INT;
    if(FIRST_TYPE){
        temp = look->sym;
        move();
        return temp;
    }
    else{
        recovery(firstIs(MUL)_OR_(IDENT),TYPE_LOST,TYPE_WRONG);
        return KW_INT;  // 错误情况,返回int
    }
}

// <def>     -> <ID> <idtail>
// <def>     -> * <ID> <idtail>
void Parser::def(bool isExtern,Symbol s)
{
    string name;
    bool isPtr = match(MUL);

    if(firstIs(IDENT)){
        name = ((ID*)look)->name;
        move();
        idtail(isExtern,s,isPtr,name);
    }
    else{
        recovery(firstIs(SEMICON)_OR_(ASSIGN)_OR_(COMMA), ID_LOST,ID_WRONG);
    }
}

// <idtail>  -> <defvar><deflist>
// <idtail>  -> ( <para> ) <funtail>
void Parser::idtail(bool isExtern,Symbol s,bool isPtr,std::string name)
{
    if(match(LPAREN)){
        symtab.enter();
        vector<Var*> paraList;
        para(paraList);
        if(!match(RPAREN)){
            recovery(firstIs(LBRACE)_OR_(SEMICON),RPAREN_LOST,RPAREN_WRONG);
        }
        // 创建函数
        Fun* fun = new Fun(isExtern,s,name,paraList);
        funtail(fun);
        symtab.leave();
    }
    else{
        Var* var = defvar(isExtern,s,isPtr,name);
        symtab.addVar(var);

        deflist(isExtern,s);
    }
}

// <defvar> -> [ <NUM> ]
// <defvar> -> <init>
Var* Parser::defvar(bool isExtern,Symbol s,bool isPtr,std::string name)
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
            recovery(firstIs(COMMA),RBRACK_LOST,RBRACK_WRONG);
        }

        return new Var(symtab.getScopePath(),isExtern, s, isPtr,name,len);
    }
    else{
        return init(isExtern,s,isPtr,name);
    }
}


// <init>    -> = <expr>
// <init>    -> e
Var* Parser::init(bool isExtern,Symbol s,bool isPtr,std::string name)
{
    Var* init = nullptr;
    if(match(ASSIGN)){
        init = expr();
    }
    return new Var(symtab.getScopePath(),isExtern,s,isPtr,name,init);
}


// <deflist> -> ,<def>
// <deflist> -> ;
void Parser::deflist(bool isExtern,Symbol s)
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
       symtab.endDefFun(fun);
    }
}


// <para> -> <type><paradata><paralist>
// <para> -> e
void Parser::para(std::vector<Var*>& para)
{
    if(firstIs(RPAREN)){
        return;
    }
    
    Symbol s = type();
    Var* var = paradata(s);
    symtab.addVar(var);
    para.push_back(var);
    paralist(para);
    
}

// <paradata> -> * <ID>
// <paradata> -> <ID> <paradatatail>
Var* Parser::paradata(Symbol s)
{
    string name;
    if(match(MUL)){
        if(firstIs(IDENT)){
            name = ((ID*)look) -> name;
            move();
            return new Var(symtab.getScopePath(),false,s,true,name,nullptr);
        }
        else{
            recovery(firstIs(COMMA)_OR_(RPAREN), ID_LOST,ID_WRONG);
        }
    }
    else{
        if(firstIs(IDENT)){
            name = ((ID*)look) -> name;
            move();
            return paradatatail(s,name);
        }
        else{
            recovery(firstIs(COMMA)_OR_(RPAREN)_OR_(LBRACK), ID_LOST,ID_WRONG);
        }
    }
    // 无论何种错误,最后都需要返回一个Var*
    return new Var(symtab.getScopePath(),false,s,false,name,nullptr);
}

// <paradatatail> -> [ <num> ]
// <paradatatail> -> e
Var* Parser::paradatatail(Symbol s,string name)
{
    if(match(LBRACK)){
        // 函数参数列表中的数组可以没有指定长度
        // 所以不需要错误处理,并且以非0数值作为初始值(否在,在写入符号表是会报错)
        int len = 1;
        if(firstIs(NUM)){
            len = ((Num*)look)->val;
            move();
        }

        if(!match(RBRACK)){
            recovery(firstIs(COMMA),RBRACK_LOST,RBRACK_WRONG);
        }

        return new Var(symtab.getScopePath(),false,s,false,name,len);
    }
    else{
        return new Var(symtab.getScopePath(),false,s,false,name,nullptr);
    }
}


// <paralist> -> ,<paradata><paralist>
// <paralist> -> e
void Parser::paralist(std::vector<Var*>& para)
{
    if(match(COMMA)){
        Symbol s = type();
        Var* var = paradata(s);
        symtab.addVar(var);
        para.push_back(var);
        paralist(para);
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
    Symbol s = type();
    def(false,s);
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
        statCheckSemicon();
        break;
    case KW_CONTINUE:
        move();
        statCheckSemicon();
        break;
    case KW_RETURN:
        move();
        altexpr();
        statCheckSemicon();
    default:
        altexpr();
        statCheckSemicon();
    }
}

// <whilestat>     -> while ( <altexpr> ) <block>
void Parser::whilestat()
{
    match(KW_WHILE);
    if(!match(LPAREN)){
        recovery(FIRST_EXPR||firstIs(RPAREN),LPAREN_LOST,LPAREN_WRONG);
    }
    altexpr();
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    block();

}

// <forsata>       -> for(<forinit><altexpr>;<altexpr>) <block>
void Parser::forstat()
{
    match(KW_FOR);
    if(!match(LPAREN)){
        recovery(FIRST_EXPR||firstIs(RPAREN),LPAREN_LOST,LPAREN_WRONG);
    }
    forinit();
    altexpr();
    if(!match(SEMICON)){
        recovery(FIRST_EXPR||firstIs(RPAREN),SEMICON_LOST,SEMICON_WRONG);
    }
    altexpr();    
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    block();

}


//  <forinit>       -> <localdef> | <altexpr> ;
void Parser::forinit()
{
    if(FIRST_TYPE){
        localdef();
    }
    else{
        altexpr();
    }
}


// <dowhilestat>   -> do <block> while ( <altexpr> );
void Parser::dowhilestat()
{
    match(KW_DO);
    block();
    if(!match(KW_WHILE)){
        recovery(firstIs(LPAREN),WHILE_LOST,WHILE_WRONG);
    }
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    if(!match(SEMICON)){
        recovery(FIRST_TYPE||FIRST_STATEMENT||firstIs(RBRACE),SEMICON_LOST,SEMICON_WRONG);
    }
}


// <ifstat>     -> if ( <expr> ) <block> <elsestat>
void Parser::ifstat()
{
    match(KW_IF);
    if(!match(LPAREN)){
        recovery(FIRST_EXPR,LPAREN_LOST,LPAREN_WRONG);
    } 
    expr();
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    block();
    elsestat();
}

// <elsestat>   -> else <block> | e
void Parser::elsestat()
{
    if(match(KW_ELSE)){
        block();
    }
}

// <switchstat> -> switch ( <expr> ) { <casestat> }
void Parser::switchstat()
{
    match(KW_SWITCH);
    if(!match(LPAREN)){
        recovery(FIRST_EXPR,LPAREN_LOST,LPAREN_WRONG);
    } 
    expr();
    if(!match(RPAREN)){
        recovery(firstIs(LBRACE),RPAREN_LOST,RPAREN_WRONG);
    }
    casestat();
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



// <altexpr> -> <expr>
// <altexpr> -> e
Var* Parser::altexpr()
{
    if(FIRST_EXPR){
        return expr();
    }
    else{
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
        // TODO: 代码生成
        return rval;
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
        Var* var = andexpr();
        // 左结合,在递归之前生成代码
        return ortail(lval);
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
        Var* var = comexpr();

        return andtail(lval);
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
         Var* var = aloexpr();

         return comtail(lval);
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
        Var* val = item();

        return alotail(lval);
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
        Var* val = factor();

        return itemtail(lval);
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

        return val;
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
Var* Parser::val()
{
    Var* val = elem();
    Symbol op;
    if(firstIs(INC)_OR_(DEC)){
        op = rop();
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
// <elem>   -> ( <expr> )
// <elem>   -> <literal>
Var* Parser::elem()
{
    Var* val = nullptr;
    if(firstIs(IDENT)){
        string name = ((ID*)look)->name;
        move();
        val = idexpr(name);
    }
    else if(match(LPAREN)){
        val = expr();
        
        if(!match(RPAREN)){
            recovery(LVAL_OP,RPAREN_LOST,RPAREN_WRONG);
        }
    }
    else{
        val = literal();
    }

    return val;
}

// <idexpr> -> [ <expr> ] | ( <realarg> ) | e
Var* Parser::idexpr(string name)
{
    Var* val = nullptr;
    if(match(LBRACK)){
        Var* index = expr();

        if(!match(RBRACK)){
            recovery(LVAL_OP,RBRACE_LOST,RBRACE_WRONG);
        }
        val = symtab.getVal(name);
        // 后续生成数组的相关代码
    }
    else if(match(LPAREN)){
        vector<Var*> args;
        realarg(args);

        if(!match(RPAREN)){
            recovery(LVAL_OP,RPAREN_LOST,RPAREN_WRONG);
        }
        // 后续产生函数调用的代码
    }
    else{
        // 没有其他后缀,是普通的变量
        val = symtab.getVal(name);
    }

    return val;
}

// <realarg> -> <arg><arglist>

void Parser::realarg(vector<Var*>& vec)
{
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
    }
}

void Parser::statCheckSemicon()
{
    if(!match(SEMICON)){
        recovery(FIRST_TYPE||FIRST_STATEMENT||firstIs(RBRACE),SEMICON_LOST,SEMICON_WRONG);
    }
}
