#include "common.h"
#include <string>
#include <map>

class Token
{
public:
    Symbol sym;
    Token(Symbol s);   
    virtual std::string toString();
    virtual ~Token();
};

class ID: public Token
{
public:
    std::string name;
    ID(std::string n);
    virtual std::string toString();
};

class Num: public Token
{
public:
    int val;
    Num(int v);
    virtual std::string toString();
};

class Char:public Token
{
public:
    char ch;
    Char(char c);
    virtual std::string toString();
};

class Str:public Token
{
public:
    std::string str;
    Str(std::string s);
    virtual std::string toString();
};



