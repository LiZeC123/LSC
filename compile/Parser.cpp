#include "Parser.h"
#include "Error.h"

#define firstIs(C) look->sym == C       // 判断向前读取的一个符号是不是指定的符号
#define _OR_(T) || look->sym == T       // 与上面的宏连用,组成更复杂的条件语句

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
    match(KW_EXTERN);
    type();
    def();
}

// <type>    -> int | void | char 
Symbol Parser::type()
{
    //TODO
}

void Parser::def()
{
    //TODO
}


void Parser::recover(bool cond,SynError lost,SynError wrong)
{
    if(cond){
        Error::synError(lost,look);
    }
    else{
        Error::synError(wrong,look);
    }
}
