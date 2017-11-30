#pragma once
#include "common.h"
#include "Token.h"
#include "Lexer.h"

class Parser
{
public:
    Parser(Lexer& lex);

    void analyse();


private:
    Token* look;
    Lexer& lexer;

private:
    void move();
    bool match(Symbol need);

};