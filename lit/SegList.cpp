#include "SegList.h"
#include "common.h"


Block::Block(char* data,unsigned int offset,unsigned int size)
{
    this->data = data;
    this->offset = offset;
    this->size = size;
}

void SegList::allocAddr(string name,unsigned int& base,unsigned int& off)
{
    begin = off;    // 对齐前偏移

    // 代码段按照16字节对齐,其他段按照4字节对齐
    int align = (name == ".text")? TEXT_ALIGN : DISC_ALIGN; 
    off += (align - off % align) % align;
    
    base += (MEM_ALIGN - base % MEM_ALIGN) % MEM_ALIGN + off % MEM_ALIGN;

    baseAddr = base;
    offset = off;
    size = 0;           // 段大小以及段内偏移
    
    
    for(unsigned int i=0;i<ownerList.size();i++){
        // 段内的每一部分按照4字节对齐
        size += (DISC_ALIGN - size % DISC_ALIGN) % DISC_ALIGN;

        Elf32_Shdr* seg = ownerList[i]->shdrTab[name];

        char *buf = new char[seg->sh_size];
        ownerList[i]->getData(buf, seg->sh_offset, seg->sh_size);
        blocks.push_back(new Block(buf, size, seg->sh_size));

        seg->sh_addr = base + size;
        size += seg->sh_size;
    }

    base += size;
    off += size;
}

void SegList::relocAddr(unsigned int relAddr,unsigned char type, unsigned int symAddr)
{
    unsigned int relOffset = relAddr - baseAddr;
    Block* block = nullptr;
    for(unsigned int i=0;i<blocks.size();i++){
        unsigned int start = blocks[i]->offset;
        unsigned int end = blocks[i]->size;
        if(start <= relOffset && relOffset < end){
            block = blocks[i];
            break;
        }
    }

    int *pAddr = (int*)(block->data + relOffset - block->offset);
    if(type == R_386_32){
        // 绝对地址重定位直接写入新地址
        *pAddr = symAddr;
    }
    else if(type == R_386_PC32){
        // 相对地址重定位 = 符号地址 - 重定位位置 + 原内容
        // = (符号地址 - 重定位位置) + (重定位位置 - 下一条指令位置)
        // = 符号地址 - 下一条指令位置 
        *pAddr = symAddr - relAddr + *pAddr;
    }

}

void SegList::addOwner(ElfFile* file)
{
    this->ownerList.push_back(file);
}

unsigned int SegList::getBaseAddr()
{
    return baseAddr;
}

unsigned int SegList::getBegin()
{
    return begin;
}

unsigned int SegList::getOffset()
{
    return offset;
}

unsigned int SegList::getSize()
{
    return size;
}

std::vector<Block*> SegList::getBlocks()
{
    return blocks;
}