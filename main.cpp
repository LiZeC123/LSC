#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <unistd.h>
#define SIZE 256

//#define __LSC_DEBUG__
#define __LSC_VERSION__ "1.1.0"

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
	bool isOnlyPreproc = false;
	bool isOnlyCompile = false;
	bool isOnlyAss = false;
	bool isSaveTempFile = false;

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
	printf("[GCC %d.%d.%d] on Ubuntu\n\n",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
	
	printf("指令格式 lsc [options] file ...\n");
	printf("-h 显示此信息\n");
	printf("-o 指定输出的可执行文件名\n");
	printf("-E 只进行编译预处理\n");
	printf("-S 只进行编译预处理和编译\n");
	printf("-C 只进行编译预处理,编译和汇编\n");
	printf("-T 保留中间文件\n\n");

	printf("Lsc iS Compile!\n");
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
				if((i+1) < argc){
					args.isSetOutput = true;
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
			else if(option[1] == 'E'){
				args.isOnlyPreproc = true;
			}
			else if(option[1] == 'S'){
				args.isOnlyCompile = true;
			}
			else if(option[1] == 'C'){
				args.isOnlyAss = true;
			}
			else if(option[1] == 'T'){
				args.isSaveTempFile = true;
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

void toIFile(const vector<CompileFile>& compilefiles, const string& exePath)
{
	for(const auto& file: compilefiles){
		if(file.getType() == ".c"){
			string opt = file.getCoreName()+".c " + exePath + "/stdlib";
			execCmd(exePath,"/preproc/lscp", opt);
		}
	}
}

void toSFile(const vector<CompileFile>& compilefiles, const string& exePath)
{
	for(const auto& file: compilefiles){
		if(file.getType() == ".c" || file.getType() == ".i"){
			execCmd(exePath,"/compile/lscc", file.getCoreName()+".i");
		}
	}
}

void toOFile(const vector<CompileFile>& compilefiles, const string& exePath)
{
	for(const auto& file: compilefiles){
		if(file.getType() == ".c"|| file.getType() == ".i" || file.getType() == ".s"){
			execCmd(exePath,"/ass/lsca", file.getCoreName()+".s");
		}
	}
}

void toExeFile(const vector<CompileFile>& compilefiles, const string& exePath)
{
	string allfiles = exePath + "/stdlib/start.o " + exePath + "/stdlib/stdlib.o ";
	for(const auto& file: compilefiles){
		allfiles += (file.getCoreName()+ ".o ");
	}
	execCmd(exePath,"/lit/lscl", allfiles);
	execCmd("","chmod +x z.out","");
}

void clearTempFile(const vector<CompileFile>& compilefiles, const Args& args)
{
	if(args.isOnlyPreproc){
		// 只保留编译预处理文件,则不进行任何删除
		return;
	}
	if(args.isOnlyCompile){
		for(const auto& file: compilefiles){
			execCmd("","rm -f",file.getCoreName()+".i");
		}
	}
	else if(args.isOnlyAss) {
		for(const auto& file: compilefiles){
			execCmd("","rm -f",file.getCoreName()+".i");
			execCmd("","rm -f",file.getCoreName()+".s");
		}
	}
	else{
		for(const auto& file: compilefiles){
			execCmd("","rm -f",file.getCoreName()+".i");
			execCmd("","rm -f",file.getCoreName()+".s");
			execCmd("","rm -f",file.getCoreName()+".o");
		}	
	}
}


int main(int argc,char* argv[])
{
	if(argc == 1){
		printf("输入文件不能为空, 输入 lsc -h 查看帮助\n");
        return 0;
    }

	Args args;
	vector<CompileFile> compilefiles;

	analyseOptions(args,argc,argv);
	analyseFile(compilefiles,argc,argv);


	if(compilefiles.size()==0){
		if(!args.isHelped){
			printf("输入文件不能为空, 输入 lsc -h 查看帮助\n");
		}

		return 0;
	}

   string exePath = getFilePath();
   if(args.isOnlyPreproc) {
	   toIFile(compilefiles,exePath);
   }
   else if(args.isOnlyCompile) {
	   toIFile(compilefiles,exePath);
	   toSFile(compilefiles,exePath);
   }
   else if(args.isOnlyAss) {
	   toIFile(compilefiles,exePath);
	   toSFile(compilefiles,exePath);
	   toOFile(compilefiles,exePath);
   }
   else{
	   toIFile(compilefiles,exePath);
	   toSFile(compilefiles,exePath);
	   toOFile(compilefiles,exePath);
	   toExeFile(compilefiles,exePath);
   }

	if(args.isSetOutput && !args.isOnlyPreproc && !args.isOnlyCompile && !args.isOnlyAss){
		execCmd("","mv z.out "+args.outputName,"");
	}

	if(!args.isSaveTempFile) {
		clearTempFile(compilefiles, args);
	}
}

