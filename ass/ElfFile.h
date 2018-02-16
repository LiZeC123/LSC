#pragma once
#include <elf.h>
#include <string>
#include <vector>
#include <map>

using std::string;

struct RelItem
{
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
    std::vector<RelItem*> relTab;           // 重定位表
    string shstrtab;                        // 段表字符串表
    string strtab;                          // 字符串表

    void addShdr(string shName,int size);
    void addShdr(string name,
                 Elf32_Word sh_type,  Elf32_Word sh_flags,     Elf32_Addr sh_addr,
                 Elf32_Off sh_offset, Elf32_Word sh_size,      Elf32_Word sh_link,
                 Elf32_Word sh_info,  Elf32_Word sh_addralign, Elf32_Word sh_entsize);
};