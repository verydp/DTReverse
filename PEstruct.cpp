/***************32位***************
DOS头：
typedef struct _IMAGE_DOS_HEADER {
	WORD e_magic;
	WORD e_cblp;
	WORD e_cp;
	WORD e_crlc;
	WORD e_cparhdr;
	WORD e_minalloc;
	WORD e_maxalloc;
	WORD e_ss;
	WORD e_sp;
	WORD e_csum;
	WORD e_ip;
	WORD e_cs;
	WORD e_lfarlc;
	WORD e_ovno;
	WORD e_res[4];
	WORD e_oemid;
	WORD e_oeminfo;
	WORD e_res2[10];
	LONG e_lfanew;
} IMAGE_DOS_HEADER,*PIMAGE_DOS_HEADER;


标准PE头 + 扩展PE头 = NT头
typedef struct _IMAGE_NT_HEADERS{
	DWORD Signature;								//PE头标记，通常值为4550
	IMAGE_FILE_HEADER FileHeader;					//标准PE头 20字节。
	IMAGE_OPTIONAL_HEADER32 OptionalHeader32;		//扩展PE头 大小是可变的。
}IMAGE_NT_HEADERS32,*PIMAGE_NT_HEADERS32;



标准PE头
typedef struct _IMAGE_FILE_HEADER {
	WORD  Machine;									//2字节，运行在何种CPU上；0就是任意；014C是interl386及以后；8664则是x64也就是64位系统上；
	WORD  NumberOfSections;							//2字节，保存节的数量。.data、.text
	DWORD TimeDateStamp;							//4字节，由编译器填写的时间戳，和文件的创建、修改时间无关。
	DWORD PointerToSymbolTable;						//4字节，调试相关
	DWORD NumberOfSymbols;							//4字节，调试相关
	WORD  SizeOfOptionalHeader;						//2字节 可保存扩展PE头的大小，32位是0xE0(224字节)，64位是0xF0(240字节)
	WORD  Characteristics;							//2字节，这部分可根据其二进制位上是否置1，来判断该PE文件的相关属性。
}IMAGE_FILE_HEADER,*PIMAGE_FILE_HEADER;

*--------------Characteristics------------------*
* 0		文件不存在重定位信息					*
* 1		文件是可执行的							*
* 2		不存在行信息							*
* 3		不存在符号信息							*
* 4		调整工作集								*
* 5		应用程序可调用2GB的地址（64位）			*
* 6		此标志保留								*
* 7		小尾方式								*
* 8		只在32位平台上运行						*
* 9		不包含调试信息							*
* 10	不能从可移动盘运行						*
* 11	不能从网络运行							*
* 12	系统文件（如驱动程序），不能直接运行。	*
* 13	这是一个DLL文件。						*
* 14	文件不能在多处理器计算机上运行			*
* 15	大尾方式								*
*--------------Characteristics------------------*
//https://blog.csdn.net/qq_51521220/article/details/122045878
扩展PE头
typedef struct _IMAGE_OPTIONAL_HEADER  {
	WORD  Magic;									//2字节，能准确识别该文件的位数，要是该成员位置上为0x10B则是32位程序，0x20B则是64位程序。
	BYTE  MajorLinkerVersion;						//1字节，链接器版本号。
	BYTE  MinorLinkerVersion;						//1字节，链接器版本号。
	DWORD SizeOfCode;								//4字节，所有代码节的总和，文件对齐后的大小。
	DWORD SizeOfInitializedData;					//4字节，包含所有已经初始化数据的节的总和，文件对齐后的大小。
	DWORD SizeOfUninitializedData;					//4字节，包含书友未初始化数据的节的总和，文件对齐后的大小。
	DWORD AddressOfEntryPoint;						//4字节，程序的入口。
	DWORD BaseOfCode;								//4字节，代码开始的基址。
	DWORD BaseOfData;								//4字节，数据开始的基址。
	DWORD ImageBase;								//4字节，内存镜像基址，每个程序运行时都有一个4GB的虚拟内存，而这个基址就是该PE文件在这4GB里面的起始地址。
	DWORD SectionAlignment;							//4字节，内存对齐，PE文件在内存上运行时，每个节数据的大小就是其整数倍的大小；PE的DOS头+PE头+节表的大小也是其整数倍
	DWORD FileAlignment;							//4字节，文件对齐，PE文件在磁盘上时，每个节数据的大小就是其整数倍的大小；PE的DOS头+PE头+节表的大小也是其整数倍。
	WORD  MajorOperatingSystemVersion;				//2字节，操作系统主版本号
	WORD  MinorOperatingSystemVersion;				//2字节，操作系统次版本号
	WORD  MajorImageVersion;						//2字节，PE文件主版本号
	WORD  MinorImageVersion;						//2字节，PE文件次版本号
	WORD  MajorSubsystemVersion;					//2字节，运行时所需系统主版本号
	WORD  MinorSubsystemVersion;					//2字节，运行时所需系统次版本号
	DWORD Win32VersionValue;						//4字节，子系统版本的值必须是0
	DWORD SizeOfImage;								//4字节，内存中整个PE文件的大小，可以比硬盘上的文件大，但必须是SectionAlignment值的整数倍，决定整个PE在内存中大小
	DWORD SizeOfHeaders;							//
	DWORD CheckSum;
	WORD  Subsystem;
	WORD  DllCharacteristics;
	DWORD SizeOfStackReserve;
	DWORD SizeOfStackCommit;
	DWORD SizeOfHeapReserve;
	DOWRD SizeOfHeapCommit;
	DWORD LoaderFlags;
	DWORD NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
}IMAGE_OPTIONAL_HEADER32,*PIMAGE_OPTINAL_HEADER32;

*/