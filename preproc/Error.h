#pragma once
#include <string>


enum PreError
{
    SOURCE_FILE_FAIL,
    OUTPUT_FILE_FAIL,
    EMPTY_COMMAND,
    ERROR_FILENAME,
    ERROR_COMMAND,
};





class Error
{
public:
    static void preError(PreError code, const std::string& filename,int rol);
};