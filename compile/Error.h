#pragma once
#include "Scanner.h"

class Error
{
public:
    Error(Scanner * scanner);
    int getWarnNum();
    int getErrorNUm();


    static void lexError(int code);
    static void lexWarm(int code);

private:
    static Scanner * scanner;

    static int warnNum;
    static int errorNum;
};