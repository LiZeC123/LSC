#include "Lexer.h"
#include <string>
using namespace std;

KeyWords::KeyWords()
{
    keywords["section"] = KW_SEC;
    keywords["global"]  = KW_GLB;
    keywords["times"]   = KW_TIMES;
    keywords["equ"]= KW_EQU;
    keywords["db"] = KW_DB;
    keywords["dw"] = KW_DW;
    keywords["dd"] = KW_DD;
    keywords["al"] = BR_AL;
    keywords["cl"] = BR_CL;
    keywords["dl"] = BR_DL;
    keywords["bl"] = BR_BL;
    keywords["ah"] = BR_AH;
    keywords["ch"] = BR_CH;
    keywords["dh"] = BR_DL;
    keywords["bh"] = BR_BH;
    keywords["eax"] = DR_EAX;
    keywords["ecx"] = DR_ECX;
    keywords["edx"] = DR_EDX;
    keywords["ebx"] = DR_EBX;
    keywords["esp"] = DR_ESP;
    keywords["ebp"] = DR_EBP;
    keywords["esi"] = DR_ESI;
    keywords["edi"] = DR_EDI;
    keywords["mov"] = I_MOV;
    keywords["cmp"] = I_CMP;
    keywords["sub"] = I_SUB;
    keywords["add"] = I_ADD;
    keywords["and"] = I_AND;
    keywords["or"]  = I_OR;
    keywords["lea"] = I_LEA; 
    keywords["call"]= I_CALL;
    keywords["int"] = I_INT;
    keywords["imul"]= I_IMUL; 
    keywords["idiv"]= I_IDIV;
    keywords["neg"] = I_NEG;
    keywords["inc"] = I_INC;
    keywords["dec"] = I_DEC;
    keywords["jmp"] = I_JMP;
    keywords["je"]  = I_JE; 
    keywords["jne"] = I_JNE;
    keywords["sete"] = I_SETE;
    keywords["setne"]= I_SETNE;
    keywords["setg"] = I_SETG;
    keywords["setge"]= I_SETGE;
    keywords["setl"] = I_SETL;
    keywords["setle"]= I_SETLE;
    keywords["push"] = I_PUSH;
    keywords["pop"]  = I_POP;
    keywords["ret"]  = I_RET;
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

        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') 
            || (ch == '_') || (ch == '@') || (ch == '.')){
            t = getIdent();
        }
        else if ((ch >= '0') && (ch <= '9')){
            t = getNum();
        }
        else if (ch == '"'){
            t = getStr();
        }
        else{
            switch(ch)//界符
			{
            case '+':
                t = new Token(ADD);scan();break;
            case '-':
                t = new Token(SUB);scan();break;
            case ';':
                scan();
                while(ch!='\n' && ch!= -1)
                    scan(); // 读取此行所有字符
                t = new Token(ERR);scan();break;
            case ',':
                t = new Token(COMMA);scan();break;
            case ':':
                t = new Token(COLON);scan();break;
            case '[':
                t = new Token(LBRAC);scan();break;
            case ']':
                t = new Token(RBRAC);scan();break;
            default:
                t = new Token(ERR); 
                scan();
			}
            
        }
        if(token){
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
    }while((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_'||(ch>='0'&&ch<='9')||ch == '@'||ch == '.');
    
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
    string str="";
    while(!scan('"')){
        str.push_back(ch);     
    }
    //最终字符串
    return new Str(str);
}

Token* Lexer::getNum()
{
    int val=0;
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
                //0x后无数据,实际不可能存在
                return new Token(ERR);
            }
        }
    }
    return new Num(val);
}

void Lexer::reset()
{
    // 将状态恢复
    ch = ' '; 
    scanner.reset();
}

Lexer::~Lexer()
{
    delete token;
}