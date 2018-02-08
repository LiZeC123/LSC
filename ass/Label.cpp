#include "Label.h"
using namespace std;

int Label::currAddr = 0x00000000;

Label::Label(string name,bool ex)
{
    
}

Label::Label(string name,int value)
{

}

Label::Label(string name,int times,int len,std::vector<int> cont)
{

}

string Label::getSegName()
{
    return segName;
}

string Label::getName()
{
    return lbName;
}

int Label::getAddr()
{
    return addr;
}

int Label::getTimes()
{
    return times;
}