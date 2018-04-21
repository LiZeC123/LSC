#include "PreProc.h"
#include "Error.h"
#include <iostream>

using namespace std;

PreProc::PreProc(string& cfilename, string& ifilename) :
    cfile(cfilename), ifile(ifilename)
{ 
    filename = cfilename;
    if(!cfile){
        Error::preError(SOURCE_FILE_FAIL,filename,0);
    }

    if(!ifile){
        Error::preError(OUTPUT_FILE_FAIL,filename,0);
    }
}


void PreProc::anaylse()
{
    string line;
    while(getline(cfile,line)){
        ++rowNum;
        if(line[0] == '#'){
            doCmd(ifile,line);
        }
        else{
            cout << line << endl;
            ifile << line << endl;
        }
    }


    cfile.close();
    ifile.close();
}


void PreProc::doCmd(ofstream& ifile, const string& line)
{
    if(line.size() == 1){
        Error::preError(EMPTY_COMMAND,filename,rowNum);
    }
    else {
        vector<string> cmdList = spiltCmd(line);
        if(cmdList[1] == "include") {
            if(cmdList[2] == "\""){
                string filename;
                for(size_t i=3;i<cmdList.size();i++){
                    if(cmdList[i] != "\""){
                        filename += cmdList[i];
                    }
                    else{
                        break;
                    }
                }
                doInclude(ifile,filename,false);
            }
            else if(cmdList[2] == "<"){
                string filename;
                for(size_t i=3;i<cmdList.size();i++){
                    if(cmdList[i] != ">"){
                        filename += cmdList[i];
                    }
                    else{
                        break;
                    }
                }
                doInclude(ifile,filename,true);
            }
            else{
                Error::preError(ERROR_FILENAME,filename,rowNum);
            }
        }
        else if(cmdList[1] == "define") {
            // ...
        }
        else{
            Error::preError(ERROR_COMMAND,filename,rowNum);
        }
        
    } 
}



vector<string> PreProc::spiltCmd(const string& line)
{
    vector<string> r;
    unsigned int i = 0;
    while(i < line.size()){
        string word;
        if((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z') || (line[i] == '_')){
            do{
                word.push_back(line[i++]);
            } while((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z'));
            r.push_back(word);
        }
        else if(line[i] == ' '){
            i++;
        }
        else if(line[i] >= '0' && line[i] <= '9'){
            do{
                // 只赋值, 不检查是否为合法的表达式
                word.push_back(line[i++]);
            } while(line[i] == 'x' || line[i] == 'b' || (line[i] >= '0' && line[i] <= '9'));
            r.push_back(word);
        }
        else{
            word.push_back(line[i++]);
            r.push_back(word);
        }
    }

    return r;
}


void PreProc::doInclude(ofstream& ifile, string includeFile,bool isStd)
{
    string filename;
    if(isStd){
        filename = "../stdlib/"+includeFile;
    }
    else{
        filename = includeFile;
    }

    ifstream in(filename);
    if(in){
        string line;
        while(getline(in,line)){
            ifile << line << endl;
        }
    }
    else{
        Error::preError(ERROR_FILENAME,filename,rowNum);
    }
    
}