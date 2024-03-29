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
 * [函数名] lscPrintPointer
 * [效果] 向标准输出写入一个地址值
 * [参数] n    : 待输出的地址
 * [返回值] 无 
 */
void lscPrintPointer(char* p);

/**
 * [函数名] lscReadInt
 * [效果] 从标准输入读取一个数字
 * [返回值] 读取到的数字
 */
int lscReadInt();


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


/**
 * [函数名] lscint2str
 * [效果] 将数字转化为16进制地址值
 * [参数] buf  : 存放数字转换后的字符串
 * [参数] n    : 待转换的数字
 * [返回值] 无 
 */
void lscpointer2str(int value, char* buf);
