#pragma once

#include <string>


class BinWriter
{
public:
    BinWriter(FILE* out);
    bool writeBit(int8_t bit);
    bool writeByte(int8_t byte);

    // 将缓冲区的内容写入到文件之中
    // 完成最后一次写入以后, 一定要调用此函数将缓冲区剩余的数据写入文件
    // 由于bit的不完整数据也会写入, 因此此函数只能在数据写入结束后调用一次
    bool flush();

    ~BinWriter();
private:
    bool writeToFileIfFull();
    void checkStatus();
private:
    const static int BUF_SIZE = 128;
    FILE* out = nullptr;
    unsigned int bpos = 0;   // 一个Byte内bit的位置
    unsigned int tpos = 0;   // 缓冲区位置

    int8_t byteBuf = 0;
    int8_t buf[BUF_SIZE];

    bool status = true;
};

class BinReader
{
public:
    BinReader(FILE* in);
    int8_t readBit();
    //int8_t readByte();
private:
    void readDataIfEmpty();
    void fullBuf();
private:
    const static int BUF_SIZE = 128;
    FILE *in = nullptr;
    unsigned int bpos = 0;      // 一个Byte内bit的位置
    unsigned int tpos = 0;      // 缓冲区位置
    unsigned int currMax = 0;   // 当前缓冲区实际的byte数量

    int8_t buf[BUF_SIZE];

    bool isEOF = false;
};
