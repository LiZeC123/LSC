#pragma once
#include <map>
#include <string>
#include <vector>

#include "Scanner.h"
#include "Token.h"
#include "common.h"

/*******************************************************************************
                                   关键字表
*******************************************************************************/
class KeyWords {
 public:
  KeyWords();
  Symbol getSym(std::string name);

 private:
  std::map<std::string, Symbol> keywords;
};

/*******************************************************************************
                                    宏名表
*******************************************************************************/
class Macros {
 public:
  Macros(Scanner& sc);
  Macros(const Macros&) = delete;
  Macros& operator=(const Macros&) = delete;

  bool isMacro(std::string name);
  std::vector<Token*>* getRealToken(std::string name);
  void addMacro(std::string name, std::vector<Token*>* list);

 private:
  Scanner& scan;
  std::map<std::string, std::vector<Token*>*> macros;

  std::vector<Token*>* last;  // 保存动态分配的宏, 例如 __LINE__
  void expand();
};

/*******************************************************************************
                                   词法分析器
*******************************************************************************/
class Lexer {
 public:
  Lexer(Scanner* scanner);
  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;
  Token* nextToken();

 private:
  static KeyWords key;
  Scanner* scanner;
  Macros macros;
  char ch;
  bool scan(char need = 0);

  Token* token;

  // 用于宏替换
  unsigned int macroTokenIndex;
  std::vector<Token*>* macroTokenList;

  std::vector<Scanner*> scanStack;  //保存scanner的顺序的栈,用于include指令

 private:
  Token* getIdent();
  Token* getStr();
  Token* getChar();
  Token* getNum();
  Token* readToken();
  Token* nextMacroToken();
  void getMacro();

  Scanner* loadIncludeFile();
  Scanner* includeStdFile(std::vector<Token*> words);
  Scanner* includeUserFile(std::string name);
};