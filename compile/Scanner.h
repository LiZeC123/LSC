#pragma once
#include <stdio.h>
/*******************************************************************************
                                   扫描器
*******************************************************************************/
class Scanner
{
public:
    Scanner(const char* name);
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;
    ~Scanner();

    int scan();                 // 扫描获得一个字符 
    const char* getFilename();  // 获得当前文件名
    int getRow();               // 获得当前行号
    int getCol();               // 获得当前列号

private:
    const char * filename;
    FILE* fin;

    int lineNum = 1;      // 行号
    int colNum = 0;       // 列号

    int lineLen = 0;                // 当前行长度
    int readPos = -1;               // 当前字符的位置
    static const int BUF_LEN= 80;   // 缓冲区最大长度
    char line[BUF_LEN];             // 字符缓冲区
    
    char ch = 0;        // 当前字符
    char lastch;        // 上一个字符

    void showChar();

};