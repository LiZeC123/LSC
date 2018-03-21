#include <iostream>
#include "Scanner.h"
#include "Lexer.h"


using namespace std;
int main()
{
    const char* filename = "./test/testMacro.c";
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

//BUG: 由于include指令也可以使用"",因此需要正确的处理字符串,或者直接取消使用""