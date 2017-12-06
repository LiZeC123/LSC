#pragma once
#include "Scanner.h"
#include "Token.h"

class Error
{
public:
    Error(Scanner * scanner);
    int getWarnNum();
    int getErrorNUm();


    static void lexError(int code);
    static void lexWarm(int code);
    static void synError(int code,Token* token);

private:
    static Scanner * scanner;

    static int warnNum;
    static int errorNum;
};