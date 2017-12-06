#pragma once
#include "Scanner.h"
#include "Token.h"
#include <string>

class Error
{
public:
    Error(Scanner * scanner);
    int getWarnNum();
    int getErrorNum();


    static void lexError(int code);
    static void lexWarm(int code);
    static void synError(int code,Token* token);
    static void semError(int code,std::string name);

private:
    static Scanner * scanner;

    static int warnNum;
    static int errorNum;
};