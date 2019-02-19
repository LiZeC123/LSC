#include<iostream>
#include "Coder.h"

using namespace std;

Coder& Encoder::loadData(std::string filename)
{
	head = genHead(filename);
	char name[N];
	for (int i = 0; i < N; i++) {
		name[i] = i;
	}

	Haffman::Tree tree(head.weight, name, N);
	Haffman::Code* code = tree.getCode();

	// TODO: 选择合适的位置控制输出
	// tree.printCode();

	bufSize = getBufSize(code);
	buf = new char[bufSize];

	fillBuf(filename, buf, code);

	return *this;
}

Coder& Encoder::writeToFile(std::string filename)
{
	FILE * out = fopen(filename.c_str(), "wb");
	fwrite(&head, sizeof(HEAD), 1, out);

	int i;
	for (i = 0; i < bufSize; i += 8) {//8bit组成1byte
		char byte = 0;
		for (int j = 0; j < 8; j++) {
			byte <<= 1;//先移位避免移位次数错误
			if (buf[i + j] == '0') {
				byte |= 0x0;
			}
			else {
				byte |= 0x1;
			}
		}
		fwrite(&byte, sizeof(char), 1, out);
	}

	//写入剩下不足1byte的bit
	if (i != bufSize) {
		i -= 8;
		char byte = 0;
		for (; i < bufSize; i++) {
			byte <<= 1;
			if (buf[i] == '0') {
				byte |= 0x0;
			}
			else {
				byte |= 0x1;
			}
		}
		byte <<= (bufSize - i);
		fwrite(&byte, sizeof(char), 1, out);
		printf("最后移动%d次\n", bufSize - i);
	}

	fclose(out);
	return *this;
}

void Encoder::printInfo()
{
	int size = bufSize % 8 == 0 ? bufSize / 8 : bufSize / 8 + 1;
	printf("文件大小为%d字节\n", head.length);
	printf("压缩后大小为%d字节\n", size);
	printf("压缩比为%.2f%%\n", 100 * (double)size / head.length);
}

Encoder::~Encoder()
{
	delete[] buf;
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

void Encoder::fillBuf(std::string filename, char buf[], Haffman::Code code[])
{
	FILE * in = fopen(filename.c_str(), "rb");
	int ch;
	int pos = 0;
	while ((ch = fgetc(in)) != EOF)
	{
		for (int j = 0; j < code[ch].end; j++) {
			buf[pos++] = code[ch].bit[j] + '0';
		}
	}
	fclose(in);
}

int Coder::getBufSize(Haffman::Code cd[])
{
	int nSize = 0;
	for (int i = 0; i < N; i++) {
		nSize += cd[i].weight * cd[i].end;
	}

	return nSize;
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
