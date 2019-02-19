#include <iostream>
#include "Huffman.h"

#define DEBUG

namespace Haffman {
	Tree::Tree(int weight[], char chr[], int num) : 
		n(num), haffTree(new Node[2*num-1]), haffCode(new Code[num])
	{
		genTree(weight, chr);
		genCode();
	}

	Code * Tree::getCode()
	{
		return haffCode;
	}

	Node * Tree::getNode()
	{
		return haffTree;
	}

	void Tree::printCode(){
		for(int i=0;i<n;i++){
			printf("0x%02x %6d ",(unsigned char)haffCode[i].chr,haffCode[i].weight);
			for(int j = 0; j < haffCode[i].end; j++){
				printf("%d",haffCode[i].bit[j]);
			}
			printf("\n");
		}
	}

	Tree::~Tree()
	{
		delete[] haffTree;
		delete[] haffCode;
	}

	void Tree::genTree(int weight[], char chr[])
	{
		int j, m1, m2, x1, x2;
		//哈夫曼树haffTree初始化。n个叶结点的哈夫曼树共有2n-1个结点 
		for (int i = 0; i < 2 * n - 1; i++)
		{
			if (i < n) {
				haffTree[i].weight = weight[i];
				haffTree[i].chr = chr[i];
			}
			else {
				haffTree[i].weight = 0;
				haffTree[i].chr = '!';
			}
			haffTree[i].parent = 0;
			haffTree[i].flag = 0;
			haffTree[i].leftChild = -1;
			haffTree[i].rightChild = -1;
		}
		//构造哈夫曼树haffTree的n-1个非叶结点
		for (int i = 0; i < n - 1; i++)
		{
			m1 = m2 = MaxValue;//保存对应数值的权重 
			x1 = x2 = 0;//x1、x2是用来保存最小的两个值在数组对应的下标
						//DEBUG：此处为0
			for (j = 0; j < n + i; j++)//循环找出所有权重中，最小的二个值
			{
				if (haffTree[j].weight < m1 && haffTree[j].flag == 0)
				{
					m2 = m1;
					x2 = x1;
					m1 = haffTree[j].weight;
					x1 = j;
				}
				else if (haffTree[j].weight < m2 && haffTree[j].flag == 0)
				{
					m2 = haffTree[j].weight;
					x2 = j;
				}
			}
			//将找出的两棵权值最小的子树合并为一棵子树
			haffTree[x1].parent = n + i;
			haffTree[x2].parent = n + i;
			haffTree[x1].flag = 1;
			haffTree[x2].flag = 1;
			haffTree[n + i].weight = haffTree[x1].weight + haffTree[x2].weight;
			haffTree[n + i].leftChild = x1;
			haffTree[n + i].rightChild = x2;
		}
	}
	void Tree::genCode()
	{
		Code cd;
		int child, parent;
		//求n个叶结点的哈夫曼编码
		for (int i = 0; i<n; i++)
		{
			//从0开始计数
			cd.end = 0;
			cd.weight = haffTree[i].weight;//取得编码对应权值的字符 
			cd.chr = haffTree[i].chr;
			child = i;
			parent = haffTree[child].parent;
			//由叶结点向上直到根结点
			while (parent != 0)
			{
				if (haffTree[parent].leftChild == child)
					cd.bit[cd.end] = 0;//左孩子结点编码0
				else
					cd.bit[cd.end] = 1;//右孩子结点编码1

				cd.end++;
				child = parent;
				parent = haffTree[child].parent;
			}
			//保存叶结点的编码和不等长编码的结束位 
			for (int j = cd.end - 1; j >= 0; j--)//end指向的是最后一个字符的下一位 
				haffCode[i].bit[cd.end - j - 1] = cd.bit[j];

			haffCode[i].end = cd.end;
			haffCode[i].weight = cd.weight;//保存编码对应的权值
			haffCode[i].chr = cd.chr;
		}
	}
}

