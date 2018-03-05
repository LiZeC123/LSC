#pragma once
#include <vector>
#include <map>
#include <string>
#include "../ass/elf.h"

using std::string;

class Linker;

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
    void getData(char* buf,Elf32_Off offset,Elf32_Word size);
    void assemObj(Linker* linker);
    void writeElf(Linker* linker,FILE* fout);
    const string& getFileName();
    void addPhdr(Elf32_Word type,Elf32_Off off,Elf32_Addr vaddr,Elf32_Word filesz,Elf32_Word memsz,Elf32_Word flags,Elf32_Word align);
    void addShdr(string sh_name,Elf32_Word sh_type,Elf32_Word sh_flags,Elf32_Addr sh_addr,Elf32_Off sh_offset,
			Elf32_Word sh_size,Elf32_Word sh_link,Elf32_Word sh_info,Elf32_Word sh_addralign,
			Elf32_Word sh_entsize);
    void addSym(string name,Elf32_Sym*s);
    void printInfo();

    

    Elf32_Ehdr ehdr;                        // 文件头
    std::vector<Elf32_Phdr*> phdrTab;       // 程序头表
    std::map<string,Elf32_Shdr*> shdrTab;   // 段表
    std::vector<string> shdrNames;          // 段名顺序
    std::map<string,Elf32_Sym*> symTab;     // 符号表
    std::vector<string> symNames;           // 符号名顺序
    std::vector<RelInfo*> relTab;           // 重定位表
    string shstrtab;                        // 段表字符串表
    string strtab;                          // 字符串表
private:
    string file;

};