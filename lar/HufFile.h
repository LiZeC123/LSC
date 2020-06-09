#pragma once

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
