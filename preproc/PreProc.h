#pragma once
#include <fstream>
#include <string>
#include <vector>

class PreProc
{
public:
    PreProc(std::string& cfilename,std::string& ifilename,std::string& stdPath);

    void anaylse();

private:
    std::ifstream cfile;
    std::ofstream ifile;

    int rowNum = 0;
    std::string filename;
    std::string stdPath;    // 标准库路径, 用于分析尖括号引入的头文件的位置关系
    std::string path;       // 文件路径, 用于分析引号引入的头文件的位置关系
    

    void doCmd(std::ofstream& ifile, const std::string& line);
    std::vector<std::string> spiltCmd(const std::string& line);

    void doInclude(std::ofstream& ifile, std::string includeFile,bool isStd);
    void doDefine(std::ofstream& ifile, std::string& name, std::string value);
};