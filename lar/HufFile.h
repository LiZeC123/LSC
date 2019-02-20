#pragma once

struct HEAD//文件头
{
	char type[4] = "HUF";//文件类型
	int length;//源文件长度(bit数)
	int weight[256] = { 0 };//权值表
};

namespace Lar
{
	struct Head
	{
		char magic[4] = {'.', 'L', 'A', 'R'};
		int32_t itemNum;	// 合并的文件数量, 每个文件对应一个Item
		int weight[256] = {0};
	};

	struct Item
	{
		// 32bit, 从而单个文件的最大长度为4GB
		int32_t length;
	};

} 
