ELF文件结构
==============

ELF格式(Executable and Linkable Format)是Linux平台主流的可执行文件格式,除了可执行文件(Executable File)以外, 可重定位目标文件(Relocatable Object File), 共享目标文件(Shared Object File), 核心转储文件(Core Dump File)也都是ELF格式

ELF文件一般结构
-----------------------


|                             |
|-----------------------------|
|文件头(ELF Header)            |
|程序头表(Program Header Table)|
|代码段(.text)                 |
|数据段(.data)                 |
|bss段(.bss)                  |
|段表字符串(.shstrtab)          |
|段表(Section Header Tbale)    |
|符号表(.symbol)               |
|字符串表(.strtab)             |
|重定位表(.rel.text)           |
|重定位表(.rel.data)           |
|                            |


在linux系统的`/usr/include/elf.h`中可以获得ELF文件的全部信息.


文件头(ELF Header)
-------------------
``` c
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;

typedef struct
{
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
  Elf32_Half	e_type;			/* Object file type */
  Elf32_Half	e_machine;		/* Architecture */
  Elf32_Word	e_version;		/* Object file version */
  Elf32_Addr	e_entry;		/* Entry point virtual address */
  Elf32_Off	e_phoff;		/* Program header table file offset */
  Elf32_Off	e_shoff;		/* Section header table file offset */
  Elf32_Word	e_flags;		/* Processor-specific flags */
  Elf32_Half	e_ehsize;		/* ELF header size in bytes */
  Elf32_Half	e_phentsize;		/* Program header table entry size */
  Elf32_Half	e_phnum;		/* Program header table entry count */
  Elf32_Half	e_shentsize;		/* Section header table entry size */
  Elf32_Half	e_shnum;		/* Section header table entry count */
  Elf32_Half	e_shstrndx;		/* Section header string table index */
} Elf32_Ehdr;
```
ELF文件头描述了文件格式,平台信息以及文件结构等信息,具体内容如下:
1. e_ident
    - 魔数,是16字节的数组,一般值为`71 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00`
    - 前4字节对应ASCII码为DEL,E,L,F,对每个ELF文件都是一样的
    - 第5字节表示文件类别,0表示无效,1表示32位文件,2表示64位文件
    - 第6字节表示字节序,0表示无效,1表示小端序,2表示大端序
    - 第7字节表示ELF版本,默认为1
    - 第8字节ELF标准未定义,通常取0表示Unix系统
    - 第9-16字节ELF标准未定义,通常取0
2. e_type
    - 表示ELF文件类型
    - 0表示无效,1表示重定位目标文件,2表示可执行文件,3表示共享目标文件,4表示核心转储文件
    - 对于汇编器,使用重定位目标文件,取值为ET_REL
    - 对于连接器,使用可执行文件,取值为ET_EXEC
3. e_machine
    - 表示机器类型
    - 3表示Intel 80386体系结构,取值为EM_386
4. e_version
    - 表示ELF文件版本,取值为EV_CURRENT
5. e_entry
    - ELF文件线性入口地址,重定位文件一般取值为0
6. e_phoff
    - 程序头在文件中的偏移,标识程序头起始位置
7. e_shoff
    - 表示段表在程序中的偏移,标识段表在文件的位置
8. e_flags
    - ELF平台相关信息,通常取0
9. e_ehsize
    -表示ELF文件头大小,即sizeof(Elf32_Ehdr) = 52 字节
10. e_phentsize
    - 表示程序头表大小,即sizeof(Elf32_Phdr) = 32 字节
11. e_phnum	
    - 表示程序头表项的个数
    - 从而确定程序头表在地址为e_phoff 到 e_phoff + e_phnum * e_phentsize的区间内
12. e_shentsize
    - 表示段表项大小,即 sizeof(Elf32_Shdr) = 40 字节
13.	e_shnum
    - 表示段表个数
    - 从而确定段表在地址为e_shoff 到 e_shoff + e_ehsize * e_shentsize的区间内
14.	e_shstrndx
    - 表示段表字符串所在段在段表的索引

注意: 使用readelf指令的-h参数可以查看ELF文件头信息


段表(Section Header Tbale) 
--------------------------
``` c
typedef struct
{
  Elf32_Word	sh_name;		/* Section name (string tbl index) */
  Elf32_Word	sh_type;		/* Section type */
  Elf32_Word	sh_flags;		/* Section flags */
  Elf32_Addr	sh_addr;		/* Section virtual addr at execution */
  Elf32_Off	sh_offset;		/* Section file offset */
  Elf32_Word	sh_size;		/* Section size in bytes */
  Elf32_Word	sh_link;		/* Link to another section */
  Elf32_Word	sh_info;		/* Additional section information */
  Elf32_Word	sh_addralign;		/* Section alignment */
  Elf32_Word	sh_entsize;		/* Entry size if section holds table */
} Elf32_Shdr;
```
ELF文件中,除了文件头和程序头表以外,其他信息都是以段的形式组织的,段表记录了每个段的详细信息,包括段的名称,位置,大小,属性等,具体信息如下:
1. sh_name
    - 一个4字节偏移量,记录了段名字符串在段表字符串表中的偏移
    - 段表字符串表是一个文件块,记录了所有的段表字符串内容,储存在.shstrtab段中
    - 段表字符串表在段表中的位置为 e_shoff +  e_shentsize * e_shstrndx 
2. sh_type
    - 表示段类型
    - 1表示程序段,取值SHT_PROGBITS,例如.text段和.data段
    - 2表示符号表段,取值为SHT_SYMTAB,例如.symtab段
    - 3表示串表段,取值为SHT_STRTAB,例如.shstrtab段和.strtab
    - 8表示无内容段,取值SHT_NOBITS,例如.bss段
    - 9表示重定位段,取值SHT_REL,例如.rel.text和.rel.data等
3. sh_flags
    - 段标记
    - 0表示默认属性
    - 1表示可写,取值为SHF_WRITE
    - 2表示需要分配空间,取值为SHF_ALLOC
    - 4表示可执行,取值为SHF_EXECINSTR
    - 属性可以复合
    - .text段可分配,可执行,不可写,取值为SHF_ALLOC | SHF_EXECINSTR
    - .data段可分陪,可写不可执行,取值为SHF_WRITE | SHF_ALLOC
4. sh_addr
    - 表示段加载后的线性地址,可重定位文件一般取值为0
5. sh_offset
    - 表示段在文件中的偏移
6. sh_size
    - 表示段的大小
    - 如果段是SHT_NOBITS,则此大小表示加载后的大小而不是文件实际大小
7. sh_link和sh_info
    - 对于符号表段(SHT_SYMTAB)
        - sh_link记录符号表使用的串表(".strtab")对应的段表项在段表中的索引
        - sh_info记录符号表内最后一个局部符号的符号表项的索引+1,这通常对应是第一个全局符号的索引
    - 对于重定位标段(SHT_REL)
        - sh_link记录重定位表使用的符号表段(".symtab")对应的段表项在段表中的索引
        - sh_info记录重定位表作用的段对应的段表项在段表中的索引(例如.rel.text对应.text)
    - 对于其他段,这两项默认取0
8. sh_addralign
    - 表示段对齐方式,取值必须是2的幂,即1,2,4,8等,且当取值为1时,表示没有对齐需求
    - 对于重定位文件
        - 代码段取值为4
        - 数据段取值为4
        - 其他段取值为1
    - 对于可执行文件
        - 代码段取值为16
        - 数据段取值为4
        - 其他段取值为1
9. sh_entsize
    - 一般保存诸如符号表,重定位表时,表项的大小
    - 对于符号表, 表项大小为sizeof(Elf32_Sym) = 32字节
    - 对于重定位表, 表项大小为sizeof(Elf32_Rel) = 8字节
    - 对于其他段,该字段默认为0

注意: 使用readelf指令的-S参数可以查看ELF文件段表信息



程序头表(Program Header Table)
-----------------------------
``` c
typedef struct
{
  Elf32_Word	p_type;			/* Segment type */
  Elf32_Off	p_offset;		/* Segment file offset */
  Elf32_Addr	p_vaddr;		/* Segment virtual address */
  Elf32_Addr	p_paddr;		/* Segment physical address */
  Elf32_Word	p_filesz;		/* Segment size in file */
  Elf32_Word	p_memsz;		/* Segment size in memory */
  Elf32_Word	p_flags;		/* Segment flags */
  Elf32_Word	p_align;		/* Segment alignment */
} Elf32_Phdr;
```

ELF文件中程序头表和段表是相互独立的,且一般只有可执行文件拥有程序头表. 

1. p_type
    - 段的类型,由于我们只关注可加载段,所以取值为PT_LOAD
2. p_offset
    - 段对应的内容在文件中的偏移
3. p_vaddr
    - 段在内存中的线性地址
4. p_paddr
    - 段的物理地址
    - 由于现代操作系统使用分页机制,因此此段与p_vaddr保持一致即可
5. p_filesz
    - 段在文件内的大小
6. p_memsz
    - 段在内存中的大小
7. p_flags
    - 与sh_flags类似,表示段属性
    - 1 表示可执行,取值PF_X
    - 2 表示可写,取值PF_W
    - 4 表示可读,取值PF_R
    - 段标记可以符合
8. p_align
    - 对齐方式
    - 一般取值为0x1000,即linux默认页面大小


符号表(.symbol)  
------------------
``` c
typedef struct
{
  Elf32_Word	st_name;		/* Symbol name (string tbl index) */
  Elf32_Addr	st_value;		/* Symbol value */
  Elf32_Word	st_size;		/* Symbol size */
  unsigned char	st_info;		/* Symbol type and binding */
  unsigned char	st_other;		/* Symbol visibility */
  Elf32_Section	st_shndx;		/* Section index */
} Elf32_Sym;
```

符号表保存了程序中的符号信息,包括程序中的文件名,函数名,全部变量名等. 具体细节如下
1. st_name
    - 一个4字节偏移量,记录了符号名字符串在字符串表的偏移
    - 字符串表是一个文件块,可从段表中获得偏移地址
2. st_value
    - 符号的值
    - 在可重定位文件中,记录了符号相对于段基址的偏移
    - 在可执行文件中,记录了符号的线性地址
3. st_size
    - 表示符号的大小,单位为字节
4. st_info
    - 表示符号类型相关信息
    - 低4位表示类型
        - 使用宏ELF32_ST_TYPE获得
        - 0 表示未知类型,取值STT_NOTYPE
        - 1 表示数据对象,取值STT_OBJECT
        - 2 表示函数,取值STT_FUNC
        - 3 表示段,取值STT_SECTION
        - 4 表示文件名,取值STT_FILE
    - 高4位表示绑定信息
        - 使用宏ELF32_ST_BIND获得
        - 0 表示局部符号,取值STB_LOCAL
        - 1 表示全局符号,取值STB_GLOBAL
        - 2 表示弱符号,取值STB_WEAK   
5. st_other
    - 此字段未定义,取值为0
6. st_shndx
    - 表示符号所在段对应段表项在段表内的索引,一般取整数
    - 0 表示未定义,取值SHN_UNDEF

注意: 使用readelf指令的-s参数可以查看ELF文件符号表信息


重定位表(.rel.)
---------------
``` c
typedef struct
{
  Elf32_Addr	r_offset;		/* Address */
  Elf32_Word	r_info;			/* Relocation type and symbol index */
} Elf32_Rel;
```

重定位表常见于可重定位文件,静态链接的可执行文件一般都不包含重定位表. 每个需要重定位的段都可以对应一个重定位表,例如.text段对应.rel,text段,.data段对应.rel.data段. 重定位表结构细节如下

1. r_offset
    - 重定位地址,表示重定位位置相对于被重定位段基址的偏移
2. r_info
    - 表示重定位类型和符号
    - 低8位表示重定位类型
        - 使用宏ELF32_R_TYPE获得
        - 绝对地址重定位取值R_386_32
        - 相对地址重定位取值R_386_PC32
    - 高24位表示重定位符号对应的符号表项在符号表中的索引

注意: 使用readelf指令的-r参数可以查看ELF文件符号表信息


串表(.shstrtab / .strtab)
-------------------------
ELF文件的段表和符号表需要保存段名和符号名,但段表和符号表都是长度固定的数据结构. 因此ELF文件将名称和字符串放在另外的表中, 仅仅记录相应的字符串在串表的偏移值.

注意: 使用hexdump指令的-C参数可以查看文件的二进制内容