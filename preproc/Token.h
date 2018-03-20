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
    virtual ~Token();
};

class ID: public Token
{
public:
    std::string name;
    ID(std::string n);
    virtual std::string toString();
};