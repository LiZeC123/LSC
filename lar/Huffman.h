#pragma once

namespace Haffman {
	const int MaxValue = 0x7fffffff;//初始设定的权值最大值(int范围内的最大值)
	const int MaxBit = 300;//初始设定的最大编码位数

	struct Node//哈夫曼树的结点结构
	{
		int weight;//权值
		char chr;//字符的名称 
		int flag;//标记该节点是否已加入树中 
		int parent;//双亲结点下标
		int leftChild;//左孩子下标
		int rightChild;//右孩子下标
	};

	struct Code//存放哈夫曼编码的数据元素结构
	{
		int bit[MaxBit];//存放编码数组
		char chr;//字符的名称 
		int end;//编码的结束下标
		int weight;//字符的权值
	};

	class Tree {
	public:
		//根据权值和名称生成哈夫曼树
		Tree(int weight[], char chr[], int num);
		Tree(const Tree&) = delete;
		//根据哈夫曼树生成哈夫曼编码
		Code* getCode();
		Node* getNode();

		void printCode();
		
		~Tree();
	private:
		void genTree(int weight[], char chr[]);
		void genCode();

	private:
		const int n;  // 节点数量
		Node* haffTree;
		Code* haffCode;
	};
}