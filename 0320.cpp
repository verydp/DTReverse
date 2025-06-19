#include <Windows.h>
#include <stdio.h>
#include "PEpointer.h"
#include "FileHandler.h"

#pragma warning(disable : 4996)


//1、扩大最后一个节，保证程序正常运行.
/**********************思路***************************
1、首先需要修改节的大小 physicsize = sizeofrawdata（需考虑对齐尺寸）
2、修改sizeofimage
*/

void function3_20_1() {
	char fpath[] = "D:\\7-Zip\\7zshellcode.exe";

	FILE* pfile;
	int iSize;
	pfile = fopen(fpath, "rb");

	fseek(pfile, 0, SEEK_END);
	iSize = ftell(pfile);
	rewind(pfile);

	IMAGE_DOS_HEADER* DosHeader;
	//获取最后一个节的信息
	IMAGE_PE_HEADER* PEHeader;
	IMAGE_OPE_HEADER* OPEHeader;
	SECTION_HEADER* SectionHeader0;
	SECTION_HEADER* SectionHeader5;

	char* fbuffer;
	char* nbuffer;
	fbuffer = (char*)malloc(iSize);
	nbuffer = (char*)malloc(iSize + 0x1000);
	
	if (fbuffer != NULL && nbuffer != NULL) {
		fread(fbuffer,iSize,1,pfile);
		DosHeader = (IMAGE_DOS_HEADER*)fbuffer;
		//printf("%x\n", DosHeader->e_lfanew);



		PEHeader = (IMAGE_PE_HEADER*)(fbuffer + 4 + DosHeader->e_lfanew);
		OPEHeader = (IMAGE_OPE_HEADER*)(fbuffer + DosHeader->e_lfanew + 24);
		SectionHeader0 = (SECTION_HEADER*)(fbuffer + DosHeader->e_lfanew + 24 + PEHeader->SizeOfOptionalHeader);

		//获取最后一个节
		SectionHeader5 = SectionHeader0 + 5;

		//将最后一个节的physicsize = sizeofrawdata增加0x1000
		//printf("%x\n", SectionHeader5->SizeOfRawData);
		//printf("%x\n", SectionHeader5->Misc);
		SectionHeader5->SizeOfRawData = 0x2000;
		SectionHeader5->Misc.PhysicalAddress = (int)0x2000;
		printf("%x\n", SectionHeader5->SizeOfRawData);
		printf("%x\n", SectionHeader5->Misc.VirtualSize);

		//将sizeofimage增加0x1000;
		OPEHeader->SizeOfImage = OPEHeader->SizeOfImage + 0x1000;


		//尝试将内存中的数据写入到一个文件中
		char rpath[] = "D:\\7-Zip\\7zshellcodeSectionExpand.exe";

		//将fbuffer的内容拷贝到nbuffer中
		memcpy(nbuffer, fbuffer, iSize);
		//将新增的0x1000个字节置为cc
		memset(nbuffer + iSize, 0xcc, 0x1000);

		BufferToFile(nbuffer, rpath);



		fclose(pfile);
	}
	else {
		printf("malloc defeat~");
	}

	



	
}

typedef struct FileAttribute {
	int iSize;				//文件大小
};



//2、将所有节合并，保证程序正常运行.
void function3_30_2() {
	//将文件模拟PE加载写入一块内存
	char fpath[] = "D:\\7-Zip\\7zshellcode.exe";
	char* fbuffer;
	char* nbuffer;
	SectionHeader* nsectionheader;
	IMAGE_PE_HEADER* PEHeader;
	IMAGE_OPE_HEADER* OPEHeader;


	fbuffer = FileToBuffer(fpath);
	//根据原始文件的PE头申请一块运行时大小的内存
	printf("%x\n", *ptrSizeOfImage(fbuffer));
	nbuffer = NewBuffer(*ptrSizeOfImage(fbuffer));
	memset(nbuffer, 0, *ptrSizeOfImage(fbuffer));
	FbufferToNbuffer(fbuffer, nbuffer);

	if (fbuffer != NULL && nbuffer != NULL) {
		//将节的数量设置为1;
		PEHeader = (IMAGE_PE_HEADER*)ptrPE(nbuffer);
		OPEHeader = (IMAGE_OPE_HEADER*)ptrOptionPE(nbuffer);
		//printf("%x\n", PEHeader->NumberOfSections);
		PEHeader->NumberOfSections = 1;
		//printf("%x\n", PEHeader->NumberOfSections);



		//将修改内存中的section0调整尺寸
		nsectionheader = (SectionHeader*)ptrSection(nbuffer);
		//printf("%x\n", sectionheader->PointerToRawData);
		//将pointertorawdata调整为virtualaddress
		nsectionheader->PointerToRawData = nsectionheader->VirtualAddress;
		//printf("%x\n", sectionheader->PointerToRawData);

		//modify the power in 1st section, ensure the deleted section  also save their power 
		int Characteristics;
		Characteristics = 0;
		SectionHeader* fsectionheader;
		fsectionheader = (SectionHeader*)ptrSection(fbuffer);

		for (int i = 0; i < 6; i++) {
			//printf("Characteristics = %x\n", Characteristics);
			Characteristics = Characteristics | (fsectionheader + i)->Characteristics;
		}
		
		nsectionheader->Characteristics = Characteristics;
		

		//将节的大小设置为扩展后的大小sizeofimage - virtualaddress
		printf("%x\n", nsectionheader->SizeOfRawData);
		nsectionheader->SizeOfRawData = OPEHeader->SizeOfImage - nsectionheader->VirtualAddress;
		nsectionheader->Misc.VirtualSize = OPEHeader->SizeOfImage - nsectionheader->VirtualAddress;
		printf("%x\n", nsectionheader->SizeOfRawData);

		//将section1-5的节表内容置0
		//printf("%d\n", sizeof(SectionHeader));
		memset(nsectionheader + 1, 0, sizeof(SectionHeader) * 5);

		//将内存直接写入文件写入文件
		char rpath[] = "D:\\7-Zip\\7zshellcodeMerge.exe";

		MemBufferToFile(nbuffer, rpath);
	}
	
}

//3、定义一个函数，能够返回对齐后的大小Align(int x, int y)
//the first argument is the SizeOfRawData, the 2ed argument is the Alignment(it can be fileAlignment or MemoryAlignment)
int Align(int x, int y) {
	int result;
	printf("%d\n", x / y);
	result = (x / y + 1) * y;

	return result;
}


//4、编程输出全部目录项.
void function3_20_4() {
	char* DataDirectory[16];
	DataDirectory[0] = (char*)"Export Table\0";
	DataDirectory[1] = (char*)"Import Table\0";
	DataDirectory[2] = (char*)"Resource Table\0";
	DataDirectory[3] = (char*)"Exception Table\0";
	DataDirectory[4] = (char*)"Certificate Table\0";
	DataDirectory[5] = (char*)"Base Relocation Table\0";
	DataDirectory[6] = (char*)"Debug Directory\0";
	DataDirectory[7] = (char*)"Architecture\0";
	DataDirectory[8] = (char*)"Global Ptr\0";
	DataDirectory[9] = (char*)"TLS Table\0";
	DataDirectory[10] = (char*)"Load Config Table\0";
	DataDirectory[11] = (char*)"Bound Import\0";
	DataDirectory[12] = (char*)"Import Address Table (IAT)\0";
	DataDirectory[13] = (char*)"Delay Import Descriptor\0";
	DataDirectory[14] = (char*)"CLR Runtime Header\0";
	DataDirectory[15] = (char*)"Reserved\0";
	char fpath[] = "D:\\7-Zip\\7zshellcode.exe";
	char* fbuffer;

	fbuffer = FileToBuffer(fpath);

	_IMAGE_OPTIONAL_HEADER64* OPEHeader;
	OPEHeader = (_IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	//printf("%s\n", *DataDirectory[15]);
	
	for (int i = 0; i < OPEHeader->NumberOfRvaAndSizes; i++) {
		//printf("\t");
		for (int x = 0; *(DataDirectory[i] + x) != 0; x++) {
			printf("%c", *(DataDirectory[i] + x));
		}
		printf("\n");
		printf("VirtualAddress : %x\tSize : %x\n", OPEHeader->DataDirectory[i].VirtualAddress,OPEHeader->DataDirectory[i].Size);
		printf("\n");
	}

}

int main() {
	//function3_20_1();
	function3_30_2();	//run defeat,can show the messagebox windows. but can not keep running
	//printf("%d\n", Align(7, 5));
	//function3_20_4();


}
















//2、将所有节合并，保证程序正常运行.
//3、定义一个函数，能够返回对齐后的大小Align(int x, int y)
//4、编程输出全部目录项.