#pragma once
#include <vector>
#include <map>
#include <string>
#include "../ass/elf.h"

using std::string;

class RelInfo
{
public:
    RelInfo(string segName,Elf32_Rel* rel,string relName);
	string segName;      // 重定位目标段
	Elf32_Rel*  rel;     // 重定位信息
	string relName;      // 重定位符号名
};


class ElfFile
{

public:
    void readElf(const string file);

    void printInfo();
private:
    Elf32_Ehdr ehdr;                        // 文件头
    std::vector<Elf32_Phdr*> phdrTab;       // 程序头表
    std::map<string,Elf32_Shdr*> shdrTab;   // 段表
    std::vector<string> shdrNames;          // 段名顺序
    std::map<string,Elf32_Sym*> symTab;     // 符号表
    std::vector<string> symNames;           // 符号名顺序
    std::vector<RelInfo*> relTab;           // 重定位表
    string shstrtab;                        // 段表字符串表
    string strtab;                          // 字符串表


};