#include "Lexer.h"
#include <string>
using namespace std;

#define LEXERROR(code) Error::lexError(code)

KeyWords::KeyWords()
{
    keywords["include"] = KW_INCLUDE;
    keywords["define"] = KW_DEFINE;
}

Symbol KeyWords::getSym(std::string name)
{
    return keywords.find(name) != keywords.end() ? keywords[name] : IDENT;
}


KeyWords Lexer::key = KeyWords();

Lexer::Lexer(Scanner& sc) : scanner(sc)
{
    token = nullptr;
    ch = ' ';
}

bool Lexer::scan(char need)
{
    ch = scanner.scan();
    if(need){
        if(ch != need){
            return false;
        }
        ch = scanner.scan();
        return true;
    }
    return true;
}

Token* Lexer::nextToken()
{
    while(ch != -1){
        Token * t = nullptr;

        // 掠过空白字符
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
            t = new Token(POUND);
            scan();
        }
        else if(ch == '<'){
            t = new Token(LABRACK);
            scan();
        }
        else if(ch == '>'){
            t = new Token(RABRACK);
            scan();
        }
        else{
            // 无关符号,直接跳过
            scan(); 
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
    
    //匹配结束
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
                t=new Token(ERR);
                break;
            }
            else{
                str.push_back(ch);
            } 
        }
        //文件结束
        else if(ch=='\n'||ch==-1){
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
    if(ch!='0'){//10进制
        do{
            scan();
        }while(ch>='0'&&ch<='9');
    }
    else{
        scan();
        if(ch=='x'){//16进制
            scan();
            if((ch>='0'&&ch<='9')||(ch>='A'&&ch<='F')||(ch>='a'&&ch<='f')){
                do{						
                    scan();
                }while((ch>='0'&&ch<='9')||(ch>='A'&&ch<='F')||(ch>='a'&&ch<='f'));
            }
        }
        else if(ch=='b'){//2进制
            scan();
            if(ch>='0'&&ch<='1'){
                do{
                    scan();
                }while(ch>='0'&&ch<='1');
            }
        }
        else if(ch>='0'&&ch<='7'){//8进制
            do{
                scan();
            }while(ch>='0'&&ch<='7');
        }
    }

    return new Token(ERR);
}

Token* Lexer::getChar()
{
    scan();
    //转义
    if(ch=='\\'){
        scan();

    }
    scan();

    return new Token(ERR);
}

Lexer::~Lexer()
{
    delete token;
}