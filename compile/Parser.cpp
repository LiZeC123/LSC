#include "Parser.h"

Parser::Parser(Lexer& lex): lexer(lex) { }

void Parser::analyse()
{
    //TODO
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

