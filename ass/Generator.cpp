#include <stdexcept>
#include "Generator.h"
#include "Scanner.h"
#include "Parser.h"
#include "elf.h"

Inst instr;
ModRM modrm;
SIB sib;

static int TwoOpCode[][2][4] = 
{
    //          8bit                     32bit 
    // r,r  r,rm  rm,r  r,imm   r,r   r,rm  rm,r  r,imm
    {{0x8a, 0x8a, 0x88, 0xb0}, {0x8b, 0x8b, 0x89, 0xb8}}, //mov
    {{0x3a, 0x3a, 0x38, 0x80}, {0x3b, 0x3b, 0x39, 0x81}}, //cmp
    {{0x2a, 0x2a, 0x28, 0x80}, {0x2b, 0x2b, 0x29, 0x81}}, //sub
    {{0x02, 0x02, 0x00, 0x80}, {0x03, 0x03, 0x01, 0x81}}, //add
    {{0x22, 0x22, 0x20, 0x80}, {0x23, 0x23, 0x21, 0x81}}, //and
    {{0x0a, 0x0a, 0x08, 0x80}, {0x0b, 0x0b, 0x09, 0x81}}, //or
    {{0x00, 0x00, 0x00, 0x00}, {0x8d, 0x8d, 0x00, 0x00}}  //lea
};

static int OneOpCode[] =
{
//  call  int   imul  idiv  neg   inc   dec   jmp
    0xe8, 0xcd, 0xf7, 0xf7, 0xf7, 0x40, 0x48, 0xe9,
//  je    jne
    0x84, 0x85,
//  sete  setne setg  setge setl setle
    0x94, 0x95, 0x9f, 0x9d, 0x9c, 0x9e,
//  push  pop
    0x50, 0x58
};

static int NonOpCodep[] = 
{
    //ret
    0xc3
};

Generator::Generator(SymTab& tab,FILE* fout) : symtab(tab)
{
    this->fout = fout;
}

bool Generator::processRel(int type)
{
    Label* relLabel = symtab.getRelLabel();
    if(Scanner::ScanLoop == 1 || relLabel == nullptr){
        symtab.setRelLabel(nullptr);
        return false;
    }

    bool flag = false;
    if(type == R_386_32){   // 绝对地址重定位
        symtab.getFile().addRel(Label::currSegName,Label::currAddr,relLabel->getName(),type);
        flag = true;
    }
    else if(type == R_386_PC32){    // 相对地址重定位
        if(relLabel->isExterned()){ // 外部跳转
            symtab.getFile().addRel(Label::currSegName,Label::currAddr,relLabel->getName(),type);
            flag = true;
        }
    }

    symtab.setRelLabel(nullptr);
    return flag;
}

void Generator::genTwoOp(Symbol s,OpType dstType,OpType srcType,int len)
{
    int opCode = getOpCode(s,dstType,srcType,len);
    switch (modrm.mod)
    {
    case -1:                                    // op reg imm
    {
        if (s == I_MOV) {
            opCode += modrm.reg;
        }
        else {
            int regCodes[] = {7, 5, 0, 4, 1};   // cmp sub add and or
            modrm.mod = 3;
            modrm.rm = modrm.reg;
            modrm.reg = regCodes[s - I_CMP];
        }

        writeBytes(opCode, 1);
        if (s != I_MOV) writeModRM();
        processRel(R_386_32);
        writeBytes(instr.imm32, len);
        break;
    }
    case 0:                                     // op reg,[reg] 或 op [reg],reg
        writeBytes(opCode, 1);
        writeModRM();
        if (modrm.rm == 5){                     // 特殊模式,立即数寻址
            processRel(R_386_32);
            writeDisp();
        } 
        else if (modrm.rm == 4) {               // 特殊模式,引导SIB段
            writeSIB();
        }
        break;
    case 1:                                     // op reg,[reg+8bit]  或 op [reg+8bit] ,reg
    case 2:                                     // op reg,[reg+32bit] 或 op [reg+32bit],reg
        writeBytes(opCode, 1);
        writeModRM();
        if (modrm.rm == 4) writeSIB();          // 特殊模式,引导SIB+8bit或引导SIB+32bit偏移
        writeDisp();
        break;
    case 3:                                     // op reg reg
        writeBytes(opCode, 1);
        writeModRM();
        break;
    default:
        break;
    }
}

void Generator::genOneOp(Symbol s,OpType type,int len)
{
    int op = OneOpCode[s - I_CALL];
    if(s == I_CALL || (s >= I_JMP && s <= I_JNE) ){
        if(s != I_CALL && s != I_JMP){
            writeBytes(0x0f,1);
        }
        writeBytes(op,1);
        /* 跳转之前先判断是否需要重定位
         * 如果需要重定位,所名当前保存的调整地址是无效的,暂时设为当前偏移地址
         * 否则说明调整地址正常,可以正常跳转
         * 跳转地址是相对下一条指令的偏移,由于接下来只需要写入四字节的偏移,因此pc = Label::currAddr + 4
        */
        int addr = processRel(R_386_PC32) ? Label::currAddr : instr.imm32;
        int pc = Label::currAddr + 4;
        writeBytes(addr-pc,4);
    }
    else if(s >= I_SETE && s <= I_SETLE){
        modrm.mod = 3;
        modrm.rm = modrm.reg;
        modrm.reg = 0;

        writeBytes(0x0f,1);
        writeBytes(op,1);
        writeModRM();
    }
    else if(s == I_INT){
        writeBytes(op,1);
        writeBytes(instr.imm32,1);
    }
    else if(s == I_PUSH){
        if(type == IMMEDIATE){
            op = 0x68;
            writeBytes(op,1);
            writeBytes(instr.imm32,4);
        }
        else{
            op += modrm.reg;
            writeBytes(op,1);
        }
    }
    else if(s == I_INC || s == I_DEC){
        if(len == 1){                           // 如果是8bit寄存器
            op = 0xfe;
            int regCodes[] = {0,1};
            modrm.mod = 3;
            modrm.rm = modrm.reg;
            modrm.reg = regCodes[s - I_INC];
            writeBytes(op,1);
            writeModRM();
        }
        else {                                  // 否则是32bit寄存器
            op += modrm.reg;
            writeBytes(op,1);
        }
    }
    else if(s == I_NEG){
        if(len == 1){
            op = 0xf6;
        }
        modrm.mod = 3;
        modrm.rm = modrm.reg;
        modrm.reg = 3;

        writeBytes(op,1);
        writeModRM();
    }
    else if(s == I_POP){
        op += modrm.reg;
        writeBytes(op,1);
    }
    else if(s == I_IMUL || s == I_IDIV){
        int regCodes[] = {5,7};
        modrm.mod = 3;
        modrm.rm = modrm.reg;
        modrm.reg = regCodes[s - I_IMUL];

        writeBytes(op,1);
        writeModRM();        
    }
}

void Generator::genNonOp(Symbol s)
{
    if(s != I_RET) {
        throw std::runtime_error("Unknow NonOp Code");
    }

    int op = NonOpCodep[s-I_RET];
    writeBytes(op,1);
}

int Generator::getOpCode(Symbol sym,OpType dstType,OpType srcType,int len)
{
    int index = 0;
    switch (srcType)
    {
    case IMMEDIATE:
        index = 3;
        break;
    case REGISTER:
        index = 2 * (dstType != REGISTER);
        break;
    case MEMORY:
        index = 1;
        break;
    default:
        break;
    }

    return TwoOpCode[sym-I_MOV][(len!=1)][index];
}

void Generator::writeBytes(int value,int len)
{
    Label::currAddr += len;
    if(Scanner::ScanLoop == 2){
        fwrite(&value,len,1,fout);
    }
}

void Generator::writeModRM()
{
    if(modrm.mod != -1){
        int byte = (modrm.mod << 6) + (modrm.reg << 3) + modrm.rm;
        writeBytes(byte,1);
    }
}

void Generator::writeSIB()
{
    if(sib.scale != -1){
        int byte = (sib.scale << 6) + (sib.index << 3) + sib.base;
        writeBytes(byte,1);
    }
}

void Generator::writeDisp()
{
    if(instr.dispLen){
        writeBytes(instr.disp,instr.dispLen);
        instr.dispLen = 0;
    }
}