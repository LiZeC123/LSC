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
    std::string filename;
    std::ifstream cfile;
    std::ofstream ifile;
    int rowNum = 0;
    void doCmd(std::ofstream& ifile, const std::string& line);
    std::vector<std::string> spiltCmd(const std::string& line);


    void doInclude(std::ofstream& ifile, std::string includeFile,bool isStd);
};