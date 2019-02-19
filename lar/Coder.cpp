#include<iostream>
#include "Coder.h"
#include "BinTools.h"

using namespace std;

Coder& Encoder::doCoder(std::string input,std::string output)
{
	HEAD head = genHead(input);
	char name[N];
	for (int i = 0; i < N; i++) {
		name[i] = i;
	}

	Haffman::Tree tree(head.weight, name, N);
	Haffman::Code* code = tree.getCode();

	FILE* in = fopen(input.c_str(),"rb");
	FILE* out = fopen(output.c_str(),"wb");
	fwrite(&head, sizeof(HEAD), 1, out);
	
	BinWriter writer(out);

	int ch;
	int bitCount = 0;
	while ((ch = fgetc(in)) != EOF)
	{
		for (int j = 0; j < code[ch].end; j++) {
			writer.writeBit(code[ch].bit[j]);
			bitCount++;
		}
	}
	writer.flush();
	fclose(in);
	fclose(out);

	int size = bitCount % 8 == 0 ? bitCount / 8 : bitCount / 8 + 1;
	size += sizeof(HEAD);
	printf("文件大小为%d字节\n", head.length);
	printf("压缩后大小为%d字节\n", size);
	printf("压缩比为%.2f%%\n", 100 * (double)size / head.length);

	return *this;
}


void Encoder::printInfo()
{
	// int size = bufSize % 8 == 0 ? bufSize / 8 : bufSize / 8 + 1;
	// size += sizeof(HEAD);
	// printf("文件大小为%d字节\n", head.length);
	// printf("压缩后大小为%d字节\n", size);
	// printf("压缩比为%.2f%%\n", 100 * (double)size / head.length);
}


HEAD Encoder::genHead(std::string filename)
{
	HEAD head;
	FILE * in = fopen(filename.c_str(), "rb");
	if (in != nullptr) {
		int ch;
		head.length = 0;
		while ((ch = fgetc(in)) != EOF)
		{
			head.weight[ch]++;
			head.length++;
		}
		fclose(in);
	}

	return head;
}


int Coder::getBufSize(Haffman::Code cd[])
{
	int nSize = 0;
	for (int i = 0; i < N; i++) {
		nSize += cd[i].weight * cd[i].end;
	}

	return nSize;
}

Coder& Decoder::doCoder(std::string input,std::string output)
{
	return *this;
}

Coder & Decoder::loadData(std::string filename)
{
	head = readHead(filename);

	char name[N];
	for (int i = 0; i < N; i++) {
		name[i] = i;
	}

	tree = new Haffman::Tree(head.weight, name, N);
	Haffman::Code* code = tree->getCode();

	bufSize = getBufSize(code);
	buf = new char[bufSize];

	fillBuf(filename);

	return *this;
}

Coder& Decoder::writeToFile(std::string filename)
{
	FILE * out = fopen(filename.c_str(), "wb");
	Haffman::Node* tree = this->tree->getNode();
	int length = head.length;
	int count = 0;
	int treeIndex = 2 * N - 2;
	for (int pos = 0; pos<bufSize; pos++)
	{
		if (buf[pos] == 0) {
			treeIndex = tree[treeIndex].leftChild;
		}
		else {
			treeIndex = tree[treeIndex].rightChild;
		}

		if (treeIndex < N) {
			char ch = tree[treeIndex].chr;
			fwrite(&ch, sizeof(char), 1, out);
			treeIndex = 2 * N - 2;
			count++;
			if(count == length){
				// 编码过程中, 可能最后bit数并非8的倍数, 使得最后一个byte填充了一些0
				// 在解码过程中, 这些0可能导致额外的字节
				printf("当前pos=%d 当前bufSize=%d\n",pos,bufSize);
				break;
			}
		}
	}

	fclose(out);

	return *this;
}

void Decoder::printInfo()
{
}

Decoder::~Decoder()
{
	delete[] buf;
	delete tree;
}

HEAD Decoder::readHead(std::string filename)
{
	HEAD head;
	head.type[0] = '\0';
	FILE * in = fopen(filename.c_str(), "rb");
	if (in != nullptr) {
		fread(&head, sizeof(head), 1, in);
		fclose(in);
	}

	return head;
}

void Decoder::fillBuf(std::string filename)
{
	FILE * in = fopen(filename.c_str(), "rb");

	HEAD head;
	fread(&head, sizeof(head), 1, in);

	int ch;
	int pos = 0;
	while ((ch = fgetc(in)) != EOF && pos + 7 < bufSize)
	{
		for (int j = 7; j >= 0; j--) {
			buf[pos + j] = ch & 0x1;
			ch >>= 1;
		}
		pos += 8;
	}

	fclose(in);
}
