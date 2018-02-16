#include "ElfFile.h"
#include "SymTab.h"

void ElfFile::addShdr(string shName,int size)
{
    int off = sizeof(Elf32_Ehdr) + SymTab::dataLen;
    if(shName == ".text"){
        addShdr(shName,SHT_PROGBITS,SHF_ALLOC|SHF_EXECINSTR,
        0,off,size,0,0,4,0);
    }
}

void ElfFile::addShdr(
    string sh_name,
    Elf32_Word sh_type,
    Elf32_Word sh_flags,
    Elf32_Addr sh_addr,
    Elf32_Off sh_offset,
    Elf32_Word sh_size,
    Elf32_Word sh_link,
    Elf32_Word sh_info,
    Elf32_Word sh_addralign,
    Elf32_Word sh_entsize)
{
    Elf32_Shdr *sh = new Elf32_Shdr();
    sh->sh_name = 0;
    sh->sh_type = sh_type;
    sh->sh_flags = sh_flags;
    sh->sh_addr = sh_addr;
    sh->sh_offset = sh_offset;
    sh->sh_size = sh_size;
    sh->sh_link = sh_link;
    sh->sh_info = sh_info;
    sh->sh_addralign = sh_addralign;
    sh->sh_entsize = sh_entsize;
    shdrTab[sh_name] = sh;
    shdrNames.push_back(sh_name);
}