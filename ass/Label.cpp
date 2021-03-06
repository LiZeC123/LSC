#include "Label.h"
#include "Generator.h"
using namespace std;

int Label::currAddr = 0x00000000;
string Label::currSegName = "";

Label::Label(string name,bool ex,bool global)
{
    segName = currSegName;
    lbName = name;
    equed  = false;
    externed = ex;
    this->global = global; 
    addr = currAddr;
    times = 0;
    len = 0;

    if(ex){
        segName = "";
        addr = 0;
    }
}

Label::Label(string name,int value)
{
    segName = currSegName;
    lbName = name;
    equed  = true;
    externed = false;
    global = false; 
    addr = value;
    times = 0;
    len = 0;
}

Label::Label(string name,int times,int len,std::vector<int> cont)
{
    segName = currSegName;
    lbName = name;
    equed  = false;
    externed = false;
    global = false; 
    addr = currAddr;
    this->times = times;
    this->len = len;
    this->cont = cont;
    currAddr += times*len*cont.size();  // 计算实际占用的空间
}

void Label::setGlobal(bool global)
{
    this->global = global;
}

string Label::getSegName()
{
    return segName;
}

string Label::getName()
{
    return lbName;
}

bool Label::isEqu()
{
    return equed;
}

bool Label::isExterned()
{
    return externed;
}

bool Label::isGlobal()
{
    return global;
}

int Label::getAddr()
{
    return addr;
}

int Label::getTimes()
{
    return times;
}

int Label::getLen()
{
    return len;
}

const std::vector<int>& Label::getCont()
{
    return cont;
}

 void Label::write(FILE* fout)
 {
     for(int i=0;i<times;i++){
         for(auto& value:cont){
            fwrite(&value,len,1,fout);
         }
     }
 }

void Label::printSelf()
{
    printf("%s",externed?"Ex ":"   ");
    printf("%s",global?"G  ":"   ");
    printf("%s",equed?"Eq ":"   ");
    printf("%5s/%-11s ",segName.c_str(),lbName.c_str());
    printf("len = %1d times = %3d addr = 0x%-8x",len,times,addr);
    if(!cont.empty()){
        printf("src = ");
        for(auto i:cont){
            printf("%c",i);
        }
    }
    printf("\n");
}