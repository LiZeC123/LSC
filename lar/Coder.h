#pragma once
#include<string>
#include"Huffman.h"
#include"HufFile.h"

class Coder{
public:
	virtual Coder& doCoder(std::string input,std::string output) = 0;
	virtual void printInfo() = 0;
	virtual ~Coder() { }
protected:
	const static int N = 256;
	void initName(char name[]);
};


class Encoder : public Coder {
public:
	Coder& doCoder(std::string input,std::string output) override;
	void printInfo() override;
private:
	HEAD genHead(std::string filename);
private:
	int bitCount = 0;
};


class Decoder : public Coder {
public:
	Coder& doCoder(std::string input,std::string output) override;
	void printInfo() override;
	~Decoder();
};