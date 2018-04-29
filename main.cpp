#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <unistd.h>
#define SIZE 256

//#define __LSC_DEBUG__
#define __LSC_VERSION__ "1.0.0"

using namespace std;

// 手动取出以下函数的定义,从而避免vscode的语法检查无法识别头文件函数定义的问题
extern char *getcwd (char *__buf, size_t __size) __THROW __wur;
extern int access (const char *__name, int __type) __THROW __nonnull ((1));
extern ssize_t readlink (const char *__restrict __path,
			 char *__restrict __buf, size_t __len)
     __THROW __nonnull ((1, 2)) __wur;


class CompileFile
{
public:
	static string currPath;
	CompileFile(string fullname);
	CompileFile(const char* fullname);
	const string& getCoreName() const;
	const string& getType() const;
	const string& getFullName() const;
	static string getCurrWorkPath();	// 获得当前路径
private:
	string fullName;
	string coreName;
	string type;
};


string CompileFile::currPath = CompileFile::getCurrWorkPath();


CompileFile::CompileFile(string name)
{
	if(name[0] != '/'){
		fullName = currPath + "/" + name;
	}

	size_t pos = fullName.rfind(".");
	if(pos > 0 && pos == fullName.size() - 2){
		coreName = fullName.substr(0,pos);
		type = fullName.substr(pos);	
	}
	else{
		coreName = name;
		type = "";
	}
}

CompileFile::CompileFile(const char* name) : CompileFile(string(name))
{ }

string CompileFile::getCurrWorkPath()
{
	char cpath[SIZE];
	getcwd(cpath,SIZE);
	return string(cpath);
}

const string& CompileFile::getCoreName() const
{
	return coreName;
}

const string& CompileFile::getType() const
{
	return type;
}

const string& CompileFile::getFullName() const
{ 
	return fullName;
}


class Args
{
public:
	bool isHelped = false;
	bool isSetOutput = false;
	string outputName;
};


// 获得本程序对应的可执行文件实际的位置
string getFilePath()
{
	char current_absolute_path[SIZE];
	//获取当前程序绝对路径
	int cnt = readlink("/proc/self/exe", current_absolute_path, SIZE);
	if (cnt < 0 || cnt >= SIZE)
	{
		printf("获取可执行文件路径失败\n");
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



// 检查是否是合法的输入文件
bool checkType(const string& type)
{
	static string list[] = {".c",".i",".s",".o"};
	for(const auto& t: list){
		if(t == type){
			return true;
		}
	}

	return false;
}

// 执行一条命令行指令,
// 命令所在路径, 命令名(非全局命令,需要在开头加上/), 命令参数
void execCmd(const string& path, const string& cmd, const string& file)
{
	static char cmdBuf[2048];
	sprintf(cmdBuf, "%s%s %s", path.c_str(),cmd.c_str(),file.c_str());
	system(cmdBuf);

#ifdef __LSC_DEBUG__
	printf("Run Shell Command -> %s\n",cmdBuf);
#endif
}

void printHelpInfo()
{
	printf("Lsc %s (on %s,%s)\n", __LSC_VERSION__,__DATE__, __TIME__);
	printf("[GCC %d.%d.%d] on Ubuntu\n",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
	printf("Lsc iS Compile!\n\n");
	printf("指令格式 lsc [options] file ...\n");
	printf("-h 显示此信息\n");
	printf("-o 指定输出文件名\n");
	//printf("");
	//printf("");
}

void analyseOptions(Args& args, int argc, char* argv[])
{
	for(int i=0;i<argc;i++){
		if(argv[i][0] == '-'){
			string option(argv[i]);

			if(option.size() == 1){
				printf("选项不能为空\n");
			}
			else if(option[1] == 'o'){
				args.isSetOutput = true;
				if((i+1) < argc){
					args.outputName = string(argv[i+1]);
				}
				else{
					printf("选项: -o 后必须指定文件名\n");
				}
			}
			else if(option[1] == 'h') {
				args.isHelped = true;
				printHelpInfo();
			}
			else{
				printf("选项: %s 无效\n",argv[i]);
			}
		}
	}
}

void analyseFile(vector<CompileFile>& compilefiles, int argc, char* argv[])
{
    for(int i=1;i<argc;i++){
		if(argv[i][0] != '-'){
			if(argv[i-1] == string("-o")){
				// 此文件是通过-o参数指定的输出文件, 因此忽略
				continue;
			}

			CompileFile cfile = CompileFile(argv[i]);
			if(!checkType(cfile.getType())){
				printf("%s%s不是正确格式的文件, 已自动跳过此文件"
					, cfile.getCoreName().c_str()
					, cfile.getType().c_str());
			}
			else{
				compilefiles.push_back(cfile);
			}
		}
    }
}


int main(int argc,char* argv[])
{
	if(argc == 1){
        return 0;
    }

	Args args;
	vector<CompileFile> compilefiles;

	analyseOptions(args,argc,argv);
	analyseFile(compilefiles,argc,argv);


	if(args.isHelped && compilefiles.size()==0){
		return 0;
	}


   
   string exePath = getFilePath();
	// 编译, 汇编
	for(const auto& file: compilefiles){
		if(file.getType() == ".c"){
			execCmd(exePath,"/preproc/lscp", file.getCoreName()+".c");
			execCmd(exePath,"/compile/lscc", file.getCoreName()+".i");
			execCmd(exePath,"/ass/lsca", file.getCoreName()+".s");
		}
		else if(file.getType() == ".i"){
			execCmd(exePath,"/compile/lscc", file.getCoreName()+".i");
			execCmd(exePath,"/ass/lsca", file.getCoreName()+".s");
		}
		else if(file.getType() == ".s") {
			execCmd(exePath,"/ass/lsca", file.getCoreName()+".s");
		}
	}

	// 链接
	string allfiles = exePath + "/stdlib/start.o " + exePath + "/stdlib/stdlib.o ";
	for(const auto& file: compilefiles){
		allfiles += (file.getCoreName()+ ".o ");
	}
	execCmd(exePath,"/lit/lscl", allfiles);
	execCmd("","chmod +x z.out","");

	if(args.isSetOutput){
		execCmd("","mv z.out "+args.outputName,"");
	}
}

