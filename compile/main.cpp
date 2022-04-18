#include <iostream>

#include "Error.h"
#include "Lexer.h"
#include "Parser.h"
#include "Scanner.h"
#include "common.h"

//全局变量初始化为默认值, 非全局变量调用默认构造函数, 因此所有变量都需要初始化
class Args {
 public:
  bool printValTab = false;
  bool printFunTab = false;
  bool printStrTab = false;
  bool printStruct = false;
  bool printToken = false;
  bool printStats = false;
  bool printDFG = false;
  bool optimize = false;
};

Args analyseOptions(int argc, char* argv[]) {
  Args args;
  for (int i = 2; i < argc; i++) {
    string option(argv[i]);
    if (option == "--printValTab") {
      args.printValTab = true;
    } else if (option == "--printFunTab") {
      args.printFunTab = true;
    } else if (option == "--printStrTab") {
      args.printStrTab = true;
    } else if (option == "--printTokenStream") {
      args.printToken = true;
    } else if (option == "--stat") {
      args.printStats = true;
    } else if (option == "--printStruct") {
      args.printStruct = true;
    } else if (option == "--printDFG") {
      args.printDFG = true;
    } else if (option == "-O2") {
      args.optimize = true;
    }
  }

  return args;
}

void printTables(Args args, SymTab& tab) {
  if (args.printValTab) {
    printf("\n变量表内容如下:\n");
    tab.printValTab();
  }

  if (args.printFunTab) {
    printf("\n函数表内容如下:\n");
    tab.printFunTab();
  }

  if (args.printStrTab) {
    printf("\n字符串表内容如下:\n");
    tab.printStrTab();
  }

  if (args.printStruct) {
    printf("\n结构体表内容如下:\n");
    Type::printStruct();
  }
}

void optimize(Args args, SymTab& tab) {
  if (args.optimize) {
    tab.optimize();
  }
}

void genFile(const char* filename, SymTab& tab) {
  string asmFile = string(filename);
  std::size_t ipos = asmFile.rfind(".i");
  std::size_t cpos = asmFile.rfind(".c");

  if (ipos > 0 && ipos == asmFile.length() - 2) {
    asmFile.replace(ipos, 2, ".s");
  } else if (cpos > 0 && cpos == asmFile.length() - 2) {
    asmFile.replace(cpos, 2, ".s");
  } else {
    asmFile = asmFile + ".s";
  }

  FILE* fpAsm = fopen(asmFile.c_str(), "w");
  tab.toX86(fpAsm);
  fclose(fpAsm);
}

int main(int argc, char* argv[]) {
  // 读取需要编译的文件名
  const char* filename;

  if (argc >= 2) {
    filename = argv[1];
  } else {
    printf("lscc: 输入文件不能为空");
    return 0;
  }

  Args args = analyseOptions(argc, argv);

  // 初始化模块
  Scanner scanner(filename);
  Error err(&scanner);
  Lexer lex(&scanner);
  SymTab tab;
  GenIR ir(tab);
  Parser parser(lex, tab, ir);

  if (args.printToken) {
    parser.setPrintToken();
  }

  try {
    // 分析代码
    parser.analyse();

    // 根据需求输出中间结果
    printTables(args, tab);

    // 根据需求执行代码优化
    optimize(args, tab);

    // 生成汇编代码文件
    genFile(filename, tab);
  } catch (std::runtime_error& e) {
    printf("%s\n", e.what());
    printf("compilation terminated.\n");
  }

  if (err.getErrorNum() > 0 && args.printStats) {
    printf("\n编译完成!\n");
    printf("%d 错误 %d 警告\n", err.getErrorNum(), err.getWarnNum());
  }

  return err.getErrorNum() + err.getWarnNum();
}
