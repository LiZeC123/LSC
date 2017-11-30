#include "Parser.h"

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

