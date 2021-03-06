#pragma once

//所有的词法记号
typedef enum {
  ERR,                              // 错误
  END,                              // 结束
  IDENT,NUM,STR,                    // 标识符,常量
  KW_SEC,KW_GLB,KW_EQU,KW_TIMES,    // 关键字
  KW_DB,KW_DW,KW_DD,                
  BR_AL,BR_CL,BR_DL,BR_BL,          // 寄存器名
  BR_AH,BR_CH,BR_DH,BR_BH,
  DR_EAX,DR_ECX,DR_EDX,DR_EBX,
  DR_ESP,DR_EBP,DR_ESI,DR_EDI,
  I_MOV,I_CMP,                      // 双操作数指令
  I_SUB,I_ADD,
  I_AND,I_OR,
  I_LEA,
  I_CALL,I_INT,                     // 单操作数指令
  I_IMUL,I_IDIV,
  I_NEG,I_INC,I_DEC,
  I_JMP,I_JE,I_JNE,
  I_SETE,I_SETNE,I_SETG,I_SETGE,I_SETL,I_SETLE,
  I_PUSH,I_POP,
  I_RET,                            // 无操作数指令
  ADD,SUB,COMMA,LBRAC,RBRAC,COLON,        // 界符
} Symbol;

// 操作数类型
typedef enum {
  NONE,             // 无
  IMMEDIATE,        // 立即数
  REGISTER,         // 寄存器
  MEMORY,           // 内存
} OpType;           