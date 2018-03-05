#pragma once
#include <vector>
#include <string>
#include "ElfFile.h"

using std::string;

struct Block 
{
    Block(char* data,unsigned int offset,unsigned int size);
    char * data;            // 块数据
    unsigned int offset;    // 块偏移
    unsigned int size;      // 块大小
};

struct SymLink
{
    string name;
    ElfFile* recv;
    ElfFile* prov;
};

class SegList 
{
public:
    void allocAddr(string name,unsigned int& base,unsigned int& off);
    void relocAddr(unsigned int relAddr,unsigned char type, unsigned int symAddr);

    void addOwner(ElfFile* file);

    unsigned int getBaseAddr();
    unsigned int getBegin();
    unsigned int getOffset();
    unsigned int getSize();
    std::vector<Block*> getBlocks();
private:
    unsigned int baseAddr;              // 基地址
    unsigned int begin;                 // 对齐前偏移
    unsigned int offset;                // 对齐后偏移
    unsigned int size;                  // 总大小
    std::vector<ElfFile*> ownerList;    // 所有者文件
    std::vector<Block*> blocks;         // 数据块
};