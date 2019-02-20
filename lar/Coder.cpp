#include<iostream>
#include "Coder.h"
#include "BinTools.h"

using namespace std;

Coder& Encoder::doCoder(std::string input,std::string output)
{
	HEAD head = genHead(input);
	char name[N]; initName(name);

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

void Coder::initName(char name[])
{
	for (int i = 0; i < N; i++) {
		name[i] = i;
	}
}


Coder& Decoder::doCoder(std::string input,std::string output)
{
	
	FILE * in = fopen(input.c_str(), "rb");
	FILE * out = fopen(output.c_str(), "wb");

	HEAD head;
	fread(&head, sizeof(head), 1, in);

	char name[N]; initName(name);

	Haffman::Tree tree(head.weight, name, N);
	Haffman::Node* node = tree.getNode();

	BinReader reader(in);

	int8_t c = 0;
	int hasWriteBytes = 0, hasReadBits = 0;
	int nodeIndex = 2 * N - 2;
	while((c=reader.readBit()) != EOF){
		hasReadBits++;
		if (c == 0) {
			nodeIndex = node[nodeIndex].leftChild;
		}
		else {
			nodeIndex = node[nodeIndex].rightChild;
		}

		if (nodeIndex < N) {
			char ch = node[nodeIndex].chr;
			fwrite(&ch, sizeof(char), 1, out);
			nodeIndex = 2 * N - 2;
			hasWriteBytes++;
			if(hasWriteBytes == head.length){
				// 到达指定的字节数后直接退出, 之后的bit可能是因为需要对齐到byte而产生的无效bit
				break;
			}
		}
	}
	
	fclose(in);
	fclose(out);

	int size = hasReadBits % 8 == 0 ? hasReadBits / 8 : hasReadBits / 8 + 1;
	size += sizeof(HEAD);
	printf("文件大小为%d字节\n", size);
	printf("解压后大小为%d字节\n", head.length);
	printf("压缩比为%.2f%%\n", 100 * (double)size / head.length);

	return *this;
}

void Decoder::printInfo()
{
	
}

Decoder::~Decoder()
{

}