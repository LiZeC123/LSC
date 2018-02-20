#include "ElfFile.h"


RelInfo::RelInfo(string segName,int addr,string name,int type)
{
	this->segName = segName;
	this->relName = name;
	rel.r_offset = addr;
	rel.r_info   = ELF32_R_INFO(0,type);   // 目标文件生成阶段会自动填充重定位信息
}

void ElfFile::addShdr(string shName,int size,int dataLen)
{
    int off = sizeof(Elf32_Ehdr) + dataLen;
    if(shName == ".text"){
        addShdr(shName,SHT_PROGBITS,SHF_ALLOC|SHF_EXECINSTR,
        0,off,size,0,0,4,0);
    }
    else{
        addShdr(shName,SHT_PROGBITS,SHF_ALLOC|SHF_WRITE,
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

void ElfFile::addSym(Label* label)
{
    Elf32_Sym* sym = new Elf32_Sym();
    sym->st_name = 0;
    sym->st_value = label->getAddr();
    sym->st_size = label->getTimes() * label->getLen() * label->getCont().size();
    if(label->isGlobal()){
        sym->st_info = ELF32_ST_INFO(STB_GLOBAL,STT_NOTYPE);
    }
    else{
        sym->st_info = ELF32_ST_INFO(STB_LOCAL,STT_NOTYPE);
    }
    sym->st_other = 0;
    if(label->isExterned()){
        sym->st_shndx = STN_UNDEF;
    }
    else{
        sym->st_shndx = getSegIndex(label->getName());
    }

    symTab[label->getName()] = sym;
    symNames.push_back(label->getName());

}

RelInfo* ElfFile::addRel(string segName,int addr,string name,int type)
{
    RelInfo* rel = new RelInfo(segName,addr,name,type);
    relTab.push_back(rel);
    return rel;
}

int ElfFile::getSegIndex(string name)
{
    unsigned int i;
    for(i=0;i<symNames.size();i++){
        if(shdrNames[i] == name){
            return i;
        }
    }
    return -1;
}