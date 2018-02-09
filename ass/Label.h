#pragma once
#include <string>
#include <vector>

using std::string;
class Label
{
public:
    Label(string name,bool ex = false);                         // 标签或外部符号
    Label(string name,int value);                               // 宏符号
    Label(string name,int times,int len,std::vector<int> cont); // 数据符号
    
    string getSegName();
    string getName(); 
    int getAddr();
    int getTimes();

    void printSelf();
private:
    static int currAddr;
    static string currSegName;

    std::string segName;    // 所在段名
    std::string lbName;     // 标签名
    bool equed;             // 是否是宏
    bool externed;          // 是否是外部符号
    bool global;            // 是否是全局符号
    int addr;               // 符号逻辑地址
    int times;              // 重复次数
    int len;                // 单位内存大小
    std::vector<int> cont;  // 符号内容
};