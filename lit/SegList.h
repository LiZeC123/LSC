#pragma once
#include <vector>
#include "ElfFile.h"

struct Block 
{
    char * data;
    unsigned int offset;
    unsigned int size;
};


class SegList 
{
public:
    void allocAddr();

private:
    unsigned int baseAddr;
    unsigned int begin;
    unsigned int offset;
    unsigned int size;
    std::vector<ElfFile*> ownerList;
    std::vector<Block*> blocks;
};