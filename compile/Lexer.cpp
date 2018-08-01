#include "Lexer.h"
#include "Error.h"
#include <string>
using namespace std;

#define LEXERROR(code) Error::lexError(code)

KeyWords::KeyWords()
{
    keywords["int"] = KW_INT;
    keywords["char"] = KW_CHAR;
    keywords["void"] = KW_VOID;
    keywords["extern"] = KW_EXTERN;
    keywords["if"] = KW_IF;
    keywords["else"] = KW_ELSE;
    keywords["switch"] = KW_SWITCH;
    keywords["case"] = KW_CASE;
    keywords["default"] = KW_DEFAULT;
    keywords["while"] = KW_WHILE;
    keywords["do"] = KW_DO;
    keywords["for"] = KW_FOR;
    keywords["break"] = KW_BREAK;
    keywords["continue"] =KW_CONTINUE ;
    keywords["return"] = KW_RETURN;
    keywords["include"] = KW_INCLUDE;
    keywords["define"] = KW_DEFINE;
}

Symbol KeyWords::getSym(std::string name)
{
    return keywords.find(name) != keywords.end() ? keywords[name] : IDENT;
}


KeyWords Lexer::key = KeyWords();


Macros::Macros(Scanner& sc) : scan(sc)
{
    macros["__LINE__"] = nullptr;
    macros["__FILE__"] = nullptr;
}

bool Macros::isMacro(std::string name)
{
    return macros.find(name) != macros.end();
}

vector<Token*>* Macros::getRealToken(std::string name)
{
    if(name == "__LINE__"){
        return new vector<Token*>{ new Num(scan.getRow()) };

    }
    else if(name=="__FILE__"){
        return new vector<Token*>{ new Str(scan.getFilename()) };
    }
    return macros.find(name)->second;
}

void Macros::addMacro(std::string name,vector<Token*>* list)
{
    if(isMacro(name)){
        LEXERROR(MACRO_RE_DEF);
    }
    else{
        macros[name] = list;
    }
}


Lexer::Lexer(Scanner* sc) : scanner(sc), macros(*sc)
{
    token = nullptr;
    ch = ' ';
    macroTokenIndex = 0;
    macroTokenList = nullptr;
}

bool Lexer::scan(char need)
{
    ch = scanner->scan();
    if(need){
        if(ch != need){
            return false;
        }
        ch = scanner->scan();
        return true;
    }
    return true;
}

Token* Lexer::nextToken()
{
    if(macroTokenIndex != 0){
        if(macroTokenList->size() > macroTokenIndex){
            Token* r = macroTokenList->at(macroTokenIndex);
            macroTokenIndex++;
            return r;            
        }
        else{
            macroTokenIndex = 0;
        }
    }

    while(ch != -1){
        Token * t = nullptr;

        // 略过空白字符
        while (ch == ' ' || ch == 10 || ch == 9)
        {
            scan();
        }

        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_')){
            t = getIdent();
        }
        else if ((ch >= '0') && (ch <= '9')){
            t = getNum();
        }
        else if (ch == '"'){
            t = getStr();
        }
        else if (ch == '\''){
            t = getChar();
        }
        else if(ch == '#'){
            getMacro();
            t=new Token(ERR);
        }
        else{
            switch(ch)//界符
			{
            case '+':
                t=new Token(scan('+')?INC:ADD);break;
            case '-':
                t=new Token(scan('-')?DEC:SUB);break;
            case '*':
                t=new Token(MUL);scan();break;
            case '/':
                scan();
                if(ch=='/'){//单行注释
                    while(ch!='\n' && ch!= -1)
                        scan();//行 文件不结束
                    t=new Token(ERR);
                }
                else if(ch=='*'){//多行注释,不允许嵌套注释
                    while(!scan(-1)){//一直扫描
                        if(ch=='*'){
                            if(scan('/'))break;
                        }
                    }
                    if(ch==-1)//没正常结束注释
                        LEXERROR(COMMENT_NO_END);
                    t=new Token(ERR);
                }
                else
                    t=new Token(DIV);
                break;
            case '%':
                t=new Token(MOD);scan();break;
            case '>':
                t=new Token(scan('=')?GE:GT);break;
            case '<':
                t=new Token(scan('=')?LE:LT);break;
            case '=':
                t=new Token(scan('=')?EQU:ASSIGN);break;
            case '&':
                t=new Token(scan('&')?AND:LEA);break;
            case '|':
                t=new Token(scan('|')?OR:ERR);
                if(t->sym==ERR)
                    LEXERROR(OR_NO_PAIR);//||没有一对
                break;
            case '!':
                t=new Token(scan('=')?NEQU:NOT);break;
            case ',':
                t=new Token(COMMA);scan();break;
            case ':':
                t=new Token(COLON);scan();break;
            case ';':
                t=new Token(SEMICON);scan();break;
            case '(':
                t=new Token(LPAREN);scan();break;
            case ')':
                t=new Token(RPAREN);scan();break;
            case '[':
                t=new Token(LBRACK);scan();break;
            case ']':
                t=new Token(RBRACK);scan();break;
            case '{':
                t=new Token(LBRACE);scan();break;
            case '}':
                t=new Token(RBRACE);scan();break;
            case '.':
                t=new Token(POINT);scan();break;
            case -1:scan();break;
            default:
                t=new Token(ERR);//错误的词法记号
                LEXERROR(TOKEN_NO_EXIST);
                scan();
			}
            
        }
        if(token != nullptr){
            delete token;
        }
        token = t;
        
        if(token&&token->sym != ERR){
            return token;   // 正常符号,直接返回
        }
        else{
            continue;      // 错误符号,读取下一轮
        }

    }

    if(!scanStack.empty()){
        scanner = scanStack.back();
        scan();// 回复ch状态
        scanStack.pop_back();
        return nextToken();
    }

    if(token){
        delete token;
    }
    // 最后一个符号在析构函数中释放内存
    return token = new Token(END);
}

Token * Lexer::getIdent()
{
    string name="";
    do{
        name.push_back(ch);
        scan();
    }while((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_'||(ch>='0'&&ch<='9'));
    
    //匹配结束,首先检查是否为宏名
    if(macros.isMacro(name)){
        macroTokenList = macros.getRealToken(name);
        macroTokenIndex = 1;
        return macroTokenList->at(0);
    }
    
    // 不是宏再检查是关键字还是标识符
    Symbol tag=key.getSym(name);
    if(tag==IDENT){
        //正常的标志符
        return new ID(name);
    }
    else{
        //关键字
        return new Token(tag);
    }
}

Token* Lexer::getStr()
{
    Token* t = nullptr;
    string str="";
    while(!scan('"')){
        //转义
        if(ch=='\\'){
            scan();
            if(ch=='n')str.push_back('\n');
            else if(ch=='\\')str.push_back('\\');
            else if(ch=='t')str.push_back('\t');
            else if(ch=='"')str.push_back('"');
            else if(ch=='0')str.push_back('\0');
            else if(ch=='\n');//什么也不做，字符串换行
            else if(ch==-1){
                LEXERROR(STR_NO_R_QUTION);
                t=new Token(ERR);
                break;
            }
            else{
                str.push_back(ch);
            } 
        }
        //文件结束
        else if(ch=='\n'||ch==-1){
            LEXERROR(STR_NO_R_QUTION);
            t=new Token(ERR);
            break;
        }
        else{
            str.push_back(ch);
        }
            
    }
    //最终字符串
    if(!t)t=new Str(str);
    return t;
}

Token* Lexer::getNum()
{
    int val=0;
    Token* t = nullptr;
    if(ch!='0'){//10进制
        do{
            val=val*10+ch-'0';
            scan();
        }while(ch>='0'&&ch<='9');
    }
    else{
        scan();
        if(ch=='x'){//16进制
            scan();
            if((ch>='0'&&ch<='9')||(ch>='A'&&ch<='F')||(ch>='a'&&ch<='f')){
                do{
                    val=val*16+ch;
                    if(ch>='0'&&ch<='9')val-='0';
                    else if(ch>='A'&&ch<='F')val+=10-'A';
                    else if(ch>='a'&&ch<='f')val+=10-'a';							
                    scan();
                }while((ch>='0'&&ch<='9')||(ch>='A'&&ch<='F')||(ch>='a'&&ch<='f'));
            }
            else{
                LEXERROR(NUM_HEX_TYPE);//0x后无数据
                t=new Token(ERR);
            }
        }
        else if(ch=='b'){//2进制
            scan();
            if(ch>='0'&&ch<='1'){
                do{
                    val=val*2+ch-'0';
                    scan();
                }while(ch>='0'&&ch<='1');
            }
            else{
                LEXERROR(NUM_BIN_TYPE);//0b后无数据
                t=new Token(ERR);
            }
        }
        else if(ch>='0'&&ch<='7'){//8进制
            do{
                val=val*8+ch-'0';
                scan();
            }while(ch>='0'&&ch<='7');
        }
    }
    //最终数字
    if(!t)t=new Num(val);
    return t;
}

Token* Lexer::getChar()
{
    char c;
    Token * t = nullptr;
    scan();
    //转义
    if(ch=='\\'){
        scan();
        if(ch=='n')c='\n';
        else if(ch=='\\')c='\\';
        else if(ch=='t')c='\t';
        else if(ch=='0')c='\0';
        else if(ch=='\'')c='\'';
        //文件结束 换行
        else if(ch==-1||ch=='\n'){
            LEXERROR(CHAR_NO_R_QUTION);
            t=new Token(ERR);
        }
        else c=ch;//没有转义
    }
    else if(ch=='\n'||ch==-1){//行 文件结束
        LEXERROR(CHAR_NO_R_QUTION);
        t=new Token(ERR);
    }
    else if(ch=='\''){//没有数据
        LEXERROR(CHAR_NO_DATA);
        t=new Token(ERR);
        scan();//读掉引号
    }
    else c=ch;//正常字符
    if(!t){
        if(scan('\'')){//匹配右侧引号,读掉引号
            t=new Char(c);
        }
        else{
            LEXERROR(CHAR_NO_R_QUTION);
            t=new Token(ERR);
        }
    }


    return t;
}


void Lexer::getMacro()
{
    scan();
    Token* key = nextToken();
    if(key->sym == KW_DEFINE){
        // define
        // 另外拷贝一份,nextToken返回的符号下次调用前被析构
        Token* name = nextToken()->copy();
        vector<Token*>* list = new vector<Token*>();
        // 通过检查ch值来判断是否换行
        while(ch!='\n' && ch!=-1){
            Token* value = nextToken()->copy();
            list->push_back(value);
        }
        
        macros.addMacro(((ID*)name)->name,list);
    }
    else if(key->sym == KW_INCLUDE){
        // include
        Scanner* newScan = loadIncludeFile();

        if(newScan != nullptr){
            scanStack.push_back(scanner);
            scanner = newScan;
        }
    }
}

Scanner* Lexer::loadIncludeFile()
{
    Token* sp = nextToken();
    if(sp->sym == LT){
        // 系统头文件 
        vector<Token*> fileNameToken;
        sp = nextToken()->copy();
        while(sp->sym != GT){
            fileNameToken.push_back(sp);
            sp = nextToken()->copy();
        }

        return includeStdFile(fileNameToken);
    }
    else if(sp->sym == STR){
        // 用户自定义头文件
        string userfile = ((Str*)sp)->str;
        return includeUserFile(userfile);
    }
    else{
        LEXERROR(INCLUDE_ERR);
        return nullptr;
    }
}

Scanner* Lexer::includeStdFile(vector<Token*> words){
    static const string stdPath = "/usr/include/lsc/";
    string name = stdPath;
    
    for(auto token:words){
        if(token->sym == IDENT){
            name.append(((ID*)token)->name);
        }
        else{
            name.append(token->toString());
        }
    }

    return new Scanner(name.c_str());
}

Scanner* Lexer::includeUserFile(string name)
{

    string filename = scanner->getFilename();
    size_t n = filename.rfind("/");
    size_t len = filename.size();
    if(n > 0){
        filename.replace(n+1,len-n-1,name);
    }

    return new Scanner(filename.c_str());
}


Lexer::~Lexer()
{
    delete token;
}