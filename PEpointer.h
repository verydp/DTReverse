#ifndef PE_POINTER_H
#define PE_POINTER_H

//PEpointer函数

int* ptre_lfanew(char* fbuffer) {
	//先获取节表起始地址，即e_lfanew + 24 + SizeOfOptionalHeader
	//e_lfanew
	int* pe_lfanew;
	pe_lfanew = (int*)(fbuffer + 60);
	//int e_lfanew;
	//e_lfanew = *pe_lfanew;
	//printf("e_lfanew = %x\n", e_lfanew);

	return pe_lfanew;
}

char* ptrPE(char* fbuffer) {
	char* ret;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	//printf("%x\n", e_lfanew);
	ret = fbuffer + e_lfanew;

	return ret;
}

char* ptrOptionPE(char* fbuffer) {
	char* ret;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);
	///printf("%x\n", e_lfanew);
	ret = fbuffer + e_lfanew + 24;

	//printf("ptrOptionPE = %x\n", *(ret));

	return ret;

}

int* ptrAddressOfEntryPoint(char* fbuffer) {
	int* pAddressOfEntryPoint;
	pAddressOfEntryPoint = (int*)( ptrOptionPE(fbuffer) + 16 );

	return pAddressOfEntryPoint;
}

int* ptrSizeOfImage(char* fbuffer) {
	int* pSizeOfImage;
	//首先获取image在内存中的大小，即扩展PE头里面偏移56个字节的SizeOfImage(4byte)
	pSizeOfImage = (int*)(ptrOptionPE(fbuffer) + 56);

	//printf("function in ptrSizeOfImage()%x   %x\n", *((int*)( ptrOptionPE(fbuffer) + 56 )),1);
	//printf("function in ptrSizeOfImage()%x   %x\n", *pSizeOfImage,1);

	return pSizeOfImage;
}

int* ptrSizeOfHeaders(char* fbuffer) {
	//先拷贝头文件+节表，即获取扩展PE头偏移60个字节的SizeOfHeaders(4 byte)里面的值
	int* pSizeOfHeaders;
	pSizeOfHeaders = (int*)(ptrOptionPE(fbuffer) + 60);
	//int SizeOfHeaders;
	//SizeOfHeaders = *pSizeOfHeaders;
	//printf("SizeOfHeaders = %x\n", SizeOfHeaders);

	return pSizeOfHeaders;
}

short* ptrSizeOfOptionalHeader(char* fbuffer) {
	//PE标记起始地址，即e_lfanew
	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	//SizeOfOptionalHeader
	short* pSizeOfOptionalHeader;
	pSizeOfOptionalHeader = (short*)(fbuffer + e_lfanew + 20);
	//short SizeOfOptionalHeader;
	//SizeOfOptionalHeader = *pSizeOfOptionalHeader;
	//printf("SizeOfOptionalHeader = %x\n", SizeOfOptionalHeader);

	return pSizeOfOptionalHeader;
}

char* ptrSection(char* fbuffer) {
	char* ret;

	ret = fbuffer + *ptre_lfanew(fbuffer) + 24 + *ptrSizeOfOptionalHeader(fbuffer);

	return ret;
}

int SectionOffset(char* fbuffer) {
	int sectionOffset;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	short SizeOfOptionalHeader;
	SizeOfOptionalHeader = *ptrSizeOfOptionalHeader(fbuffer);

	//按照下述公式求得节表的起始地址
	sectionOffset = e_lfanew + 24 + SizeOfOptionalHeader;
	return sectionOffset;

}

int* ptrVirtualAddress(char* ptrSection) {
	int* pVirtualAddress;
	pVirtualAddress = (int*)(ptrSection + 12);

	return pVirtualAddress;
}

int* ptrSizeOfRawData(char* ptrSection) {
	int* pSizeOfRawData;
	pSizeOfRawData = (int*)(ptrSection + 16);

	return pSizeOfRawData;
}

int* ptrPhysicalAddress(char* ptrSection) {
	int* pPhysicalAddress;
	pPhysicalAddress = (int*)(ptrSection + 8);

	return pPhysicalAddress;
}

int* ptrPointerToRawData(char* ptrSection) {
	int* pPointerToRawData;
	pPointerToRawData = (int*)(ptrSection + 20);

	return pPointerToRawData;
}

int* ptrCharacteristics(char* ptrSection) {
	int* pCharacteristics;
	pCharacteristics = (int*)(ptrSection + 36);

	return pCharacteristics;
}

short* ptrNumberOfSection(char* fbuffer) {
	//根据PE头获取节的数量，即PE头偏移6个字节的NumberOfSection(4)
	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	short* pNumberOfSection;
	pNumberOfSection = (short*)(fbuffer + e_lfanew + 6);
	//short NumberOfSection;
	//NumberOfSection = *pNumberOfSection;
	//printf("NumberOfSection = %d\n", NumberOfSection);

	return pNumberOfSection;
}

int* ptrImageBase(char* fbuffer) {
	char* pOPE = ptrOptionPE(fbuffer);

	int* pImageBase = (int*)(pOPE + 32);

	return pImageBase;
}


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
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;

typedef struct _IMAGE_NT_HEADERS {
	DWORD Signature;							//0		PE头标记，通常值为4550
	IMAGE_FILE_HEADER FileHeader;				//4		标准PE头 20字节。
	IMAGE_OPTIONAL_HEADER32 OptionalHeader32;	//24	扩展PE头 大小是可变的。
}IMAGE_NT_HEADERS32, * PIMAGE_NT_HEADERS32;

typedef struct _IMAGE_FILE_HEADER {
	WORD  Machine;								//0		运行在何种CPU上；0就是任意；014C是interl386及以后；8664则是x64也就是64位系统上；
	WORD  NumberOfSections;						//2		保存节的数量。.data、.text
	DWORD TimeDateStamp;						//4		由编译器填写的时间戳，和文件的创建、修改时间无关。
	DWORD PointerToSymbolTable;					//8		调试相关
	DWORD NumberOfSymbols;						//12	调试相关
	WORD  SizeOfOptionalHeader;					//16	可保存扩展PE头的大小，32位默认是0xE0(224字节)，64位默认是0xF0(240字节)，大小可以自定义
	WORD  Characteristics;						//18	这部分可根据其二进制位上是否置1，来判断该PE文件的相关属性。
}IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;
/*
*--------------Characteristics------------------ *
*0		文件不存在重定位信息 *
*1		文件是可执行的 *
*2		不存在行信息 *
*3		不存在符号信息 *
*4		调整工作集 *
*5		应用程序可调用2GB的地址（64位） *
*6		此标志保留 *
*7		小尾方式 *
*8		只在32位平台上运行 *
*9		不包含调试信息 *
*10	不能从可移动盘运行 *
*11	不能从网络运行 *
*12	系统文件（如驱动程序），不能直接运行。 *
*13	这是一个DLL文件。 *
*14	文件不能在多处理器计算机上运行 *
*15	大尾方式 *
*--------------Characteristics------------------ *
*/

typedef struct _IMAGE_OPTIONAL_HEADER {
	WORD  Magic;								//能准确识别该文件的位数，要是该成员位置上为0x10B则是32位程序，0x20B则是64位程序。
	BYTE  MajorLinkerVersion;					//链接器主版本号。
	BYTE  MinorLinkerVersion;					//链接器次版本号。
	DWORD SizeOfCode;							//所有代码节的总和，文件对齐后的大小。
	DWORD SizeOfInitializedData;				//包含所有已经初始化数据的节的总和，文件对齐后的大小。
	DWORD SizeOfUninitializedData;				//包含书友未初始化数据的节的总和，文件对齐后的大小。
	DWORD AddressOfEntryPoint;					//程序的入口。
	DWORD BaseOfCode;							//代码开始的基址。
	DWORD BaseOfData;							//数据开始的基址。可改
	DWORD ImageBase;							//内存镜像基址，每个程序运行时都有一个4GB的虚拟内存，而这个基址就是该PE文件在这4GB里面的起始地址。
	DWORD SectionAlignment;						//内存对齐，PE文件在内存上运行时，每个节数据的大小就是其整数倍的大小；PE的DOS头+PE头+节表的大小也是其整数倍
	DWORD FileAlignment;						//文件对齐，PE文件在磁盘上时，每个节数据的大小就是其整数倍的大小；PE的DOS头+PE头+节表的大小也是其整数倍。
	WORD  MajorOperatingSystemVersion;			//操作系统主版本号
	WORD  MinorOperatingSystemVersion;			//操作系统次版本号
	WORD  MajorImageVersion;					//PE文件主版本号
	WORD  MinorImageVersion;					//PE文件次版本号
	WORD  MajorSubsystemVersion;				//运行时所需系统主版本号
	WORD  MinorSubsystemVersion;				//运行时所需系统次版本号
	DWORD Win32VersionValue;					//子系统版本的值必须是0
	DWORD SizeOfImage;							//内存中整个PE文件的大小，可以比硬盘上的文件大，但必须是SectionAlignment值的整数倍，决定整个PE在内存中大小
	DWORD SizeOfHeaders;						//所有头+节表按照文件对齐后的大小，否则加载会出错。
	DWORD CheckSum;								//校验和，一些系统文件有要求，判断文件是否被修改。
	WORD  Subsystem;							//子系统。如果该成员值为1，则是驱动程序；2就是图形界面；3就是控制台程序、DLL。
	WORD  DllCharacteristics;					//文件特性。可通过类似标准PE头的文件属性查表来看。
	DWORD SizeOfStackReserve;					//初始化时保留栈的大小。
	DWORD SizeOfStackCommit;					//初始化时实际提交的栈大小。
	DWORD SizeOfHeapReserve;					//初始化时保留的堆大小。
	DWORD SizeOfHeapCommit;						//初始化时实际提交的堆大小。
	DWORD LoaderFlags;							//调试相关。
	DWORD NumberOfRvaAndSizes;					//目录项目数。可知道该文件使用了多少个表。
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];//有多少张表，该数据的大小就是多少。
}IMAGE_OPTIONAL_HEADER32, * PIMAGE_OPTINAL_HEADER32;

/*
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
*/
#define IMAGE_SIZEOF_SHORT_NAME 8
typedef struct _IMAGE_SECTION_HEADER {
	BYTE NAME[IMAGE_SIZEOF_SHORT_NAME];				//8字节,节表的名字，ascii字符，只截取8个字节
	union {
		DWORD PhysicalAddress;						//4字节，就是该节在没有对齐前的尺寸，可以不准确．
		DWORD VirtualSize;							//联合体共用一块4字节的内存.
	}Misc;
	DWORD VirtualAddress;							//4字节,该节在内存中的偏移量,加上ImageBase等于在内存中的地址
	DWORD SizeOfRawData;							//4字节,该节在文件对齐后的大小.
	DWORD PointerToRawData;							//4字节,该节在文件中的偏移量,决定节数据在文件的位置.
	DWORD PointerToRelocations;						//4字节,调试相关
	DWORD PointerToLinenumbers;						//4字节,调试相关
	WORD  NumberOfRelocations;						//4字节,调试相关
	WORD  NumberOfLinenumbers;						//4字节,调试相关
	DWORD Characteristics;							//4字节,节属性,查表判断其属性.
}IMAGE_SECTION_HEADER, * PIMAGE_SECTION_HEADER;
/*
************************节表属性***********************************************************
* 5		IMAGE_SCN_CODE						//节中包含代码
* 6		IMAGE_SCN_CNT_INTIALIZED_DATA		//节中包含已初始化数据
* 7		IMAGE_SCN_CUT_UNINITIALIZED_DATA	//节中包含未初始化数据
* 8		IMAGE_SCN_LNK_OTHER					//保留供将来使用
* 25	IMAGE_SCN_MEM_DISCARDABLE			//节中的数据在进程开始以后将被丢弃，如.reloc
* 26	IMAGE_SCN_MEM_NOT_CACHED			//节中的数据不会经过缓存
* 27	IMAGE_SCN_MEM_NOT_PAGED				//节中的数据不会被交换到磁盘
* 28	IMAGE_SCN_MEM_SHARED				//表示节中的数据将被不同的进程所共享
* 29	IMAGE_SCN_MEM_EXECUTE				//映射到内存后的页面包含可执行的属性
* 30	MAGE_SCN_MEM_READ					//映射到内存后的页面包含可读属性
* 31	IMAGE_SCN_MEM_WRITE					//映射到内存后的页面包含可写属性
* ******************************************************************************************
*/
#endif // !PE_POINTER_H
#pragma once
