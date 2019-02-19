#include <stdio.h>

#include "BinTools.h"
using namespace std;

BinWriter::BinWriter(FILE* out)
{
    this->out = out;
}

bool BinWriter::writeBit(int8_t bit)
{
    checkStatus();

    byteBuf <<= 1;
    byteBuf |= (bit & 0x01);
    bpos++;

    if(bpos == 8){
        buf[tpos++] = byteBuf;
        byteBuf = 0;
        bpos = 0;
    }

    return writeToFileIfFull();
}

bool BinWriter::writeByte(int8_t byte)
{
    checkStatus();

    if(bpos == 0){
        buf[tpos++] = byte;
    }
    else{
        printf("WRANING: 比特位置未对齐\n");
        for(int i=0;i<8;i++){
            
            int8_t bit = (byte>>(7-i));
            writeBit(bit);
        }
    }

    return writeToFileIfFull();
}

bool BinWriter::flush()
{
    bool isSuccess = (tpos == fwrite(buf,sizeof(int8_t),tpos,out));
    tpos = 0;

    if(isSuccess && bpos != 0){
        printf("WRANING: byteBuf存在数据, 并且写入到文件");
        status = false;
        byteBuf <<= (8 - bpos);
        isSuccess = (1 == fwrite(&byteBuf,sizeof(int8_t),1,out));
    }

    return isSuccess;
}


bool BinWriter::writeToFileIfFull()
{
    // 是否成功写入, 不需要写入时返回true
    bool isSuccess = true;
    if(tpos == BUF_SIZE) {
        isSuccess = flush();
    }

    return isSuccess;
}

void BinWriter::checkStatus()
{
    if(!status){
        printf("WRANING:bit部分数据已经填充末尾0并且写入, 此模块不应该继续写入数据");
    }
}



BinReader::BinReader(FILE* in)
{
    this->in = in;
    fullBuf();
}

int8_t BinReader::readBit()
{
    if(isEOF){
        return EOF;
    }

    int8_t byte = buf[tpos];
    int8_t bit = byte >> (7 - bpos);
    bpos++;

    if(bpos == 8) {
        tpos++;
        bpos = 0;
    }

    readDataIfEmpty();

    return bit & 0x01;
}


void BinReader::readDataIfEmpty()
{
    if(tpos == currMax){
        fullBuf();
        tpos = 0;
        bpos = 0;
    }
    if(currMax == 0) {
        isEOF = true;
    }
}

void BinReader::fullBuf()
{
    currMax = fread(buf,sizeof(int8_t),BUF_SIZE,in);
}