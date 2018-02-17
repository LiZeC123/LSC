#pragma once
#include <string>
#include <vector>
#include <map>
#include "elf.h"
#include "Label.h"

using std::string;

class RelInfo
{
public:
	RelInfo(string segName,int size,string name,int type);
	string segName;       // 重定位目标段
	Elf32_Rel * rel;      // 重定位信息
	string relName;       // 重定位符号名
};


class ElfFile
{
public:
    Elf32_Ehdr ehdr;                        // 文件头
    std::vector<Elf32_Phdr*> phdrTab;       // 程序头表
    std::map<string,Elf32_Shdr*> shdrTab;   // 段表
    std::vector<string> shdrNames;          // 段名顺序
    std::map<string,Elf32_Sym*> symTab;     // 符号表
    std::vector<string> symNames;           // 符号名顺序
    std::vector<RelInfo*> relTab;           // 重定位表
    string shstrtab;                        // 段表字符串表
    string strtab;                          // 字符串表

    void addShdr(string shName,int size,int dataLen);
    void addShdr(string name,
                 Elf32_Word sh_type,  Elf32_Word sh_flags,     Elf32_Addr sh_addr,
                 Elf32_Off sh_offset, Elf32_Word sh_size,      Elf32_Word sh_link,
                 Elf32_Word sh_info,  Elf32_Word sh_addralign, Elf32_Word sh_entsize);

    void addSym(Label* label);
    void addRel(string segName,int addr,string name,int type);
    int getSegIndex(string name);
};