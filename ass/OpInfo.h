#pragma once

class ModRM     // Mod/R字段
{
public:
    int mod;
    int reg;
    int rm;
};

class SIB       // SIB字段
{
public:
    int scale;
    int index;
    int base;
};

class Inst      // 指令中的其他字段
{
public:
    unsigned char opcode;
    int disp;       // 偏移字段
    int imm32;      // 立即数字段
    int dispLen;    // 偏移字段长度
};

class GlobalOpInfo
{
public:
    Inst inst;
    ModRM modrm;
    SIB sib;
};