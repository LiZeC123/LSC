#pragma once

struct HEAD//文件头
{
	char type[4] = "HUF";//文件类型
	int length;//源文件长度(bit数)
	int weight[256] = { 0 };//权值表
};
