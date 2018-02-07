#include "Token.h"
#include <sstream>

using namespace std;

/*******************************************************************************
                                   词法记号名字
*******************************************************************************/

const char * tokenName[]=
{
  "ERR",                             
  "END",                              
  "IDENT","NUM","STR",                   
  "KW_SEC","KW_GLB","KW_EQU","KW_TIMES",   
  "KW_DB","KW_DW","KW_DD",
  "BR_AL","BR_CL","BR_DL","BR_BL",      
  "BR_AH","BR_CH","BR_DH","BR_BH",
  "DR_EAX","DR_ECX","DR_EDX","DR_EBX",
  "DR_ESP","DR_EBP","DR_ESI","DR_EDI",
  "I_MOV","I_CMP",                  
  "I_ADD","I_SUB",
  "I_AND","I_OR",
  "I_LEA","I_CALL","I_INT","I_RET",
  "I_IMUL","I_IDIV",
  "I_NEG","I_INC","I_DEC",
  "I_JMP","I_JE","I_JNE",
  "I_SETE","I_SETNE","I_SETG","I_SETGE","I_SETL","I_SETLE",
  "I_PUSH","I_POP",
  "ADD","SUB","COMMA","LBRAC","RBRAC","COLON"  
};

/*******************************************************************************
                                   词法记号
*******************************************************************************/

Token::Token (Symbol t):sym(t) { }

string Token::toString()
{
	return std::string(tokenName[sym]);
}

Token::~Token () { }


/*******************************************************************************
                                   标识符
*******************************************************************************/

ID::ID (string n):Token(IDENT),name(n) { }

string ID::toString()
{
	return Token::toString()+name;
}

/*******************************************************************************
                                   字符串
*******************************************************************************/

Str::Str (string s):Token(STR),str(s) { }

string Str::toString()
{
	return string("[")+Token::toString()+"]:"+str;
}

/*******************************************************************************
                                   数字
*******************************************************************************/

Num::Num (int v):Token(NUM),val(v) { }

string Num::toString()
{
	stringstream ss;
	ss<<val;
	return string("[")+Token::toString()+"]:"+ss.str();
}