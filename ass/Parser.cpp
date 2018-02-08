#include "Parser.h"
#include "Token.h"

#define firstIs(C) look->sym == C       // 判断向前读取的一个符号是不是指定的符号
#define _OR_(T) || look->sym == T       // 与上面的宏连用,组成更复杂的条件语句

using namespace std;

Parser::Parser(Lexer& lex,SymTab& tab) : 
    lexer(lex),
    symtab(tab)
{ }

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

// <program> -> section <IDENT> <program>
// <program> -> global <IDENT> <program>
// <program> -> <IDENT> <lbtail> <program>
// <program> -> <inst> <program>
// <program> -> e
void Parser::program()
{
    switch (look->sym){
    case END:
        return;
    case KW_SEC:
        move();
        match(IDENT);
        break;
    case KW_GLB:
        move();
        match(IDENT);
        break;
    case IDENT:
        lbtail( ((ID *)look)->name );
        break;
    default:
        inst();
    }

    program();
}


// <lbtail> -> COLON
// <lbtail> -> equ <NUM>
// <lbtail> -> <basetail>
// <lbtail> -> times <NUM> <basetail>
void Parser::lbtail(string name)
{
    move();
    switch(look->sym){
    case COLON:
        symtab.addLabel(new Label(name));
        move();
        break;
    case KW_EQU:
        move();
        symtab.addLabel(new Label(name, ((Num *)look)->val));
        move();
        break;
    case KW_TIMES:
        move();
        basetail(name, ((Num *)look)->val);
        break;
    default:
        basetail(name, 1);
    }
}

// <basetail> -> <len> <value>
void Parser::basetail(string name,int times)
{
    move();
    int size = len();
    value(name,times,size);
}

// <len> -> db | dw | dd
int Parser::len()
{
    switch(look->sym){
    case KW_DB:
        return 1;
    case KW_DW:
        return 2;
    case KW_DD:
        return 4;
    default:
        return 0;
    }
}

// <value> -> <type> <valtail>
void Parser::value(string name,int times,int size)
{
    move();
    vector<int> cont;
    type(cont,size);
    valtail(cont,size);
    symtab.addLabel(new Label(name,times,size,cont));
}

// <type> -> <NUM> | <off> <NUM> | STR | IDENT
void Parser::type(std::vector<int>& cont,int size)
{
    switch (look->sym){
    case NUM:
        cont.push_back(((Num *)look)->val);
        break;
    case STR:
        for(int i=0;i<((Str*)look)->str.size();i++){
            cont.push_back(((Str*)look)->str[i]);
        }
        break;
    case IDENT:
        cont.push_back(
            symtab.getLabel(
                ((Str*)look)->str
            )->getAddr()
        );
        break;
    default:
        break;
    }
    move();
}

// <valtail> -> , <type><valtail> | e
void Parser::valtail(std::vector<int>& cont,int size)
{
    if (match(COMMA)){
        type(cont, size);
        valtail(cont, size);
    }
}

void Parser::inst()
{
    // TODO:
}