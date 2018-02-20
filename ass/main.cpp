#include <iostream>
#include "common.h"
#include "Scanner.h"
#include "Lexer.h"
#include "Parser.h"
#include "Generator.h"
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
    

    string objFile = "tempobj.o";
    FILE* fpObj = fopen(objFile.c_str(),"w");

    // 初始化模块
    Scanner scanner = Scanner(filename);
    Lexer lex       = Lexer(scanner);
    SymTab tab      = SymTab();
    Parser parser   = Parser(lex,tab);
    Generator gen   = Generator(tab,parser,fpObj);

    parser.analyse();
    cout << "符号表内容如下:" << endl;
    tab.printSymbolTable();
    cout << "\n汇编完成!" << endl;
    return 0;
}

