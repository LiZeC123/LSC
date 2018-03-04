#include <string.h>
#include "ElfFile.h"
#include "SymTab.h"
using namespace std;

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
        sym->st_shndx = getSegIndex(label->getSegName()); 
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
    for(i=0;i<shdrNames.size();i++){
        if(shdrNames[i] == name){
            return i;
        }
    }
    return -1;
}

void ElfFile::assemObj(int dataLen)
{
    vector<string> AllSegNames = shdrNames;
    AllSegNames.push_back(".shstrtab");
    AllSegNames.push_back(".symtab");
    AllSegNames.push_back(".strtab");
    AllSegNames.push_back(".rel.text");
    AllSegNames.push_back(".rel.data");

    // 段索引
    map<string,int> shIdx;
    // 段名索引
    map<string,int> shstrIdx;
    // 建立索引
    for(unsigned int i=0;i<AllSegNames.size();i++){
        string name = AllSegNames[i];
        shIdx[name] = i;
        shstrIdx[name] = shstrtab.size();
        shstrtab += name;
        shstrtab.push_back('\0');
    }

    // 符号索引
    map<string,int> symIdx;
    // 符号名索引
    map<string,int> strIdx;
    // 建立索引
    for(unsigned int i=0;i<symNames.size();i++){
        string name = symNames[i];
        symIdx[name] = i;
        strIdx[name] = strtab.size();
        strtab += name;
        strtab.push_back('\0');
    }

    // 更新符号表符号名索引
    for(unsigned int i=0;i<symNames.size();i++){
        string name = symNames[i];
        symTab[name]->st_name = strIdx[name];
    }

    // 处理重定位表
    for(unsigned int i=0;i<relTab.size();i++){
        Elf32_Rel * rel = new Elf32_Rel();
        rel->r_offset = relTab[i]->rel.r_offset;
        rel->r_info = ELF32_R_INFO(
            symIdx[relTab[i]->relName],
            ELF32_R_TYPE(relTab[i]->rel.r_info)
        );

        if(relTab[i]->segName == ".text"){
            relTextTab.push_back(rel);
        }
        else if(relTab[i]->segName == ".data") {
            relDataTab.push_back(rel);
        }
        else{
            delete rel;
        }
    }


    int* p_id = (int*)ehdr.e_ident;
	*p_id++=0x464c457f;
    *p_id++=0x00010101;
    *p_id++=0x00000000;
    *p_id++=0x00000000;
    
    ehdr.e_type=ET_REL;
	ehdr.e_machine=EM_386;
	ehdr.e_version=EV_CURRENT;
	ehdr.e_entry=0;
	ehdr.e_phoff=0;
	ehdr.e_flags=0;
	ehdr.e_ehsize= sizeof(Elf32_Ehdr);
	ehdr.e_phentsize=0;
	ehdr.e_phnum=0;
	ehdr.e_shentsize = sizeof(Elf32_Shdr);
	ehdr.e_shnum     = AllSegNames.size();
	ehdr.e_shstrndx  = shIdx[".shstrtab"];

    int curOff = sizeof(ehdr);
    curOff += dataLen;

    addShdr(".shstrtab",SHT_STRTAB,0,0,curOff,shstrtab.size(),SHN_UNDEF,0,1,0);
    curOff += shstrtab.size();
    curOff += (4 - curOff % 4) % 4;

    ehdr.e_shoff = curOff;      // 段表偏移位置
    curOff += ehdr.e_shnum*ehdr.e_shentsize;

    addShdr(".symtab",SHT_SYMTAB,0,0,curOff,symNames.size()*sizeof(Elf32_Sym),shIdx[".strtab"],0,1,sizeof(Elf32_Sym));
    curOff += symNames.size()*sizeof(Elf32_Sym);

    addShdr(".strtab",SHT_STRTAB,0,0,curOff,strtab.size(),SHN_UNDEF,0,1,0);
    curOff += strtab.size();
    curOff += (4 - curOff % 4) % 4;
  
    addShdr(".rel.text",SHT_REL,0,0,curOff,relTextTab.size()*sizeof(Elf32_Rel),shIdx[".symtab"],shIdx[".text"],1,sizeof(Elf32_Rel));
    curOff += relTextTab.size()*sizeof(Elf32_Rel);

    addShdr(".rel.data",SHT_REL,0,0,curOff,relDataTab.size()*sizeof(Elf32_Rel),shIdx[".symtab"],shIdx[".data"],1,sizeof(Elf32_Rel));
    curOff += relDataTab.size()*sizeof(Elf32_Rel);

    for(unsigned int i=0;i<AllSegNames.size();i++){
        string name = AllSegNames[i];
        shdrTab[name]->sh_name = shstrIdx[name];
    }
}


void ElfFile::writeElfHead(FILE* fin,FILE* fout)
{
    int padNum = 0;
    char pad[1] = {0};

    fwrite(&ehdr,ehdr.e_ehsize,1,fout);
}

void ElfFile::writeElfTail(FILE* fin,FILE* fout)
{
    int padNum = 0;
    char pad[1] = {0};

    // 由于编译器输出的时候,先输出的data段,再输出的text段
    // 所以实际上应该,汇编的时候页应该先输出data段,再输出text段
    
    // 填补data段和text段之间的空隙
    padNum = shdrTab[".text"]->sh_offset - shdrTab[".data"]->sh_offset - shdrTab[".data"]->sh_size;
    fwrite(pad,sizeof(pad),padNum,fout);
    
    // .text
    char buffer[1024] = {0};
    int count = -1;
    //int addLen = 0;
    while(count){
        count = fread(buffer,1,1024,fin);
        fwrite(buffer,1,count,fout);
        //addLen += count;
    }

    // .shstrtab
    padNum = shdrTab[".shstrtab"]->sh_offset - shdrTab[".text"]->sh_offset - shdrTab[".text"]->sh_size;
    fwrite(pad,sizeof(pad),padNum,fout);
    fwrite(shstrtab.c_str(),shstrtab.size(),1,fout);

    // 段表
    padNum = ehdr.e_shoff - shdrTab[".shstrtab"]->sh_offset - shdrTab[".shstrtab"]->sh_size;
    fwrite(pad,sizeof(pad),padNum,fout);
    for(unsigned int i=0;i<shdrNames.size();i++){
        Elf32_Shdr* sh = shdrTab[shdrNames[i]];
        fwrite(sh,ehdr.e_shentsize,1,fout);
    }

    // 符号表
    for(unsigned int i=0;i<symNames.size();i++){
        Elf32_Sym* sym = symTab[symNames[i]];
        fwrite(sym,sizeof(Elf32_Sym),1,fout);
    }

    // .strtab
    fwrite(strtab.c_str(),strtab.size(),1,fout);

    // .rel.text
    padNum = shdrTab[".rel.text"]->sh_offset - shdrTab[".strtab"]->sh_offset - shdrTab[".strtab"]->sh_size;
    fwrite(pad,sizeof(pad),padNum,fout);
    for(unsigned int i=0;i<relTextTab.size();i++){
        Elf32_Rel* rel = relTextTab[i];
        fwrite(rel,sizeof(Elf32_Rel),1,fout);
    }

    // .rel.data
    for(unsigned int i=0;i<relDataTab.size();i++){
        Elf32_Rel* rel = relDataTab[i];
        fwrite(rel,sizeof(Elf32_Rel),1,fout);      
    }  
}
