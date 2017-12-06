#include "Parser.h"
#include "Error.h"

#define firstIs(C) look->sym == C       // 判断向前读取的一个符号是不是指定的符号
#define _OR_(T) || look->sym == T       // 与上面的宏连用,组成更复杂的条件语句

//类型的First集
#define FIRST_TYPE firstIs(KW_INT)_OR_(KW_CHAR)_OR_(KW_VOID)
//表达式First集
#define FIRST_EXPR firstIs(LPAREN)_OR_(NUM)_OR_(CH)_OR_(STR)_OR_(IDENT)_OR_(NOT)         \
_OR_(SUB)_OR_(LEA)_OR_(MUL)_OR_(INC)_OR_(DEC)
//语句First集
#define FIRST_STATEMENT (FIRST_EXPR)_OR_(SEMICON)_OR_(KW_WHILE)_OR_(KW_FOR)              \
_OR_(KW_DO)_OR_(KW_IF)_OR_(KW_SWITCH)_OR_(KW_RETURN)_OR_(KW_BREAK)_OR_(KW_CONTINUE)

using namespace std;

Parser::Parser(Lexer& lex): lexer(lex) { }

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
        recover(firstIs(MUL)_OR_(IDENT),TYPE_LOST,TYPE_WRONG);
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
        recover(firstIs(SEMICON)_OR_(ASSIGN)_OR_(COMMA), ID_LOST,ID_WRONG);
    }
}

// <idtail>  -> <defvar><deflist>
// <idtail>  -> ( <para> ) <funtail>
void Parser::idtail(bool isExtern,Symbol s,bool isPtr,std::string name)
{
    if(match(LPAREN)){
        //TODO: function
    }
    else{
        //TODO: 存符号表
        defvar(isExtern,s,isPtr,name);
        deflist(isExtern,s);
        //TODO: 处理后续声明
    }
}

// <defvar> -> [ <NUM> ]
// <defvar> -> <init>
Var* Parser::defvar(bool isExtern,Symbol s,bool isPtr,std::string name)
{
    if(match(LBRACE)){
        int len = 0;
        if(firstIs(NUM)){
            len = ((Num*)look)->val;
            move();
        }
        else{
            recover(firstIs(RBRACE),NUM_LOST,NUM_WRONG);
        }

        if(!match(RBRACE)){
            recover(firstIs(COMMA),RBRACE_LOST,RBRACE_WRONG);
        }

        // TODO: 返回符号
    }
    else{
        return init(isExtern,s,isPtr,name);
    }
}


// <init>    -> = <expr>
// <init>    -> e
Var* Parser::init(bool isExtern,Symbol s,bool isPtr,std::string name)
{
    if(match(ASSIGN)){
        //TODO 调用expr()
    }
    //TODO: 返回变量
}


// <deflist> -> ,<defvar><deflist>
// <deflist> -> ;
void Parser::deflist(bool isExtern,Symbol s)
{
    if(match(COMMA)){
        def(isExtern,s);
        deflist(isExtern,s);
    }
    else if(match(SEMICON)){
        return;
    }
    else{
        if(firstIs(MUL)){
            recover(true,COMMA_LOST,COMMA_WRONG);
            //TODO: 处理符号

            // 虽然此处缺少来逗号,但还是继续编译
            deflist(isExtern,s);
        }
        else{
            recover(FIRST_TYPE || FIRST_STATEMENT || firstIs(KW_EXTERN)_OR_(RBRACE),
                SEMICON_LOST,SEMICON_WRONG);
        }
    }
}


// <funtail> -> ;
// <funtail> -> <block>




void Parser::recover(bool cond,SynError lost,SynError wrong)
{
    if(cond){
        Error::synError(lost,look);
    }
    else{
        Error::synError(wrong,look);
    }
}
