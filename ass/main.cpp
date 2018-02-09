#include <iostream>
#include "common.h"
#include "Scanner.h"
#include "Lexer.h"
#include <string>
using namespace std;

int main(int argc,char* argv[])
{
    // 读取需要编译的文件名
    const char* filename;
    if(argc == 2){
        filename = argv[1];
    }
    else{
        return 0;
    }
    
    // 初始化模块
    Scanner scanner = Scanner(filename);
    Lexer lex       = Lexer(scanner);

    Token* token = lex.nextToken();

    while(token->sym != END){
        cout << token->toString() << endl;
        token = lex.nextToken();
    }
   


    printf("\n汇编完成!\n");
    return 0;
}

