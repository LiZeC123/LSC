#include "ElfFile.h"

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
		if(name.empty()){
            //删除空段表项
            delete shdr;
        } 
        else {
			shdrNames.push_back(name);
			shdrTab[name]=shdr;
		}
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
		if(name.empty()){
            //删除空符号项
            delete sym;
        }	
		else {
			symNames.push_back(name);
			//symList.push_back(sym);
			symTab[name]=sym;//加入符号表
		}
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

const string& ElfFile::getFileName()
{
	return file;
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