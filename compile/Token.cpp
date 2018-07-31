#include "Token.h"
#include <sstream>
#include "error.h"

using namespace std;

/*******************************************************************************
                                   词法记号名字
*******************************************************************************/

const char * tokenName[]=
{
	"error",//错误，异常，结束标记等
	"文件结尾",//文件结束
	"标识符",//标识符
	"int","char","void",//数据类型
	"extern",//extern
	"数字","字符","字符串",//字面量
	"!","&",//单目运算 ! - & *
	"+","-","*","/","%%",//算术运算符
	"++","--",
	">",">=","<","<=","==","!=",//比较运算符
	"&&","||",//逻辑运算
	"(",")",//()
	"[","]",//[]
	"{","}",//{}
	",",":",";",//逗号,冒号,分号
	"=",//赋值
	"if","else",//if-else
	"switch","case","default",//swicth-case-deault
	"while","do","for",//循环
	"break","continue","return"//break,continue,return
};

/*******************************************************************************
                                   词法记号
*******************************************************************************/

Token::Token (Symbol t):sym(t) { }

string Token::toString()
{
	return std::string(tokenName[sym]);
}

Token* Token::copy()
{
	return new Token(sym);
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

Token* ID::copy()
{
	return new ID(name);
}

/*******************************************************************************
                                   字符串
*******************************************************************************/

Str::Str (string s):Token(STR),str(s) { }

string Str::toString()
{
	return string("[")+Token::toString()+"]:"+str;
}

Token* Str::copy()
{
	return new Str(str);
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

Token* Num::copy()
{
	return new Num(val);
}

/*******************************************************************************
                                   字符
*******************************************************************************/

Char::Char (char c):Token(CH),ch(c) { }

string Char::toString()
{
	stringstream ss;
	ss<<ch;
	return string("[")+Token::toString()+"]:"+ss.str();
}

Token* Char::copy()
{
	return new Char(ch);
}
