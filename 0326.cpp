#include "defDLL.h"
#include <Windows.h>
#include <stdio.h>

//2、使用工具打开修改后的DLL 看能否正常解析.
//3、在DLL中新增一个节，并将重定位表移动到这个新的节中.
//4、修改DLL的ImageBase,根据重定位表修正，然后存盘.看DLL是否可以使用.

//1、在DLL新增一个节，并将导出表信息移动到这个新的节中.
/*
 *
 *
 *
 * 移动addressoffunctions表
 * 移动nameofordinal表
 * 移动名字表
 *
 *
 *
 *
 *
 *
 *
 *
 */
 //计算节表后面的剩余空间
int SectionRestSpace(char* fbuffer) {
	//节表起始地址
	char* pSection = ptrSection(fbuffer);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(fbuffer);

	//节表结束的FOA
	int SectionEndFOA;
	SectionEndFOA = pSection + 40 * NumberOfSection - fbuffer;

	int SpaceMin, CurrentSpace;

	SpaceMin = *ptrPointerToRawData(pSection) - SectionEndFOA;
	//通过遍历节表信息，获取每一个节的虚拟地址范围（VirtualAddress ~ VirtualAddress + SizeRawData）
	for (int i = 0; i < NumberOfSection - 1; i++) {
		//printf("%d\n", SpaceMin);
		CurrentSpace = *ptrPointerToRawData(ptrSection(fbuffer) + 40 * i) - SectionEndFOA;

		if (SpaceMin < 0) {
			SpaceMin = CurrentSpace;
		}
		else {
			if (SpaceMin > CurrentSpace) {
				SpaceMin = CurrentSpace;
			}
		}

	}
	printf("The Min Rest Space is %x\n", SpaceMin);
	printf("There are %d Sections Can be added~\n", (SpaceMin - 40) / 40);
	return SpaceMin;
}

int GetSizeOfFbuffer(char* fbuffer) {
	//第一个节表指针
	char* pSection;
	pSection = ptrSection(fbuffer);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(fbuffer);

	//初始化文件大小为0
	int FileSize;
	FileSize = 0;

	int PointerToRawData;
	int SizeOfRawData;

	for (int i = 0; i < NumberOfSection; i++) {
		PointerToRawData = *ptrPointerToRawData(pSection + i * 40);
		SizeOfRawData = *ptrSizeOfRawData(pSection + i * 40);

		if (PointerToRawData + SizeOfRawData > FileSize) {
			FileSize = PointerToRawData + SizeOfRawData;
		}
		//printf("%x\n", FileSize);
	}
	printf("FileSize = %x\n", FileSize);
	return FileSize;
}

//3、定义一个函数，能够返回对齐后的大小Align(int x, int y)
int FileAlignment(char* fbuffer, int LengthOfData) {
	int result;
	printf("LengthOfData = %x\n", LengthOfData);
	int LengthOfFileAlign;
	LengthOfFileAlign = *ptrFileAlignment(fbuffer);
	printf("LengthOfFileAlign = %x\n", LengthOfFileAlign);

	result = (LengthOfData / LengthOfFileAlign + 1) * LengthOfFileAlign;
	printf("result = %x\n", result);

	return result;
}

int SectionAlignment(char* fbuffer, int LengthOfData) {
	int result;
	printf("LengthOfData = %x\n", LengthOfData);
	int LengthOfMemoryAlign;
	LengthOfMemoryAlign = *ptrMemoryAlignment(fbuffer);
	printf("LengthOfFileAlign = %x\n", LengthOfMemoryAlign);

	result = (LengthOfData / LengthOfMemoryAlign + 1) * LengthOfMemoryAlign;
	printf("result = %x\n", result);

	return result;
}

int SectionCharacteristics(char* fbuffer, int rva) {
	char* pSection;
	IMAGE_SECTION_HEADER* Section;
	int VirtualAddressRVA;
	int SizeOfRawData;

	int numberofsection;
	numberofsection = *ptrNumberOfSection(fbuffer);

	for (int i = 0; i < numberofsection; i++) {
		pSection = ptrSection(fbuffer) + i * 40;
		Section = (IMAGE_SECTION_HEADER*)pSection;
		if (Section->VirtualAddress <= rva && Section->VirtualAddress + Section->SizeOfRawData) {
			return Section->Characteristics;
		}
	}
	printf("can\'t find the RVA in which Section~\n");
	return 0;

}

typedef struct PEstruct64 {
	IMAGE_DOS_HEADER DOSHEADER;
	IMAGE_FILE_HEADER PEHEADER;
	IMAGE_OPTIONAL_HEADER64 OPEHEADER;
	IMAGE_EXPORT_DIRECTORY EXPORT;
	IMAGE_RELOCATION RELOCATION;
	IMAGE_IMPORT_DESCRIPTOR IMPORT;
};

PEstruct64 InitPEstruct(char* fbuffer) {
	PEstruct64 PE;

	memcpy(&PE.DOSHEADER, fbuffer, sizeof(IMAGE_DOS_HEADER));
	memcpy(&PE.PEHEADER, ptrPE(fbuffer), sizeof(IMAGE_FILE_HEADER));
	memcpy(&PE.OPEHEADER, ptrOptionPE(fbuffer), sizeof(IMAGE_OPTIONAL_HEADER64));
	memcpy(&PE.EXPORT, fbuffer + RVA_TO_FOA(fbuffer, PE.OPEHEADER.DataDirectory[0].VirtualAddress), sizeof(IMAGE_EXPORT_DIRECTORY));
	memcpy(&PE.RELOCATION, fbuffer + RVA_TO_FOA(fbuffer, PE.OPEHEADER.DataDirectory[5].VirtualAddress), sizeof(IMAGE_RELOCATION));
	memcpy(&PE.IMPORT, fbuffer + RVA_TO_FOA(fbuffer, PE.OPEHEADER.DataDirectory[1].VirtualAddress), sizeof(IMAGE_IMPORT_DESCRIPTOR));
	//PE.PEHEADER = (IMAGE_FILE_HEADER*)(ptrPE(fbuffer));
	//PE.OPEHEADER = (IMAGE_OPTIONAL_HEADER64*)(ptrOptionPE(fbuffer));

	return PE;
}

void MoveExportDirectory(char* fbuffer) {
	//获取当前fbuffer的导出表
	_IMAGE_OPTIONAL_HEADER64* OPE64;
	OPE64 = (_IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	int ExportTableRVA;
	ExportTableRVA = OPE64->DataDirectory[0].VirtualAddress;

	int ExportTableFOA;
	ExportTableFOA = RVA_TO_FOA(fbuffer, ExportTableRVA);

	IMAGE_EXPORT_DIRECTORY* ExportTable;
	ExportTable = (IMAGE_EXPORT_DIRECTORY*)(fbuffer + ExportTableFOA);

	//获取导出表的几个基本信息：函数地址表、名字表、名字序号表、函数个数、名字个数
	int AddrFuncTableFOA;
	int NameTablePtrFOA;
	int NameOrdinalFOA;
	int NumberOfFunction;
	int NumberOfName;
	//函数地址表、名字表、名字序号表
	AddrFuncTableFOA = RVA_TO_FOA(fbuffer, ExportTable->AddressOfFunctions);
	NameTablePtrFOA = RVA_TO_FOA(fbuffer, ExportTable->AddressOfNames);
	NameOrdinalFOA = RVA_TO_FOA(fbuffer, ExportTable->AddressOfNameOrdinals);
	//函数个数、名字个数
	NumberOfFunction = ExportTable->NumberOfFunctions;
	NumberOfName = ExportTable->NumberOfNames;

	//判断能够新增几个节表信息
	int space;
	space = SectionRestSpace(fbuffer);

	//判断导出表占用空间：40字节
	int sizeofExportDict;
	sizeofExportDict = sizeof(_IMAGE_EXPORT_DIRECTORY);

	//判断地址表占用空间：(9-2+1)*4 = 32字节
	int SizeOfAddrFuncTable;
	SizeOfAddrFuncTable = NumberOfFunction * 4;

	//判断名字表占用空间：(4*4)=16字节
	int SizeOfNameTable;
	SizeOfNameTable = NumberOfName * 4;

	//判断名字序号表占用空间：(4*2)=8字节
	int SizeOfNameOrdinalsTable;
	SizeOfNameOrdinalsTable = NumberOfName * 2;

	//判断名字占用空间：5 + 4 + 6 + 6 = 21字节
	int SizeOfName;
	int* ptrNameTable;
	int NamePtrFOA;
	char* ptrName;
	int NameLength;
	NameLength = 0;
	for (int i = 0; i < NumberOfName; i++) {
		ptrNameTable = (int*)(fbuffer + NameTablePtrFOA) + i;
		NamePtrFOA = RVA_TO_FOA(fbuffer, *ptrNameTable);
		ptrName = fbuffer + NamePtrFOA;
		NameLength = NameLength + strlen(ptrName);
		//printf("NameLength = %d\n", NameLength);
	}
	//每个名字字符串后面都少算了一个字节，为字符串结束符0
	SizeOfName = NameLength + NumberOfName;

	//获取导出表名字长度
	int SizeOfDLLName;
	int DLLNameFOA;
	DLLNameFOA = RVA_TO_FOA(fbuffer, ExportTable->Name);

	SizeOfDLLName = strlen(fbuffer + DLLNameFOA) + 1;


	//获取当前文件的磁盘大小：ec00
	int SizeOfFile;
	SizeOfFile = GetSizeOfFbuffer(fbuffer);

	//获取文件的文件对齐参数：200
	int FileAlign;
	FileAlign = *ptrFileAlignment(fbuffer);

	//按照文件对齐参数，计算需要增加的内存空间
	int PhysicalSize;
	int SizeOfRawData;
	PhysicalSize = sizeofExportDict + SizeOfAddrFuncTable + SizeOfNameTable + SizeOfNameOrdinalsTable + SizeOfName + SizeOfDLLName;
	//printf("%d\t%d\t%d\t%d\t%d\n", sizeofExportDict , SizeOfAddrFuncTable, SizeOfNameTable , SizeOfNameOrdinalsTable , NameLength);
	SizeOfRawData = FileAlignment(fbuffer, PhysicalSize);

	//申请一段新内存
	char* nbuffer;
	nbuffer = (char*)malloc(SizeOfRawData + SizeOfFile);





	//写入新节表信息
	if (nbuffer != NULL) {

		memset(nbuffer, 0, SizeOfRawData + SizeOfFile);
		memcpy(nbuffer, fbuffer, SizeOfFile);

		//获取PE指针结构体
		PEstruct64 NewPE;
		NewPE = InitPEstruct(nbuffer);
		/*****************************************开始新增节*****************************************************/
		//准备新节内容
		char Section_Name[] = "\.newout";												//节名称
		int Section_PhysicalSize;
		Section_PhysicalSize = PhysicalSize;										//节物理大小
		int Section_VirtualAddress;
		Section_VirtualAddress = *ptrSizeOfImage(nbuffer);							//节虚拟地址
		int Section_PtrRawDataFOA;
		Section_PtrRawDataFOA = SizeOfFile;											//节FOA
		int Section_SizeOfRawData;
		Section_SizeOfRawData = SizeOfRawData;										//节文件对齐的大小
		//获取节属性，需要和原始导出表相同
		int ExportTableRVA;
		ExportTableRVA = NewPE.OPEHEADER.DataDirectory[0].VirtualAddress;			//获取导出表的RVA
		int Section_Characteristics;
		Section_Characteristics = SectionCharacteristics(nbuffer, ExportTableRVA);	//节属性SectionCharacteristics

		IMAGE_SECTION_HEADER* ptrNewSection;
		ptrNewSection = (IMAGE_SECTION_HEADER*)(ptrSection(nbuffer) + 40 * NewPE.PEHEADER.NumberOfSections);	//新节起始地址
		//开始写入新节
		memcpy(ptrNewSection, Section_Name, sizeof(Section_Name));					//赋值节名称
		ptrNewSection->Misc.PhysicalAddress = Section_PhysicalSize;					//赋值节真实大小
		ptrNewSection->VirtualAddress = Section_VirtualAddress;						//赋值节的虚拟地址，sizeofimage的结束地址
		ptrNewSection->PointerToRawData = Section_PtrRawDataFOA;					//赋值节的文件偏移
		ptrNewSection->SizeOfRawData = Section_SizeOfRawData;						//赋值文件对齐后的大小
		ptrNewSection->Characteristics = Section_Characteristics;					//赋值文件属性
		/*****************************************完成新增节*****************************************************/
		//修改sizeofimage;
		*ptrSizeOfImage(nbuffer) = *ptrSizeOfImage(nbuffer) + SectionAlignment(nbuffer, Section_PhysicalSize);
		//修改节数量
		*ptrNumberOfSection(nbuffer) = *ptrNumberOfSection(nbuffer) + 1;

		char* CurPtr;

		CurPtr = nbuffer + SizeOfFile;

		//移动导出表
		printf("------try to move ExportTable table------\n");
		memcpy(CurPtr, &NewPE.EXPORT, sizeof(NewPE.EXPORT));

		IMAGE_EXPORT_DIRECTORY* NewExportTable;
		NewExportTable = (IMAGE_EXPORT_DIRECTORY*)CurPtr;

		IMAGE_OPTIONAL_HEADER64* OPE;
		OPE = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(nbuffer);

		OPE->DataDirectory[0].VirtualAddress = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);	//修改导出表的地址

		CurPtr = CurPtr + sizeof(NewPE.EXPORT);

		//移动导出表名字如：XXX.dll
		printf("------try to move DLLname------\n");
		int DLLNameFOA;
		DLLNameFOA = RVA_TO_FOA(nbuffer, NewExportTable->Name);
		char* ptrDLLName;
		ptrDLLName = nbuffer + DLLNameFOA;

		memcpy(CurPtr, ptrDLLName, strlen(ptrDLLName));

		NewExportTable->Name = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);		//修改导出表的库名RVA

		CurPtr = CurPtr + strlen(ptrDLLName) + 1;

		//移动地址表：获取地址表的FOA，目的地址，地址表大小(32byte)
		printf("------try to move function address table------\n");
		int FuncAddrTableFOA;
		FuncAddrTableFOA = RVA_TO_FOA(nbuffer, NewPE.EXPORT.AddressOfFunctions);
		memcpy(CurPtr, nbuffer + FuncAddrTableFOA, SizeOfAddrFuncTable);

		FuncAddrTableFOA = SizeOfFile;

		NewExportTable->AddressOfFunctions = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);//修改导出表的函数地址表

		CurPtr = CurPtr + SizeOfAddrFuncTable;
		//移动序号表：序号表FOA，目的地址，序号表大小(16byte)
		printf("------try to move name ordinals table------\n");
		int OrdinalTableFOA;
		OrdinalTableFOA = RVA_TO_FOA(nbuffer, NewPE.EXPORT.AddressOfNameOrdinals);
		memcpy(CurPtr, nbuffer + OrdinalTableFOA, SizeOfNameOrdinalsTable);

		NewExportTable->AddressOfNameOrdinals = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);//修改导出表的序号表地址

		CurPtr = CurPtr + SizeOfNameOrdinalsTable;
		//移动名字表 & 名字
		//找到名字指针、找到名字、获取名字长度、开始复制
		printf("------try to move name table------\n");
		int NamePtrArrayRVA;
		NamePtrArrayRVA = NewPE.EXPORT.AddressOfNames;

		int NamePtrArrayFOA;
		NamePtrArrayFOA = RVA_TO_FOA(nbuffer, NamePtrArrayRVA);

		int* NamePtr;
		int NameFOA;
		int LengthOfName;

		char* NameInSection;
		NameInSection = CurPtr + 4 * NumberOfName;

		int* NamePtrInSection;
		NamePtrInSection = (int*)CurPtr;

		int NewNamePtrRVA;

		for (int i = 0; i < NewPE.EXPORT.NumberOfNames; i++) {

			NamePtr = (int*)(nbuffer + NamePtrArrayFOA) + i;		//名字指针数组
			NameFOA = RVA_TO_FOA(nbuffer, *NamePtr);				//名字FOA

			LengthOfName = strlen(nbuffer + NameFOA);				//名字长度

			memcpy(NameInSection, nbuffer + NameFOA, LengthOfName);	//开始复制

			NewNamePtrRVA = FOA_TO_RVA(nbuffer, NameInSection - nbuffer);//根据名字的FOA计算名字的RVA

			*(NamePtrInSection + i) = NewNamePtrRVA;				//将计算出的RVA放到新的名字指针表中

			NameInSection = NameInSection + LengthOfName + 1;		//设置名字拷贝完的指针位置，+1是预留一个0的结束标记

		}
		NewExportTable->AddressOfNames = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);


		//将文件导出
		char fpath[] = "E:\\Github\\repo\\Demo\\StudyDLL\\x64\\Debug\\0326\\DLLMoveExport.dll";
		BufferToFile(nbuffer, fpath);


	}

	printf("\n");
}

int SizeOfFile(char* fbuffer) {
	//获取块的数量
	short numberofsection;
	numberofsection = *ptrNumberOfSection(fbuffer);

	//获取块的起始地址
	char* section0;
	section0 = ptrSection(fbuffer);

	//获取最后一节块的起始地址
	char* section;
	section = section0 + (numberofsection - 1) * 40;

	_IMAGE_SECTION_HEADER* SectionHeader;
	SectionHeader = (_IMAGE_SECTION_HEADER*)section;

	return SectionHeader->SizeOfRawData + SectionHeader->PointerToRawData;
}

void MoveRelocationDirectory(char* fbuffer, unsigned long long imagebase) {

	int FileSize;
	FileSize = SizeOfFile(fbuffer);
	printf("FileSize = %x\n", FileSize);

	char* nbuffer;
	nbuffer = NewBuffer(FileSize);

	memcpy(nbuffer, fbuffer, FileSize);

	//尝试找到重定位表
	IMAGE_OPTIONAL_HEADER64* OPE;
	OPE = (IMAGE_OPTIONAL_HEADER64*)(ptrOptionPE(nbuffer));

	int RelocatedRVA;
	RelocatedRVA = OPE->DataDirectory[5].VirtualAddress;

	int RelocatedFOA;
	RelocatedFOA = RVA_TO_FOA(nbuffer, RelocatedRVA);

	char* ptrRelocated;
	ptrRelocated = nbuffer + RelocatedFOA;

	_IMAGE_BASE_RELOCATION* Relocation;
	Relocation = (_IMAGE_BASE_RELOCATION*)ptrRelocated;

	//printf("Relocation->VirtualAddress = %x\n", Relocation->VirtualAddress);
	//printf("Relocation->SizeOfBlock = %x\n", Relocation->SizeOfBlock);
	short* ptrRawAddr;

	int offset;
	offset = imagebase - OPE->ImageBase;		//获取imagebase的偏移量

	OPE->ImageBase = imagebase;

	unsigned long long* PublicArg;

	while (Relocation->VirtualAddress != 0 && Relocation->SizeOfBlock != 0) {
		printf("Current VirtualAddress: %x\n", Relocation->VirtualAddress);
		printf("Current SizeOfBlock: %x\n", Relocation->SizeOfBlock);

		unsigned long sizeofblock = (Relocation->SizeOfBlock - 8) / 2;

		for (int i = 0; i < sizeofblock; i++) {
			//在该重定位区块上，遍历所有的对象(2字节为一个重定位对象)
			ptrRawAddr = (short*)(ptrRelocated + 8) + i;

			unsigned short Raw;
			Raw = *ptrRawAddr;

			int RawRVA;
			int RawFOA;

			//注意64bit的高四位为0xa时，说明该对象需要被修改；在32bit中这个值是为3，说明需要被修改
			if (Raw >> 12 == 0xa) {
				//如何获取到低12位的值？
				//将16位(2字节)的值 - 0xa000 = 低12位的值
				Raw = Raw - 0xa000;
				RawRVA = Relocation->VirtualAddress + Raw;
				RawFOA = RVA_TO_FOA(nbuffer, RawRVA);

				printf("RVA = %x (FOA = %x) need to be relocated\n", RawRVA, RawFOA);

				PublicArg = (unsigned long long*)(nbuffer + RawFOA);
				//printf("PublicArg = %x\n", *PublicArg);
				*PublicArg = *PublicArg + (unsigned long long)offset;
			}
			//printf("%x\n", Raw >>12);
		}

		ptrRelocated = ptrRelocated + Relocation->SizeOfBlock;
		Relocation = (_IMAGE_BASE_RELOCATION*)ptrRelocated;
	}

	char fpath[] = "E:\\Github\\repo\\Demo\\StudyDLL\\x64\\Debug\\0326\\DLLMoveReloction.dll";
	BufferToFile(nbuffer, fpath);
}

int main() {
	char fpath[] = "E:\\Github\\repo\\Demo\\StudyDLL\\x64\\Debug\\0326\\StudyDLL.dll";
	char* fbuffer;
	fbuffer = FileToBuffer(fpath);

	MoveExportDirectory(fbuffer);

	MoveRelocationDirectory(fbuffer, 0x190000000);
}