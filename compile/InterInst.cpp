#include "InterInst.h"

using namespace std;


void InterInst::printSelf()
{
    //TODO: 打印指令
}

void InterCode::addInst(InterInst* inst)
{
    code.push_back(inst);
}

vector<InterInst*>& InterCode::getCode()
{
    return code;
}

void InterCode::printSelf()
{
    for(auto&i:code){
        i->printSelf();
    }
}

InterCode::~InterCode()
{
    for(auto& i:code){
        delete i;
    }
}

