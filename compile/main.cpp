#include <iostream>
#include "common.h"
#include "Scanner.h"
#include "Lexer.h"


int main()
{
    const char* filename = "/home/lizec/CWorkSpace/lsc/test/main.c";
    Scanner scanner(filename);
    Lexer lex(scanner);

    Token * token;
    while(true){
        token = lex.nextToken();
        std::cout << token->toString() << std::endl;
        if(token->sym == END){
            break;
        } 
   }

    return 0;
}

