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
        else{
            switch(ch)//界符
			{
            case '#'://忽略行（忽略宏定义）
                while(ch!='\n' && ch!= -1)
                    scan();//行 文件不结束
                t=new Token(ERR);
                break;
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

    if(token){
        delete token;
    }
    return new Token(END);
    // 内存回收处理


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

Lexer::~Lexer()
{
    delete token;
}