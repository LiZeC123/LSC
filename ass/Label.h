#pragma once
#include <string>
#include <vector>

using std::string;
class Label
{
public:
    Label(string name,bool ex = false);
    Label(string name,int value);
    Label(string name,int times,int len,std::vector<int> cont);
    
    string getSegName();
    string getName(); 
    int getAddr();
    int getTimes();
private:

    static int currAddr;
    std::string segName;
    std::string lbName;
    bool equed;
    bool externed;
    bool global;
    int addr;
    int times;
    int len;
    std::vector<int> cont;
};