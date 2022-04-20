#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <string>
#include <unistd.h>
#define SIZE 256

//#define __LSC_DEBUG__
#define __LSC_VERSION__ "2.1.0"

using namespace std;

// 手动取出以下函数的定义,从而避免vscode的语法检查无法识别头文件函数定义的问题
extern char *getcwd (char *__buf, size_t __size) __THROW __wur;

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
	bool isVersion = false;
	bool isSetOutput = false;
	bool isOnlyCompile = false;
	bool isOnlyAss = false;
	bool isSaveTempFile = false;
	
	string outputName;

	// 模块参数列表
	vector<string> cmp;
	vector<string> ass;
	vector<string> lit;
};

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
	int v = system(cmdBuf);
	if( v != 0 ) {
		printf("执行:%s 返回值为%d, 终止编译过程.\n", cmdBuf, WEXITSTATUS(v));
		exit(0);
	}

#ifdef __LSC_DEBUG__
	printf("Run Shell Command -> %s\n",cmdBuf);
#endif
}

void printVersionInfo()
{
	printf("Lsc %s (on %s, %s)\n", __LSC_VERSION__,__DATE__, __TIME__);
	printf("[GCC %d.%d.%d] on Ubuntu\n",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
	printf("\n");
	printf("Lsc iS Compiler!\n");
}

void printHelpInfo()
{	
	printf("指令格式 lsc [options] file ...\n");
	printf("Options:\n");
	printf("  -h  显示此信息\n");
	printf("  -v  显示编译器版本信息\n");
	printf("  -o  指定输出的可执行文件名\n");
	printf("  -S  只进行编译\n");
	printf("  -C  只进行编译和汇编\n");
	printf("  -T  保留中间文件\n");
	printf("  -O1 启用O1等级优化\n");
	printf("  -O2 启用O2等级优化\n");
	printf("  -x  输出调试控制指令信息\n");
}

void printMoreOptions()
{
	printf("编译器指令:\n");
	printf("  --printValTab       输出变量表\n");
	printf("  --printFunTab       输出函数表\n");
	printf("  --printStrTab       输出字符串表\n");
	printf("  --printTokenStream  输出Token流\n");
	printf("  --printDFG          输出数据流图\n");
	printf("\n");
	printf("汇编器指令:\n");
	printf("  --printSymbolTable  输出符号表\n");
	printf("\n");
	printf("链接器指令:\n");
	printf("  --printLinkInfo     输出链接信息\n");

}


bool analyseModuleOptions(string option,Args& args)
{
	// 各模块可支持的参数
	const static set<string> cmpOptionArgs = {
		"--printValTab","--printFunTab","--printStrTab",
		"--printTokenStream", "--printDFG", "-O1", "-O2"};
	const static set<string> assOptionArgs = {"--printSymbolTable"};
	const static set<string> litOptionArgs = {"--printLinkInfo"};

	if(cmpOptionArgs.find(option) != cmpOptionArgs.end()){
		args.cmp.push_back(option);
		return true;
	}
	else if(assOptionArgs.find(option) != assOptionArgs.end()){
		args.ass.push_back(option);
		return true;
	}
	else if(litOptionArgs.find(option) != litOptionArgs.end()){
		args.lit.push_back(option);
		return true;
	}

	return false;
}

Args analyseOptions(int argc, char* argv[])
{
	Args args;
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
			else if(option[1] == 'h'){
				args.isHelped = true;
				printHelpInfo();
			}
			else if(option[1] == 'v'){
				args.isVersion = true;
				printVersionInfo();
			}
			else if(option[1] == 'x'){
				args.isHelped = true;
				printMoreOptions();
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
				bool isModuleOption = analyseModuleOptions(option,args);
				if(!isModuleOption){
					printf("选项: %s 无效\n",argv[i]);
				}				
			}
		}
	}
	return args;
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


void toSFile(const vector<CompileFile>& compilefiles,const vector<string>& options)
{

	string optionStr = " ";
	for(const auto& option: options){
		optionStr.append(option).append(" ");
	}

	for(const auto& file: compilefiles){
		if(file.getType() == ".c" || file.getType() == ".i"){
			execCmd("","lscc", file.getFullName()+optionStr);
		}
	}
}

void toOFile(const vector<CompileFile>& compilefiles,const vector<string>& options)
{
	string optionStr = " ";
	for(const auto& option: options){
		optionStr.append(option).append(" ");
	}

	for(const auto& file: compilefiles){
		if(file.getType() == ".c"|| file.getType() == ".i" || file.getType() == ".s"){
			execCmd("","lsca", file.getCoreName()+".s "+ optionStr);
		}
	}
}

void toExeFile(const vector<CompileFile>& compilefiles,const vector<string>& options)
{
	const static string libs[] = {
		"/usr/include/lsc/lscore.o",
		"/usr/include/lsc/lscio.o",
		};

	string allfiles = "";
	for(const auto& lib: libs){
		allfiles.append(lib).append(" ");
	}

	
	for(const auto& file: compilefiles){
		allfiles.append(file.getCoreName()).append(".o");
		allfiles.append(" ");
	}

	for(const auto& option: options){
		allfiles.append(option).append(" ");
	}
	
	execCmd("","lscl", allfiles);
	execCmd("","chmod +x z.out","");
}

void clearTempFile(const vector<CompileFile>& compilefiles, const Args& args)
{
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

	vector<CompileFile> compilefiles;

	Args args = analyseOptions(argc,argv);
	analyseFile(compilefiles,argc,argv);


	if(compilefiles.size()==0){
		if(!args.isHelped && !args.isVersion){
			printf("输入文件不能为空, 输入 lsc -h 查看帮助\n");
		}

		return 0;
	}
	
	if(args.isOnlyCompile) {
	   	toSFile(compilefiles,args.cmp);
	}
	else if(args.isOnlyAss) {
		toSFile(compilefiles,args.cmp);
		toOFile(compilefiles,args.ass);
	}
	else{
		toSFile(compilefiles,args.cmp);
		toOFile(compilefiles,args.ass);
		toExeFile(compilefiles,args.lit);
	}

	if(args.isSetOutput && !args.isOnlyCompile && !args.isOnlyAss){
		execCmd("","mv z.out",args.outputName);
	}

	if(!args.isSaveTempFile) {
		clearTempFile(compilefiles, args);
	}
}

