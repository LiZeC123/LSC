#include <iostream>
#include "common.h"
#include "Scanner.h"
#include "Lexer.h"
#include "Parser.h"
#include "Error.h"

//全局变量初始化为默认值, 非全局变量调用默认构造函数, 因此所有变量都需要初始化
class Args{
public:
    bool printValTab = false;
    bool printFunTab = false;
    bool printStrTab = false;
    bool printToken  = false;
};

Args analyseOptions(int argc, char* argv[])
{
    Args args;
    for(int i=2; i < argc; i++){
        string option(argv[i]);
        if(option == "--printValTab"){
            args.printValTab = true;
        }
        else if(option == "--printFunTab"){
            args.printFunTab = true;
        }
        else if(option == "--printStrTab"){
            args.printStrTab = true;
        }
        else if(option == "--printTokenStream"){
            args.printToken = true;
        }
    }

    return args;
}

void printTables(Args args,SymTab tab)
{
    if(args.printValTab){
        printf("\n变量表内容如下:\n");
        tab.printValTab();
    }
    else if(args.printFunTab){
        printf("函数表内容如下:\n");
        tab.printFunTab();
    }
    else if(args.printStrTab){
        printf("字符串表内容如下:\n");
        tab.printStrTab();
    }
}

void genFile(const char* filename, SymTab tab)
{
    string asmFile = string(filename);
    std::size_t ipos = asmFile.rfind(".i");
    std::size_t cpos = asmFile.rfind(".c");
   
    if (ipos>0 && ipos==asmFile.length()-2)
    {
        asmFile.replace(ipos, 2, ".s");
    }
    else if(cpos>0 && cpos==asmFile.length()-2) 
    {
        asmFile.replace(cpos, 2, ".s");
    }
    else
    {
        asmFile = asmFile + ".s";
    }

    FILE* fpAsm = fopen(asmFile.c_str(),"w");
    tab.toX86(fpAsm);
    fclose(fpAsm);
}

int main(int argc,char* argv[])
{
    // 读取需要编译的文件名
    const char* filename;

    if(argc >= 2){
        filename = argv[1];
    }
    else{
        printf("lscc: 输入文件不能为空");
        return 0;
    }

    Args args = analyseOptions(argc,argv);
    
    // 初始化模块
    Scanner scanner = Scanner(filename);
    Error err       = Error(&scanner);
    Lexer lex       = Lexer(&scanner);
    SymTab tab      = SymTab();
    GenIR ir        = GenIR(tab);
    Parser parser   = Parser(lex,tab,ir);

    if(args.printToken){
        parser.setPrintToken();
    }

    // 开始分析
    parser.analyse();

  
    // 根据需求输出中间结果
    printTables(args,tab);

    
    // 生成汇编代码文件
    genFile(filename,tab);

    if(err.getErrorNum() > 0){
        printf("\n编译完成!\n");
        printf("%d 错误 %d 警告\n",err.getErrorNum(),err.getWarnNum());
    }
    
    return 0;
}

