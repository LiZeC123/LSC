ELF文件读取过程
----------------

1. 读取文件头
    - 文件头是固定大小的,从中可以获得程序头在文件中的偏移(e_phoff),段表在程序中的偏移(e_shoff),段表字符串所在段在段表的索引(e_shstrndx)
2. 读取段表字符串对应段表项
    - e_shoff +  e_shentsize * e_shstrndx 
    - 根据段表项中记录的段表偏移(sh_offset),获得段表字符串表段的位置
3. 读取段表
    - 根据段表在程序中的偏移(e_shoff)和段表个数(e_shnum)可以获得段表范围在地址为e_shoff 到 e_shoff + e_ehsize * e_shentsize的区间内
    - 在每一项中,根据段名字符串在段表字符串表中的偏移(sh_name),获得段名字符串地址
4. 读取符号表
    - 根据段表定位到符号表位置
    - 根据符号表使用的串表对应的段表项在段表中的索引位置(sh_link),得到符号表使用的串表在文件中的位置
    - 依次读取符号表
5. 读取重定位表