#include <iostream>
#include "common.h"
#include "Scanner.h"
#include "Lexer.h"
#include "Parser.h"
#include "Error.h"


int main()
{
    const char* filename = "/home/lizec/CWorkSpace/lsc/test/baseTest.c";
    Scanner scanner = Scanner(filename);
    Error err     = Error(&scanner);
    Lexer lex       = Lexer(scanner);
    SymTab tab      = SymTab();
    Parser parser   = Parser(lex,tab);

    parser.analyse();
    printf("变量表内容如下:\n");
    tab.printValTab();
    printf("\n");
    printf("函数表内容如下:\n");
    tab.printFunTab();

    printf("编译完成!\n");
    printf("%d 错误 %d 警告\n",err.getErrorNum(),err.getWarnNum());
    return 0;
}

