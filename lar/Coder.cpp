#include<iostream>
#include "Coder.h"
#include "BinTools.h"

using namespace std;

Encoder& Encoder::addFile(std::string input)
{
	files.push_back(input);
	return *this;
}

Encoder& Encoder::doCoder(std::string output)
{
	initHeads();
	char name[N]; initName(name);

	Haffman::Tree tree(head.weight,name,N);
	Haffman::Code* code = tree.getCode();

	FILE* out = fopen(output.c_str(),"wb");
	writeHeads(out);
	writeBody(code,out);

	fclose(out);

	return *this;
}


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
	size += sizeof(Lar::Head) + head.length*sizeof(Lar::Item); 
	printf("文件大小为%d字节\n", head.length);
	printf("压缩后大小为%d字节\n", size);
	printf("压缩比为%.2f%%\n", 100 * (double)size / head.length);

	return *this;
}


void Encoder::printInfo()
{
	int size = bitCount % 8 == 0 ? bitCount / 8 : bitCount / 8 + 1;
	size += sizeof(Lar::Head) + head.itemNum*sizeof(Lar::Item); 
	for(const auto&file:files){
		// 文件名实际占用空间还要加上末尾的\0
		size += (file.size()+1);
	}
	
	int length = 0;
	for(int i=0;i<head.itemNum;i++){
		length += itemTable[i].length;
	}

	printf("文件大小为%d字节\n", length);
	printf("压缩后大小为%d字节\n", size);
	printf("压缩比为%.2f%%\n", 100 * (double)size / length);
}

Encoder::~Encoder()
{
	delete[] itemTable;
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

void Encoder::initHeads()
{
	head.itemNum = files.size();
	itemTable = new Lar::Item[head.itemNum];
	for(unsigned int idx = 0;idx < files.size();idx++){
		FILE* in = fopen(files[idx].c_str(),"rb");
		if (in != nullptr) {
			int ch;
			while ((ch = fgetc(in)) != EOF)
			{
				head.weight[ch]++;
				itemTable[idx].length++;
			}
			fclose(in);
		}
		else {
			printf("无法访问文件%s",files[idx].c_str());
		}
	}
}

void Encoder::writeHeads(FILE* out)
{
	fwrite(&head, sizeof(Lar::Head), 1, out);
	fwrite(itemTable,sizeof(Lar::Item),head.itemNum,out);
	for(auto& file:files){
		// 将末尾的\0也写入
		fwrite(file.c_str(),sizeof(char),file.size()+1,out);
	}
}

void Encoder::writeBody(Haffman::Code* code, FILE* out)
{
	BinWriter writer(out);

	for(auto& file: files){
		FILE* in = fopen(file.c_str(),"rb");
		if(in == nullptr){
			printf("无法访问文件%s",file.c_str());
			continue;
		}

		int ch;
		while ((ch = fgetc(in)) != EOF)
		{
			for (int j = 0; j < code[ch].end; j++)
			{
				writer.writeBit(code[ch].bit[j]);
				bitCount++;
			}
		}
		fclose(in);
	}

	writer.flush();	
}


void Coder::initName(char name[])
{
	for (int i = 0; i < N; i++) {
		name[i] = i;
	}
}

Decoder& Decoder::doCoder(std::string input)
{
	FILE * in = fopen(input.c_str(), "rb");
	readHeads(in);
	
	char name[N]; initName(name);

	Haffman::Tree tree(head.weight, name, N);
	Haffman::Node* node = tree.getNode();

	BinReader reader(in);
	for(int i=0;i<head.itemNum;i++){
		writeFile(reader,node, i);
	}

	fclose(in);

	return *this;
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
	int size = hasReadBits % 8 == 0 ? hasReadBits / 8 : hasReadBits / 8 + 1;
	size += sizeof(Lar::Head) + head.itemNum*sizeof(Lar::Item); 
	for(int i=0;i<head.itemNum;i++){
		size += (files[i].size()+1);
	}

	int length = 0;
	for(int i=0;i<head.itemNum;i++){
		length += itemTable[i].length;
	}

	printf("文件大小为%d字节\n", size);
	printf("解压后大小为%d字节\n", length);
	printf("压缩比为%.2f%%\n", 100 * (double)size / length);
}

Decoder::~Decoder()
{
	delete[] itemTable;
	delete[] files;
}

void Decoder::readHeads(FILE* in)
{
	fread(&head, sizeof(Lar::Head), 1, in);

	itemTable = new Lar::Item[head.itemNum];
	fread(itemTable,sizeof(Lar::Item),head.itemNum, in);

	const int BUF_MAX = 256;
	char buf[BUF_MAX];
	files = new string[head.itemNum];
	for (int i = 0; i < head.itemNum; i++){
		int ch, pos = 0;
		while ((ch = fgetc(in)) != '\0'){
			buf[pos++] = ch;
		}
		buf[pos] = '\0';  // 结束当前字符串,避免被未初始化的数据干扰
		files[i] = string(buf);
	}
}

void Decoder::writeFile(BinReader& reader, Haffman::Node* node, int fileIdx)
{
	FILE* out = fopen(files[fileIdx].c_str(),"wb");

	int hasWriteBytes = 0;
	int nodeIndex = 2 * N - 2;
	int8_t c = 0;
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
			if(hasWriteBytes == itemTable[fileIdx].length){
				// 到达指定的字节数后直接退出, 之后的bit可能是因为需要对齐到byte而产生的无效bit
				break;
			}
		}
	}
}