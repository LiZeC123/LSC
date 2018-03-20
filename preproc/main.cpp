#include <iostream>
#include "Scanner.h"
#include "Lexer.h"


using namespace std;
int main()
{
    const char* filename = "./preproc/test.c";
    Scanner scanner = Scanner(filename);
    Lexer lex = Lexer(scanner);

    while(true){
        Token* token = lex.nextToken();
        if(token->sym != END){
            cout << token->toString() << endl;
        }
        else {
            break;
        }
    }

    return 0;
}