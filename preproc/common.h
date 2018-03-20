#pragma once

//所有的词法记号
typedef enum {
    ERR,                            //错误，异常
    END,                            //文件结束标记
    IDENT,                          //标识符
    POUND,                          // #
    KW_INCLUDE,                     //include
    KW_DEFINE,                      //define
} Symbol;

// 更新词法记号列表的同时需要更新Token.cpp中的字符串表