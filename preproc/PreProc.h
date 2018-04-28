#pragma once
#include <fstream>
#include <string>
#include <vector>

class PreProc
{
public:
    PreProc(std::string& cfilename,std::string& ifilename);

    void anaylse();

private:
    int rowNum = 0;
    std::string filename;
    std::string path;

    std::ifstream cfile;
    std::ofstream ifile;
    
    void doCmd(std::ofstream& ifile, const std::string& line);
    std::vector<std::string> spiltCmd(const std::string& line);

    void doInclude(std::ofstream& ifile, std::string includeFile,bool isStd);
    void doDefine(std::ofstream& ifile, std::string& name, std::string value);
};