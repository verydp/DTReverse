/***************32位***************
DOS头：
typedef struct _IMAGE_DOS_HEADER {
****WORD e_magic;
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
****LONG e_lfanew;
} IMAGE_DOS_HEADER,*PIMAGE_DOS_HEADER;


标准PE头 + 扩展PE头 = NT头
typedef struct _IMAGE_NT_HEADERS{
	****DWORD Signature;							//PE头标记，通常值为4550
		IMAGE_FILE_HEADER FileHeader;				//标准PE头 20字节。
		IMAGE_OPTIONAL_HEADER32 OptionalHeader32;	//扩展PE头 大小是可变的。
}IMAGE_NT_HEADERS32,*PIMAGE_NT_HEADERS32;



标准PE头（20字节）
typedef struct _IMAGE_FILE_HEADER {
	****WORD  Machine;								//2字节，运行在何种CPU上；0就是任意；014C是interl386及以后；8664则是x64也就是64位系统上；
	****WORD  NumberOfSections;						//2字节，保存节的数量。.data、.text
		DWORD TimeDateStamp;						//4字节，由编译器填写的时间戳，和文件的创建、修改时间无关。
		DWORD PointerToSymbolTable;					//4字节，调试相关
		DWORD NumberOfSymbols;						//4字节，调试相关
	****WORD  SizeOfOptionalHeader;					//2字节 可保存扩展PE头的大小，32位默认是0xE0(224字节)，64位默认是0xF0(240字节)，大小可以自定义
		WORD  Characteristics;						//2字节，这部分可根据其二进制位上是否置1，来判断该PE文件的相关属性。
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
	****WORD  Magic;								//2字节，能准确识别该文件的位数，要是该成员位置上为0x10B则是32位程序，0x20B则是64位程序。
		BYTE  MajorLinkerVersion;					//1字节，链接器主版本号。
		BYTE  MinorLinkerVersion;					//1字节，链接器次版本号。
		DWORD SizeOfCode;							//4字节，所有代码节的总和，文件对齐后的大小。
		DWORD SizeOfInitializedData;				//4字节，包含所有已经初始化数据的节的总和，文件对齐后的大小。
		DWORD SizeOfUninitializedData;				//4字节，包含书友未初始化数据的节的总和，文件对齐后的大小。
	****DWORD AddressOfEntryPoint;					//4字节，程序的入口。
	****DWORD BaseOfCode;							//4字节，代码开始的基址。
	****DWORD BaseOfData;							//4字节，数据开始的基址。可改
	****DWORD ImageBase;							//4字节，内存镜像基址，每个程序运行时都有一个4GB的虚拟内存，而这个基址就是该PE文件在这4GB里面的起始地址。
		DWORD SectionAlignment;						//4字节，内存对齐，PE文件在内存上运行时，每个节数据的大小就是其整数倍的大小；PE的DOS头+PE头+节表的大小也是其整数倍
		DWORD FileAlignment;						//4字节，文件对齐，PE文件在磁盘上时，每个节数据的大小就是其整数倍的大小；PE的DOS头+PE头+节表的大小也是其整数倍。
		WORD  MajorOperatingSystemVersion;			//2字节，操作系统主版本号
		WORD  MinorOperatingSystemVersion;			//2字节，操作系统次版本号
		WORD  MajorImageVersion;					//2字节，PE文件主版本号
		WORD  MinorImageVersion;					//2字节，PE文件次版本号
		WORD  MajorSubsystemVersion;				//2字节，运行时所需系统主版本号
		WORD  MinorSubsystemVersion;				//2字节，运行时所需系统次版本号
		DWORD Win32VersionValue;					//4字节，子系统版本的值必须是0
	****DWORD SizeOfImage;							//4字节，内存中整个PE文件的大小，可以比硬盘上的文件大，但必须是SectionAlignment值的整数倍，决定整个PE在内存中大小
	****DWORD SizeOfHeaders;						//4字节，所有头+节表按照文件对齐后的大小，否则加载会出错。
		DWORD CheckSum;								//4字节，校验和，一些系统文件有要求，判断文件是否被修改。
		WORD  Subsystem;							//4字节，子系统。如果该成员值为1，则是驱动程序；2就是图形界面；3就是控制台程序、DLL。
		WORD  DllCharacteristics;					//2字节，文件特性。可通过类似标准PE头的文件属性查表来看。
	****DWORD SizeOfStackReserve;					//4字节，初始化时保留栈的大小。
	****DWORD SizeOfStackCommit;					//4字节，初始化时实际提交的栈大小。
	****DWORD SizeOfHeapReserve;					//4字节，初始化时保留的堆大小。
	****DOWRD SizeOfHeapCommit;						//4字节，初始化时实际提交的堆大小。
		DWORD LoaderFlags;							//4字节，调试相关。
		DWORD NumberOfRvaAndSizes;					//4字节，目录项目数。可知道该文件使用了多少个表。
		IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];//有多少张表，该数据的大小就是多少。
}IMAGE_OPTIONAL_HEADER32,*PIMAGE_OPTINAL_HEADER32;

*--------------DllCharacteristics---------------*
* 0		保留，必须为零							*
* 1		保留，必须为零							*
* 2		保留，必须为零							*
* 3		保留，必须为零							*
* 4		DLL可在加载时被重定位					*
* 5		强制代码实施完整性						*
* 6		该映像兼容DEP							*
* 7		可以隔离，但并不隔离此映像。			*
* 8		映像不使用SEH							*
* 9		不绑定映像								*
* 10	保留，必须为零							*
* 11	该映像为一个WDM driver					*
* 12											*
* 13											*
* 14	保留，必须为零							*
* 15	可用于终端服务器						*
*--------------DllCharacteristics---------------*

节表
在PE文件中节表是一个结构数组，也就是有多个节表构成，这部分数据按照文件对齐的大小存储。可以通过FILEAlignment(line 78)查看。
且该结构体的成员大多是标识节数据的相关属性，包括在文件和在内存中的起始地址、在文件中尺寸。
*/