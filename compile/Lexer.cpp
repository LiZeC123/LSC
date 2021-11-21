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
    keywords["struct"] = KW_STRUCT;
    keywords["sizeof"] = KW_SIZEOF;
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
    last = nullptr;
}

bool Macros::isMacro(std::string name)
{
    return macros.find(name) != macros.end();
}

vector<Token*>* Macros::getRealToken(std::string name)
{
    // 如果上一轮有分配内存, 本轮释放
    deleteLast();

    if(name == "__LINE__"){
        last = new vector<Token*>{ new Num(scan.getRow()) };
        return last;

    }
    else if(name=="__FILE__"){
        last = new vector<Token*>{ new Str(scan.getFilename()) };
        return last;
    }
    else{
        return macros.find(name)->second;
    }
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

Macros::~Macros()
{
    for(auto it = macros.begin(); it != macros.end(); ++it){
        auto vec = it->second;

        // __LINE__ 等宏对应的内容为nullptr, 不需要释放
        if(vec != nullptr){
            for(auto v:(*vec)){
                delete v;
            }
            delete vec;
        }
    }

    if(last != nullptr){
        delete last;
    }
}


void Macros::deleteLast()
{
    if(last != nullptr){
        for(auto token:(*last)){
            delete token;
        }
    }
    delete last;
    last = nullptr;
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

Token* Lexer::readToken()
{
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
                    bool endflag = false;
                    while(!scan(-1)){//一直扫描
                        if(ch=='*'){
                            if(scan('/')){
                                endflag = true;
                                break;
                            }
                        }
                    }
                    if(!endflag)//没正常结束注释
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

        if(t == nullptr || t->sym == ERR){
            delete t;
        }
        else {
            return t;
        }       
    }

    return new Token(END);
}

Token* Lexer::nextMacroToken()
{
    if(macroTokenList != nullptr){
        if(macroTokenList->size() > macroTokenIndex){
            Token* r = macroTokenList->at(macroTokenIndex);
            macroTokenIndex++;
            return r;            
        }
        else{
            macroTokenIndex = 0;
            macroTokenList = nullptr;
        }
    }
    return nullptr;
}

Token* Lexer::nextToken()
{
    // 如果是宏, 返回相应的Token
    Token* m = nextMacroToken();
    if(m != nullptr){
        return m;
    }

    // 释放上一轮的符号
    // token仅仅被readToken()赋值, 其他模块产生的Token由各自模块处理
    if(token != nullptr){
        delete token;
    }
    token = readToken();

    // 如果是宏的第一个符号, 返回相应的Token
    if(token->sym == MACRO){
        return nextMacroToken();
    }
    
    // 如果是文件结束,则先判断是否是处理头文件
    if(token->sym == END && !scanStack.empty()){
        // 释放当前Scanner,并且切换到栈顶的Scanner
        delete scanner;
        scanner = scanStack.back();
        scanStack.pop_back();

        // 恢复后当前符号是 > , 读取该符号后scanner状态恢复正常
        scan();
        
        // 更换scanner以后, 重新读取符号
        return nextToken();
    }
    
    return token;
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
        macroTokenIndex = 0;
        return new Token(MACRO);
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
    Token* key = readToken();
    if(key->sym == KW_DEFINE){
        // define
        Token* name = readToken();
        vector<Token*>* list = new vector<Token*>();
        // 通过检查ch值来判断是否换行
        while(ch!='\n' && ch!=-1){
            Token* value = readToken();
            list->push_back(value);
        }
        
        macros.addMacro(((ID*)name)->name,list);
        delete name;
    }
    else if(key->sym == KW_INCLUDE){
        // include
        Scanner* newScan = loadIncludeFile();

        if(newScan != nullptr){
            scanStack.push_back(scanner);
            scanner = newScan;
        }
    }

    delete key;
}

Scanner* Lexer::loadIncludeFile()
{
    Token* sp = readToken();
    if(sp->sym == LT){
        // 系统头文件 
        vector<Token*> fileNameToken;
        Token* name = readToken();
        while(name->sym != GT){
            fileNameToken.push_back(name);
            name = readToken();
        }

        delete sp;
        delete name;
        return includeStdFile(fileNameToken);
    }
    else if(sp->sym == STR){
        // 用户自定义头文件
        string userfile = ((Str*)sp)->str;

        delete sp;
        return includeUserFile(userfile);
    }
    else{
        LEXERROR(INCLUDE_ERR);

        delete sp;
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

        delete token;
    }

    return new Scanner(name);
}

Scanner* Lexer::includeUserFile(string name)
{

    string filename = scanner->getFilename();
    size_t n = filename.rfind("/");
    size_t len = filename.size();
    if(n > 0){
        filename.replace(n+1,len-n-1,name);
    }

    return new Scanner(filename);
}


Lexer::~Lexer()
{
    delete token;
}