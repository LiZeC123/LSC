/**
 * [函数名] lscRead
 * [效果] 从标准输入读取至多maxLen个字符
 *       lscRead函数会一直读取,直到满足一下两个条件之一就停止
 *       1. 读取到换行符'\n'
 *       2. 到达最大长度
 *       如果因为换行符导致读取结束,则读取的字符串中也包含换行符
 * [参数] buf   : 存入读取字符串的缓冲区
 * [参数] maxLen: 最大读取长度
 * [返回值] 无 
 */
void lscRead(char* buf,int maxLen);

/**
 * [函数名] lscWrite
 * [效果] 向标准输出写入len个字符
 * [参数] buf   : 存放待写入字符串的缓冲区
 * [参数] len   : 最大读取长度
 * [返回值] 无 
 */
void lscWrite(char* buf,int len);

/**
 * [函数名] lscPrintStr
 * [效果] 向标准输出写入字符串
 * [参数] str  : 以\0结尾的C风格字符串
 * [返回值] 无 
 */
void lscPrintStr(char* str);

/**
 * [函数名] lscPrintInt
 * [效果] 向标准输出写入一个数字
 * [参数] n    : 待输出的数字
 * [返回值] 无 
 */
void lscPrintInt(int n);

/**
 * [函数名] lscReadInt
 * [效果] 从标准输入读取一个数字
 * [返回值] 读取到的数字
 */
int lscReadInt();
