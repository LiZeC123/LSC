#pragma once
#include<string>
#include"Huffman.h"
#include"HufFile.h"

int getWeight(std::string filename, int[]);

class Coder{
public:
	virtual Coder& loadData(std::string filename) = 0;
	virtual Coder& writeToFile(std::string filename) = 0;
	virtual void printInfo() = 0;
	virtual ~Coder() { }
protected:
	const static int N = 256;
	//根据编码分配适当大小的缓冲区
	int getBufSize(Haffman::Code code[]);
	//生成哈夫曼编码

};


class Encoder : Coder {
public:
	Coder& loadData(std::string filename) override;
	Coder& writeToFile(std::string filename) override;
	void printInfo() override;
	~Encoder();
private:
	HEAD genHead(std::string filename);
	void fillBuf(std::string filename, char buf[], Haffman::Code code[]);
private:
	HEAD head;
	char* buf;
	int bufSize;
};


class Decoder : Coder {
public:
	Coder& loadData(std::string filename) override;
	Coder& writeToFile(std::string filename) override;
	void printInfo() override;
	~Decoder();
private:
	HEAD readHead(std::string filename);
	void fillBuf(std::string filename);
private:
	HEAD head;
	char* buf;
	int bufSize;
	Haffman::Tree* tree;
};