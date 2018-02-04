#include <iostream>
#include "common.h"
#include "Scanner.h"
#include "Lexer.h"
#include "Parser.h"
#include "Error.h"


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
    Error err       = Error(&scanner);
    Lexer lex       = Lexer(scanner);
    SymTab tab      = SymTab();
    GenIR ir        = GenIR(tab);
    Parser parser   = Parser(lex,tab,ir);

    parser.analyse();
    
    // printf("\n变量表内容如下:\n");
    // tab.printValTab();
    // printf("\n");
    printf("函数表内容如下:\n");
    tab.printFunTab();
    printf("\n");
    printf("字符串表内容如下:\n");
    tab.printStrTab();

    string asmFile = string(filename);
    std::size_t pos = asmFile.rfind(".c");
    if (pos > 0 && pos == asmFile.length() - 2)
    {
        asmFile.replace(pos, 2, ".s");
    }
    else
    {
        asmFile = asmFile + ".s";
    }
    FILE* fpAsm = fopen(asmFile.c_str(),"w");
    tab.toX86(fpAsm);
    fclose(fpAsm);



    printf("\n编译完成!\n");
    printf("%d 错误 %d 警告\n",err.getErrorNum(),err.getWarnNum());
    return 0;
}

