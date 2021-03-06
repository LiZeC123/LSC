#include <string>
#include <iostream>
#include <vector>
#include "HufFile.h"
#include "Huffman.h"
#include "Coder.h"

using namespace std;

class Args {
public:
	bool isEncode = false;
	bool isDecode = false;
	bool printHelp = false;
	bool printInfo = false;
	bool isSetOutput = false;
	string larFileName;
	vector<string> filenames;
};

Args analyseOptions(int argc, char* argv[])
{
    Args args;
    for(int i=1; i < argc; i++){
        string option(argv[i]);
		if(option == "-c"){
			args.isEncode = true;
		}
        else if(option == "-x"){
			args.isDecode = true;
        }
        else if(option == "-h"){
			args.printHelp = true;
        }
		else if(option == "-v"){
			args.printInfo = true;
		}
        else if(option == "-f"){
			args.isSetOutput = true;
			if(i+1<argc){
				args.larFileName = string(argv[i+1]);
				i = i + 1;
			}
			else{
				printf("选项: -f 后必须指定文件名\n");
			}
        }
        else {
			args.filenames.push_back(argv[i]);
        }
    }

    return args;
}

string simpleReplaceName(string fullName) {
	size_t pos = fullName.rfind(".");
	if(pos > 0 && pos == fullName.size() - 2){
		return fullName.substr(0,pos) + ".lar";
	}
	else{
		return fullName + ".lar";
	}
}


//const int N = 256;

void printHelpInfo()
{
	printf("指令格式 lar [options] file ...\n");
	printf("Options:\n");
	printf("  -h 显示此信息\n");
	printf("  -c 压缩文件\n");
	printf("  -x 解压缩文件\n");
	printf("  -f 指定压缩文件名\n");
	printf("  -v 输出详细信息\n");	
}


void encode(vector<string> inputNames, string outputName)
{
	Encoder encoder;
	for(const auto& file: inputNames){
		encoder.addFile(file);
	}
	encoder.doCoder(outputName).printInfo();
}

void decode(string larFileName)
{
	Decoder().doCoder(larFileName).printInfo();
}



int main(int argc,char* argv[])
{
	if(argc == 1){
		printf("输入文件不能为空, 输入 lar -h 查看帮助\n");
        return 0;
    }

	Args args = analyseOptions(argc,argv);
	if(args.printHelp){
		printHelpInfo();
		return 0;
	}
	else if(args.isDecode){
		if(args.isSetOutput){
			decode(args.larFileName);
		}
		else{
			decode(simpleReplaceName(args.larFileName));
		}
	}
	else if(args.isEncode){
		if(args.isSetOutput){
			encode(args.filenames,args.larFileName);
		}
		else{
			encode(args.filenames,simpleReplaceName(args.larFileName));
		}
	}
}