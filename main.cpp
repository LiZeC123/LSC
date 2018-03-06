#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <unistd.h>
#define SIZE 256

using namespace std;

// 手动取出以下函数的定义,从而避免vscode的语法检查无法识别头文件函数定义的问题
extern char *getcwd (char *__buf, size_t __size) __THROW __wur;
extern int access (const char *__name, int __type) __THROW __nonnull ((1));
extern ssize_t readlink (const char *__restrict __path,
			 char *__restrict __buf, size_t __len)
     __THROW __nonnull ((1, 2)) __wur;
string getCurrWordPath();
string getFilePath();
void execCmd(const char* cmdBuf);

int main(int argc,char* argv[])
{
	if(argc == 1){
        return 0;
    }

	string currPath = getCurrWordPath();
	string filePath = getFilePath();

	vector<string> fileNames;
	vector<string> coreNames;
    for(int i=1;i<argc;i++){
		string file = string(argv[i]);

		if(file[0] != '/'){
			file = currPath + "/" + file;
		}

		fileNames.push_back(file);
		size_t pos = file.rfind(".c");
		if(pos > 0 && pos == file.size() - 2){
			coreNames.push_back(file.substr(0,pos));
		}
		else{
			// 非.c文件,给出警告
			//coreNames.push_back(file);
		}

    }
   
	char cmdBuf[1024];
	for(auto name:fileNames){
		sprintf(cmdBuf,"%s/compile/lscc %s",filePath.c_str(),name.c_str());
		execCmd(cmdBuf);
	}

	for(auto name:coreNames){
		sprintf(cmdBuf,"%s/ass/lsca %s.s",filePath.c_str(),name.c_str());
		execCmd(cmdBuf);
	}

	string allfiles = filePath + "/stdlib/start.o " + filePath + "/stdlib/stdlib.o ";
	for(auto name:coreNames){
		allfiles += (name + ".o ");

	}
	sprintf(cmdBuf,"%s/lit/lscl %s",filePath.c_str(),allfiles.c_str());
	execCmd(cmdBuf);

	execCmd("chmod +x z.out");


}


string getCurrWordPath()
{
	char cpath[SIZE];
	getcwd(cpath,SIZE);
	return string(cpath);
}

void execCmd(const char* cmdBuf)
{
	printf("%s\n",cmdBuf);
	system(cmdBuf);
}

string getFilePath()
{
	char current_absolute_path[SIZE];
	//获取当前程序绝对路径
	int cnt = readlink("/proc/self/exe", current_absolute_path, SIZE);
	if (cnt < 0 || cnt >= SIZE)
	{
		printf("***Error***\n");
		exit(-1);
	}
	//获取当前目录绝对路径，即去掉程序名
	int i;
	for (i = cnt; i >= 0; --i)
	{
		if (current_absolute_path[i] == '/')
		{
			current_absolute_path[i + 1] = '\0';
			break;
		}
	}
	return string(current_absolute_path);
}