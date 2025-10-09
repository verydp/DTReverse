#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

//根据PE文件获取文件的具体大小
int SizePE(char* fbuffer) {
	int NumSection;
	NumSection = *ptrNumberOfSection(fbuffer);

	IMAGE_SECTION_HEADER* section;
	int sizePE = 0;
	for (int i = 0; i < NumSection; i++) {
		section = (IMAGE_SECTION_HEADER*)(ptrSection(fbuffer) + 40 * i);
		if (section->PointerToRawData + section->Characteristics > sizePE) {
			sizePE = section->PointerToRawData + section->SizeOfRawData;
		}
	}

	return sizePE;
}

int len(char* string) {
	int i = 0;
	while (string[i] != 0) {
		i++;
	}
	return i + 1;
}

//自动新增节，不影响原始程序的使用

//思路(最好还是借助PE工具，对于修改的节信息比较直观)
// 涉及修改的原属性有sizeofimage、numberofsection
// 涉及新增的只有节数据和节表
// 新增的节表参照工具上面的节表信息会更容易直观的修改
//1、修改节的数量
//2、新增节表
//3、添加节。修改节表的三个属性(pointertorawdata、sizeofrawdata、virtualaddress)
//4、修改sizeofimage

//基础逻辑：读取文件buffer，判断节表是否有新增空间，确认需要新增节的大小，重新申请内存空间，写入新增内容
//参数为原始fbuffer和要写入的节大小
//返回值为一个char*指针，一块新的内存
char* AddSection(char* fbuffer ,int sizeofshellcode, char SectionName[]) {
	//获取节的基本信息：节数量、节头大小、节的起始FOA
	int NumSection;
	NumSection = *ptrNumberOfSection(fbuffer);

	int CurSectionSize;
	CurSectionSize = sizeof(IMAGE_SECTION_HEADER) * (NumSection + 1);

	int section0FOA;
	section0FOA = ptrSection(fbuffer) - fbuffer;

	//该模块主要是判断是否有足够的空间添加一个新节
	IMAGE_SECTION_HEADER* section;
	int X = 1;
	for (int i = 0; i < NumSection; i++) {
		section = (IMAGE_SECTION_HEADER*)(ptrSection(fbuffer) + 40 * i);

		if (section->PointerToRawData - (section0FOA + CurSectionSize) < 40) {
			printf("The rest space between header and section less than 40 ,can not add a new sectionheader~\n");
			X = 0;
		}
	}



	if (X) {
		int FileAlign;
		int MemAlign;
		FileAlign = *ptrFileAlignment(fbuffer);
		MemAlign = *ptrMemoryAlignment(fbuffer);


		//获取当前文件的大小

		int NewSectionFileSize;
		int NewSectionMemSize;
		int SrcFileSize;
		int Filesize;

		SrcFileSize = SizePE(fbuffer);
		NewSectionFileSize = ((sizeofshellcode / FileAlign) + 1) * FileAlign;
		NewSectionMemSize = ((sizeofshellcode / MemAlign) + 1) * MemAlign;
		Filesize = SrcFileSize + NewSectionFileSize;

		//申请一段内存空间放置，扩展后的文件buffer。
		char* nbuffer;
		nbuffer = NewBuffer(Filesize);

		memcpy(nbuffer, fbuffer, SrcFileSize);

		IMAGE_OPTIONAL_HEADER64* OPE;
		OPE = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(nbuffer);

		IMAGE_FILE_HEADER* PE;
		PE = (IMAGE_FILE_HEADER*)ptrPE(nbuffer);

		//修改节表的三个属性(pointertorawdata、sizeofrawdata、virtualaddress)
		IMAGE_SECTION_HEADER* sectionEnd;
		sectionEnd = (IMAGE_SECTION_HEADER*)(ptrSection(nbuffer) + 40 * (PE->NumberOfSections - 1) );
		
		int EndFOA;
		int EndRVA;
		int EndSectionFileAlign;
		int EndSectionMemAlign;

		EndFOA = sectionEnd->PointerToRawData + (sectionEnd->SizeOfRawData % FileAlign == 0 ? sectionEnd->SizeOfRawData / FileAlign: sectionEnd->SizeOfRawData / FileAlign + 1) * FileAlign;
		EndRVA = sectionEnd->VirtualAddress + (sectionEnd->SizeOfRawData % MemAlign == 0 ? sectionEnd->SizeOfRawData / MemAlign: sectionEnd->SizeOfRawData / MemAlign + 1) * MemAlign;

		printf("EndFOA = %x EndRVA = %x\n", EndFOA, EndRVA);

		IMAGE_SECTION_HEADER* sectionNew;
		sectionNew = (IMAGE_SECTION_HEADER*)(ptrSection(nbuffer) + 40 * PE->NumberOfSections);

		sectionNew->PointerToRawData = EndFOA ;
		sectionNew->VirtualAddress = EndRVA;
		sectionNew->SizeOfRawData = (sizeofshellcode % FileAlign == 0 ? sizeofshellcode / FileAlign : sizeofshellcode / FileAlign + 1)* FileAlign;
		sectionNew->Characteristics = sectionEnd->Characteristics;
		sectionNew->Misc.PhysicalAddress = sizeofshellcode;
		//修改新节的名字
		memcpy(sectionNew->Name, SectionName, len(SectionName));

		printf("sectionNew->SizeOfRawData = %x\n", sectionNew->SizeOfRawData);

		//1、修改节的数量
		PE->NumberOfSections++;

		//printf("%d\n", PE->NumberOfSections);

		//4、修改sizeofimage
		OPE->SizeOfImage = NewSectionMemSize + EndRVA;

		printf("sectionNew: PointerToRawData = %x VirtualAddress = %x SizeOfRawData = %x Characteristics = %x\n", 
			sectionNew->PointerToRawData, sectionNew->VirtualAddress, sectionNew->SizeOfRawData, sectionNew->Characteristics);


		printf("sizePE = %d\n", SizePE(nbuffer));

		return nbuffer;

	}

}

void ImportTableInjection(char* fbuffer, char* dllName, char* funcName) {
	//该模块是导入表
	//获取文件的导入表
	IMAGE_OPTIONAL_HEADER64* OPE;
	OPE = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	int ImportDescriptorRVA;
	int ImportDescriptorFOA;
 	ImportDescriptorRVA = OPE->DataDirectory[1].VirtualAddress;
	ImportDescriptorFOA = RVA_TO_FOA(fbuffer, ImportDescriptorRVA);


	IMAGE_IMPORT_DESCRIPTOR* ImportDescriptor;
	ImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)(fbuffer + ImportDescriptorFOA);
	//printf("%d\n", sizeof(IMAGE_IMPORT_DESCRIPTOR));



	//遍历是否新增节成功。

	char* psection;
	for (int i = 0; i < *ptrNumberOfSection(fbuffer); i++) {

		psection = ptrSection(fbuffer) + i * 40;//文件头(buffer) + PE头的偏移量/a_lfanew(0x108) + PE Signature(4) + PE(20) + option PE(0xf0)

		char (*name)[8];
		name = (char (*)[8])psection;			//8字节，显示节表名字。
		//printf("%s\n", *name);					

		int* Misc;
		Misc = (int*)(psection + 8);			//4字节，未对齐的尺寸，可修改
		//printf("%x\n", *Misc);					

		int* VirtualAddress;
		VirtualAddress = (int*)(psection + 12);	//4字节，该节在内存中距离ImageBase地址的距离
		//printf("%x\n", *VirtualAddress);

		int* SizeOfRawData;
		SizeOfRawData = (int*)(psection + 16);	//4字节，该节在文件对齐后的大小
		//printf("%x\n", *SizeOfRawData);

		int* PointerToRawData;
		PointerToRawData = (int*)(psection + 20);//4字节，该节在文件中的起始地址
		//printf("%x\n", *PointerToRawData);

		int* Characteristics;
		Characteristics = (int*)(psection + 36);//4字节，该节在内存中的属性、r-w-x
		//printf("%x\n", *Characteristics);

		printf("%s\t%x\t%x\t\t%x\t\t%x\t%x\n", *name, *Misc, *VirtualAddress, *SizeOfRawData, *PointerToRawData, *Characteristics);
	}



	char* tmptr;
	tmptr = fbuffer + 0x1e00;

	ULONGLONG* FuncAddress;

	int NumImportDescriptor;
	NumImportDescriptor = 0;

	for (; ImportDescriptor->Characteristics != 0; ImportDescriptor += 1) {
		//显示DLL库名字
		int NameRVA;
		int NameFOA;
		NameRVA = ImportDescriptor->Name;
		NameFOA = RVA_TO_FOA(fbuffer, NameRVA);
		printf("===========Import DLL ");
		showstring(fbuffer + NameFOA);
		printf("===========\n");

		//遍历导入表述表，并将其复制到新增节中。

		memcpy(tmptr, ImportDescriptor, 20);

		tmptr += 20;

		NumImportDescriptor++;

	}
	//重新修改扩展PE头的导入表起始地址
	OPE->DataDirectory[1].VirtualAddress = FOA_TO_RVA(fbuffer, tmptr - NumImportDescriptor * 20 - fbuffer);
	
	//一张导入表20个字节，原来有两张导入表，新增了一张导入表，空白20个0，一共80个字节=0x50
	OPE->DataDirectory[1].Size = 0x50;
	
	//注意!!!新增的一个导入描述表后面需要一个20个字节的0，代表结束符号。
	memset(tmptr, 0, 100);

	ImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)tmptr;

	/***************************************************************************************************************
	**20字节导入描述表 + 空20字节 + 16字节IMAGE_TRUNK_DATA64 + 16字节IMAGE_TRUNK_DATA64 + dll库名 + function函数名**
	****************************************************************************************************************/
	
	//复制DLL库名StudyDLL.dll、function名window
	memcpy(tmptr + 80, dllName, len(dllName));

	memcpy(tmptr + 82 + len(dllName), funcName, len(funcName));

	//重新修改导入描述表的虚拟地址
	ImportDescriptor->Name = FOA_TO_RVA(fbuffer, tmptr - fbuffer + 80 );

	ImportDescriptor->FirstThunk = FOA_TO_RVA(fbuffer, tmptr - fbuffer + 40);

	ImportDescriptor->OriginalFirstThunk = FOA_TO_RVA(fbuffer, tmptr - fbuffer + 56);

	//两个函数名地址
	FuncAddress = (ULONGLONG*)(tmptr + 40);

	*FuncAddress = (ULONGLONG)FOA_TO_RVA(fbuffer, tmptr + 82 + len(dllName) - fbuffer);

	FuncAddress = (ULONGLONG*)(tmptr + 56);

	*FuncAddress = (ULONGLONG)FOA_TO_RVA(fbuffer, tmptr + 82 + len(dllName) - fbuffer);

	char finalpath[] = "E:\\Studies\\滴水课件\\2015-03-31导入表注入\\DLLinjectionmain.exe";

	BufferToFile(fbuffer, finalpath);
}


int main() {
	//将文件读取到内存中
	char fpath[] = "E:\\Studies\\滴水课件\\2015-03-31导入表注入\\main.exe";
	//char fpath[] = "D:\\IDA_Pro_7.5SP3\\IDA Pro 7.5 SP3(x86, x64, ARM, ARM64, PPC, PPC64, MIPS)\\ida.exe";
	char* fbuffer;
	fbuffer = FileToBuffer(fpath);

	char* nbuffer;
	char sectionname[] = ".new0";
	nbuffer = AddSection(fbuffer, 100, sectionname);

	char DllName[] = "StudyDLL\.dll";
	char funcName[] = "window";
	ImportTableInjection(nbuffer, DllName, funcName);





}
