#include "common.h"
#include "Scanner.h"
#include "Lexer.h"
#include "Parser.h"
#include "Generator.h"
#include <string>
using namespace std;

class Args
{
public:
    bool printSymbol = false;
};

Args analyseOptions(int argc, char* argv[])
{
    Args args;
    for(int i=2; i < argc; i++){
        string option(argv[i]);
        if(option == "--printSymbolTable"){
            args.printSymbol = true;
        }
    }

    return args;
}

void printInfo(Args& args, SymTab& tab)
{
    if(args.printSymbol){
        tab.printSymbolTable();
    }
}


int main(int argc,char* argv[])
{
    // 读取需要编译的文件名
    const char* filename;
    if(argc >= 2){
        filename = argv[1];
    }
    else{
        printf("lsca: 输入文件不能为空");
        return 0;
    }
    
    Args args = analyseOptions(argc,argv);

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
    Scanner scanner(filename);
    Lexer lex(scanner);
    SymTab tab;
    Parser parser(lex,tab,fptmp);

    parser.analyse();

    printInfo(args,tab);

    fclose(fptmp);
    fptmp = fopen(objtmp.c_str(),"r");
    tab.writeToFile(fptmp,fpObj);

    fclose(fptmp);
    fclose(fpObj);

    remove(objtmp.c_str());     // 汇编完成,删除临时文件

    return 0;
}

