#include "Linker.h"
#include "common.h"

using namespace std;

Linker::Linker()
{
	segNames.push_back(".text");
	segNames.push_back(".data");
	//segNames.push_back(".bss");   // .bss段作用待定
	for(unsigned int i=0;i<segNames.size();i++){
        segLists[segNames[i]]=new SegList();
    }
		
}

bool Linker::link(const char* filename)
{
    collectInfo();
    if(!symValid()){
        return false;
    }
    allocAddr();
    symParser();
    relocate();
    assemExe();
    exportElf(filename);

    return true;
}

void Linker::addElfFile(char* filename)
{
    ElfFile* file = new ElfFile();
    file->readElf(string(filename));
    fileList.push_back(file);
}

void Linker::allocAddr()
{
    unsigned int currAddr = BASE_ADDR;
    unsigned int currOff = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) * segNames.size();
    for(auto& name:segNames){
        segLists[name]->allocAddr(name,currAddr,currOff);
    }
}

void Linker::collectInfo()
{
    for(unsigned int i=0;i<fileList.size();i++){
        ElfFile* elf = fileList[i];

        // 记录段表信息
        for(unsigned int i=0;i<segNames.size();i++){
            if(elf->shdrTab.find(segNames[i]) != elf->shdrTab.end()){
                segLists[segNames[i]]->addOwner(elf);
            }
        }

        // 记录符号引用信息
        for(auto it = elf->symTab.begin();it != elf->symTab.end();it++){
            if(ELF32_ST_BIND(it->second->st_info) == STB_GLOBAL) {
                SymLink* symLink = new SymLink();
                symLink->name = it->first;
                if(it->second->st_shndx == STN_UNDEF){
                    // 导入符号
                    symLink->recv = elf;
                    symLink->prov = nullptr;
                    symLinks.push_back(symLink);
                }
                else{
                    // 导出符号
                    symLink->recv = nullptr;
                    symLink->prov = elf;
                    symDef.push_back(symLink);
                }
            }
        }
    }
}


bool Linker::symValid()
{
    bool isValid = true;
    ElfFile* startOwner = nullptr;

    for(unsigned int i=0;i<symDef.size();i++){
        if(symDef[i]->name == START){
            startOwner = symDef[i]->prov;
        }

        for(unsigned int j = i+1;j<symDef.size();j++){
            if(symDef[i]->name == symDef[j]->name){
                printf("在%s中和%s中,符号 %s 重定义\n"
                    ,symDef[i]->prov->getFileName().c_str()
                    ,symDef[j]->prov->getFileName().c_str()
                    ,symDef[i]->name.c_str());
                isValid = false;
            }
        }
    }

    if(startOwner == nullptr){
        printf("找不到入口符号\n");
        isValid = false;
    }

    for(unsigned int i=0;i<symLinks.size();i++){
        for(unsigned int j=0;j<symDef.size();j++){
            if(symLinks[i]->name == symDef[j]->name){
                symLinks[i]->prov = symDef[j]->prov;
                break;
            }
        }

        if(symLinks[i]->prov == nullptr){
            printf("在%s中,符号%s未定义\n"
                ,symLinks[i]->recv->getFileName().c_str()
                ,symLinks[i]->name.c_str());
            isValid = false;
        }
    }

    return isValid;
}

void Linker::symParser()
{
    for(ElfFile* elf:fileList){
        for(auto it=elf->symTab.begin();it!=elf->symTab.end();it++){
            Elf32_Sym* sym = it->second;
            if(sym->st_shndx != STN_UNDEF){
                string name = elf->shdrNames[sym->st_shndx];
                sym->st_value += elf->shdrTab[name]->sh_addr;
            }
        }
    }

    for(auto symlink:symLinks){
        string name = symlink->name;
        Elf32_Sym* provsym = symlink->prov->symTab[name];
        Elf32_Sym* recvsym = symlink->recv->symTab[name];
        recvsym->st_value = provsym->st_value;
    }
}

void Linker::relocate()
{
    for(unsigned int i=0;i<fileList.size();i++){
        vector<RelInfo*> tab = fileList[i]->relTab;

        for(unsigned int j=0;j<tab.size();j++){
            // 重定位符号
            string relName = tab[j]->relName;
            Elf32_Sym* sym = fileList[i]->symTab[relName];

            // 重定位段
            string segName = tab[j]->segName;
            Elf32_Shdr* seg = fileList[i]->shdrTab[segName];

            //重定位符号地址(符号本身相对段基址偏移)
            unsigned int symAddr = sym->st_value;
            
            // 重定位位置相对于被重定位段基址的偏移(引用这个符号的位置)
            unsigned int offset = tab[j]->rel->r_offset;
            
            // 重定位值等于符号所在段基址加上相对段的偏移(绝对地址)
            unsigned int relAddr = seg->sh_addr + offset;
            
            // 重定位类型
            unsigned char type = ELF32_R_TYPE(tab[j]->rel->r_info);

            segLists[segName]->relocAddr(relAddr,type,symAddr);
        }
    }
}

void Linker::assemExe()
{
    fileList[0]->assemObj(this);
}


void Linker::exportElf(const char* filename)
{
    FILE * fout = fopen(filename,"w");
    fileList[0]->writeElf(this,fout);
    fclose(fout);
}

void Linker::printInfo()
{
    for(auto file:fileList){
        file->printInfo();
        printf("\n");
    }
}

std::vector<string> Linker::getSegNames()
{
    return segNames;
}

std::vector<SymLink*> Linker::getSymDef()
{
    return symDef;
}

std::map<string,SegList*> Linker::getSegLists()
{
    return segLists;
}