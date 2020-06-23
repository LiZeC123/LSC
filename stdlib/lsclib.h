/**
 * [函数名] exit
 * [效果] 结束程序,返回指定的返回码
 * [参数] code : 指定的返回码
 * [返回值] 无 
 */
void exit(int code);

/**
 * [函数名] lscmalloc
 * [效果] 分配指定大小的内存空间
 * [参数] size : 指定的内存空间的大小
 * [返回值] 分配的空间的首地址
 */
char* lscmalloc(int size);

/**
 * [函数名] lscfree
 * [效果] 释放分配的空间
 * [参数] size : 由lscmalloc分配的空间首地址
 * [返回值] 无
 */
void lscfree(char* addr);


/**
 * [函数名] lscp2i
 * [效果] 将指针类型转化为数值类型
 * [参数] point : 需要获得取值的指针
 * [返回值] 指针的具体取值
 */
int lscp2i(int* point);

// 考虑补充随机函数, 测试用例中应该考虑添加标准库的相关测试
// 文件读取相关函数, 