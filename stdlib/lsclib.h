/**
 * [函数名] exit
 * [效果] 结束程序,返回指定的返回码
 * [参数] code : 指定的返回码
 * [返回值] 无 
 */
void exit(int code);

/**
 * [函数名] lscmalloc
 * [效果] 分配制定大小的内存空间
 * [参数] size : 指定的内存空间的大小
 * [返回值] 分配的空间的首地址
 */
char* lscmalloc(int size);

/**
 * [函数名] lscfree
 * [效果] 释放分配的空间
 * [参数] size : 有lscmalloc分配的空间首地址
 * [返回值] 无
 */
void lscfree(char* addr);

/**
 * [函数名] lscstr2int
 * [效果] 将字符串转化为数字
 * [参数] str   : 存放待转换的字符串
 * [参数] maxLen: buf长度
 * [返回值] 字符串对应的数字
 */
int lscstr2int(char* buf,int maxLen);

/**
 * [函数名] lscint2str
 * [效果] 将数字转化为字符串
 * [参数] buf  : 存放数字转换后的字符串
 * [参数] n    : 待转换的数字
 * [返回值] 无 
 */
void lscint2str(char* buf,int n);