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
    

    string objtmp = "tempobj.tmp";
    FILE* fptmp = fopen(objtmp.c_str(),"w");
    
    string objFile = string(filename);
    std::size_t pos = objFile.rfind(".s");
    if (pos > 0 && pos == objFile.length() - 2)
    {
        objFile.replace(pos, 2, ".o");
    }
    else
    {
        objFile = objFile + ".o";
    }
    FILE* fpObj = fopen(objFile.c_str(),"w");

    // 初始化模块
    Scanner scanner = Scanner(filename);
    Lexer lex       = Lexer(scanner);
    SymTab tab      = SymTab();
    Parser parser   = Parser(lex,tab);
    Generator gen   = Generator(tab,parser,fptmp);

    parser.analyse();

    cout << "符号表内容如下:" << endl;
    tab.printSymbolTable();

    fclose(fptmp);
    fptmp = fopen(objtmp.c_str(),"r");
    tab.writeToFile(fptmp,fpObj);
    
    cout << "\n汇编完成!" << endl;
    return 0;
}

