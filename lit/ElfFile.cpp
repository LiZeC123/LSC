#include "ElfFile.h"
#include "Linker.h"
#include "common.h"
using namespace std;

RelInfo::RelInfo(string segName,Elf32_Rel* rel,string relName)
{
    this->segName = segName;
    this->rel = rel;
    this->relName = relName;
}

void ElfFile::readElf(const string file)
{
	this->file = file;
	FILE*fp=fopen(file.c_str(),"rb");

	//读取文件头
    rewind(fp);
	fread(&ehdr,sizeof(Elf32_Ehdr),1,fp);

    // 程序头表
	if(ehdr.e_type==ET_EXEC)
	{
		fseek(fp,ehdr.e_phoff,0);
		for(int i=0;i<ehdr.e_phnum;++i)
		{
			Elf32_Phdr*phdr=new Elf32_Phdr();
			fread(phdr,ehdr.e_phentsize,1,fp);
			phdrTab.push_back(phdr);
		}
	}

    // .shstrtab表项
	fseek(fp,ehdr.e_shoff+ehdr.e_shentsize*ehdr.e_shstrndx,0);
	Elf32_Shdr shstrTab;
	fread(&shstrTab,ehdr.e_shentsize,1,fp);

    // .shstrtab
	char*shstrTabData=new char[shstrTab.sh_size];
	fseek(fp,shstrTab.sh_offset,0);
	fread(shstrTabData,shstrTab.sh_size,1,fp);
	
    // 段表
	fseek(fp,ehdr.e_shoff,0);
	for(int i=0;i<ehdr.e_shnum;++i)
	{
		Elf32_Shdr*shdr=new Elf32_Shdr();
		fread(shdr,ehdr.e_shentsize,1,fp);
		string name(shstrTabData+shdr->sh_name);
		shdrNames.push_back(name);
		shdrTab[name] = shdr;
	}
	delete []shstrTabData;//清空段表字符串表

    // .strtab
	Elf32_Shdr *strTab=shdrTab[".strtab"];
	char* strTabData = new char[strTab->sh_size];
	fseek(fp,strTab->sh_offset,0);
	fread(strTabData,strTab->sh_size,1,fp);

    // .symtab
	Elf32_Shdr *sh_symTab=shdrTab[".symtab"];
	fseek(fp,sh_symTab->sh_offset,0);
	int symNum=sh_symTab->sh_size/sh_symTab->sh_entsize;
	//vector<Elf32_Sym*>symList;
	for(int i=0;i<symNum;++i)
	{
		Elf32_Sym*sym=new Elf32_Sym();
		fread(sym,sh_symTab->sh_entsize,1,fp);
		string name(strTabData+sym->st_name);
		symNames.push_back(name);
		symTab[name]=sym;//加入符号表
	}

    for(unsigned int i=0;i<shdrNames.size();i++){
        string shdrName = shdrNames[i];
        Elf32_Shdr *shdr =shdrTab[shdrName];

        if(shdr->sh_type == SHT_REL) {
            fseek(fp,shdr->sh_offset,0);
            int relNum = shdr->sh_size / shdr->sh_entsize;

            for(int j=0;j<relNum;j++){
                Elf32_Rel* rel = new Elf32_Rel();
                fread(rel,shdr->sh_entsize,1,fp);
                string segName = shdrNames[shdr->sh_info];
                string symName = symNames[ELF32_R_SYM(rel->r_info)];
                relTab.push_back(new RelInfo(segName,rel,symName));
            }

        }
    }
	delete []strTabData;//清空字符串表

	fclose(fp);
}

void ElfFile::getData(char* buf,Elf32_Off offset,Elf32_Word size)
{
	FILE* fp = fopen(file.c_str(),"rb");
	rewind(fp);
	fseek(fp,offset,0);
	fread(buf,size,1,fp);
	fclose(fp);
}

void ElfFile::assemObj(Linker* linker)
{
	vector<string> AllSegNames;
	AllSegNames.push_back("");
	vector<string> segNames = linker->getSegNames();

	for(auto name:segNames){
		AllSegNames.push_back(name);
	}
    AllSegNames.push_back(".shstrtab");
    AllSegNames.push_back(".symtab");
    AllSegNames.push_back(".strtab");


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

	// 生成符号表
	addSym("",nullptr);
	vector<SymLink*> symDef = linker->getSymDef();
	for(auto def:symDef){
		string name = def->name;
		ElfFile* prov = def->prov;
		Elf32_Sym * sym = prov->symTab[name];
		string segName = prov->shdrNames[sym->st_shndx];
		addSym(name,sym);
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


    int* p_id = (int*)ehdr.e_ident;
	*p_id++=0x464c457f;
    *p_id++=0x00010101;
    *p_id++=0x00000000;
    *p_id++=0x00000000;

	ehdr.e_type = ET_EXEC;
	ehdr.e_machine = EM_386;
	ehdr.e_version = EV_CURRENT;
	ehdr.e_entry = symTab[START]->st_value;
	ehdr.e_phoff = 0;
	ehdr.e_shoff = 0;
	ehdr.e_flags = 0;
	ehdr.e_ehsize = sizeof(Elf32_Ehdr);
	ehdr.e_phentsize = sizeof(Elf32_Phdr);
	ehdr.e_phnum = segNames.size();
	ehdr.e_shentsize = sizeof(Elf32_Shdr);
	ehdr.e_shnum     = AllSegNames.size();
	ehdr.e_shstrndx  = shIdx[".shstrtab"];

	int curOff = sizeof(ehdr);

	ehdr.e_phoff = curOff;

	for(auto name:segNames){
		Elf32_Word flags = (name == ".text")?(PF_X|PF_R):( PF_W|PF_R);
		SegList* seg = linker->getSegLists()[name];
		addPhdr(PT_LOAD,seg->getOffset(),seg->getBaseAddr()
				,seg->getSize(),seg->getSize()
				,flags,MEM_ALIGN);
	}

    curOff += ehdr.e_phentsize*ehdr.e_phnum;

	addShdr("",0,0,0,0,0,0,0,0,0);
	for(auto name:segNames){
		Elf32_Word sh_flags = (name == ".text") ? (SHF_ALLOC|SHF_EXECINSTR) : (SHF_ALLOC|SHF_WRITE);
		Elf32_Word sh_align = (name == ".text") ? TEXT_ALIGN : DISC_ALIGN;

		SegList* seg = linker->getSegLists()[name];
		addShdr(name,SHT_PROGBITS,sh_flags
				,seg->getBaseAddr(),seg->getOffset(),seg->getSize()
				,0,0,sh_align,0);
		curOff = seg->getOffset() + seg->getSize();
	}

	curOff += (4 - curOff % 4) % 4;


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


    for(unsigned int i=0;i<AllSegNames.size();i++){
        string name = AllSegNames[i];
        shdrTab[name]->sh_name = shstrIdx[name];
    }

}

void ElfFile::writeElf(Linker* linker,FILE* fout)
{
    int padNum = 0;
    unsigned char pad[1] = {0};

	// 文件头
	fwrite(&ehdr,ehdr.e_ehsize,1,fout);

	// 程序头表
	for(unsigned int i=0;i<phdrTab.size();i++){
		fwrite(phdrTab[i],ehdr.e_phentsize,1,fout);
	}

	// .text .data
	for(auto name:linker->getSegNames()){
		pad[0] = (name == ".text") ? 0x90 : 0x00;
		SegList* seg = linker->getSegLists()[name];
		padNum = seg->getOffset() - seg->getBegin();
		fwrite(pad,sizeof(pad),padNum,fout);

		Block* last = nullptr;
		for(auto block:seg->getBlocks()){
			if(last != nullptr){
				unsigned int lastEnd = last->offset + last->size;
				padNum = block->offset - lastEnd;
				//int n = sizeof(pad);
				fwrite(pad,sizeof(pad),padNum,fout);
			}
			fwrite(block->data,block->size,1,fout);
			last = block;
		}
	}
	pad[0] = 0;
    
	// 检查data段与text段关系
    // .shstrtab
    padNum = shdrTab[".shstrtab"]->sh_offset - shdrTab[".data"]->sh_offset - shdrTab[".data"]->sh_size;
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
}

const string& ElfFile::getFileName()
{
	return file;
}

void ElfFile::addPhdr(Elf32_Word type,Elf32_Off off,Elf32_Addr vaddr,Elf32_Word filesz,
		Elf32_Word memsz,Elf32_Word flags,Elf32_Word align)
{
	Elf32_Phdr*ph=new Elf32_Phdr();
	ph->p_type=type;
	ph->p_offset=off;
	ph->p_vaddr=ph->p_paddr=vaddr;
	ph->p_filesz=filesz;
	ph->p_memsz=memsz;
	ph->p_flags=flags;
	ph->p_align=align;
	phdrTab.push_back(ph);
}

void ElfFile::addShdr(string sh_name,Elf32_Word sh_type,Elf32_Word sh_flags,Elf32_Addr sh_addr,Elf32_Off sh_offset,
			Elf32_Word sh_size,Elf32_Word sh_link,Elf32_Word sh_info,Elf32_Word sh_addralign,
			Elf32_Word sh_entsize)//添加一个段表项
{
	Elf32_Shdr*sh=new Elf32_Shdr();
	sh->sh_name=0;
	sh->sh_type=sh_type;
	sh->sh_flags=sh_flags;
	sh->sh_addr=sh_addr;
	sh->sh_offset=sh_offset;
	sh->sh_size=sh_size;
	sh->sh_link=sh_link;
	sh->sh_info=sh_info;
	sh->sh_addralign=sh_addralign;
	sh->sh_entsize=sh_entsize;
	shdrTab[sh_name]=sh;
	shdrNames.push_back(sh_name);
}

void ElfFile::addSym(string name,Elf32_Sym*s)
{
	Elf32_Sym* sym = new Elf32_Sym();
	symTab[name] = sym;
	if(name=="")
	{
		sym->st_name=0;
		sym->st_value=0;
		sym->st_size=0;
		sym->st_info=0;
		sym->st_other=0;
		sym->st_shndx=0;
	}
	else
	{
		sym->st_name=0;
		sym->st_value=s->st_value;
		sym->st_size=s->st_size;
		sym->st_info=s->st_info;
		sym->st_other=s->st_other;
		sym->st_shndx=s->st_shndx;
	}
	symNames.push_back(name);
}

void ElfFile::printInfo()
{
	printf("段表:\n");
	for(auto it = shdrTab.begin();it != shdrTab.end();it++){
		printf("%s \n",it->first.c_str());
	}
	printf("\n");

	printf("符号表\n");
	for(auto it = symTab.begin();it != symTab.end();it++){
		printf("%s \n",it->first.c_str());
	}
	printf("\n");

	

}