#pragma once
#include "common.h"
#include <string>
#include <map>

class Token
{
public:
    Symbol sym;
    Token(Symbol s);   
    virtual std::string toString();
    virtual Token* copy();
    virtual ~Token();
};

class ID: public Token
{
public:
    std::string name;
    ID(std::string n);
    virtual std::string toString();
    virtual Token* copy();
};

class Num: public Token
{
public:
    int val;
    Num(int v);
    virtual std::string toString();
    virtual Token* copy();
};

class Char:public Token
{
public:
    char ch;
    Char(char c);
    virtual std::string toString();
    virtual Token* copy();
};

class Str:public Token
{
public:
    std::string str;
    Str(std::string s);
    virtual std::string toString();
    virtual Token* copy();
};



