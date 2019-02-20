#pragma once
#include <string>
#include <vector>
#include "Huffman.h"
#include "HufFile.h"

class BinReader;

class Coder{
public:
	virtual void printInfo() = 0;
	virtual ~Coder() { }
protected:
	const static int N = 256;
	void initName(char name[]);
};


class Encoder : public Coder {
public:
	Encoder& addFile(std::string input);
	Encoder& doCoder(std::string output);
	void printInfo() override;
	~Encoder();
private:
	void initHeads();
	void writeHeads(FILE* out); // 写入全部的HEAD信息
	void writeBody(Haffman::Code* code, FILE* out);
private:
	size_t bitCount = 0;
	std::vector<std::string> files;

	Lar::Head head;
	Lar::Item* itemTable;
};


class Decoder : public Coder {
public:
	Decoder& doCoder(std::string input);
	void printInfo() override;
	~Decoder();
private:
	void readHeads(FILE* in);
	void writeFile(BinReader& reader, Haffman::Node* node, int fileIdx);
private:
	Lar::Head head;
	Lar::Item* itemTable;
	std::string* files;

	size_t hasReadBits = 0;
};