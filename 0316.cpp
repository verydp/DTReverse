#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)


//2、将内存中的文件重新加载到文件中
//3、编写一个函数可以将内存地址转换到文件地址（RVAToFOA）

//编写一个函数能自动将文件读到一块内存中，并且返回该块内存的指针
char** ReadFileToMem(char fpath[]) {
	FILE* pfile;
	long lSize;
	char* fbuffer;
	char** ret;

	pfile = fopen(fpath, "rb");

	fseek(pfile, 0, SEEK_END);
	lSize = ftell(pfile);

	rewind(pfile);

	fbuffer = (char*)malloc(lSize);

	if (fbuffer != NULL) {
		fread(fbuffer, 1, lSize, pfile);
		ret = &fbuffer;
		fclose(pfile);
		return ret;
	}
	else {
		printf("Malloc defeat~");
		fclose(pfile);
		return NULL;
	}
}

//新申请内存的函数
char* NewBuffer(int SizeOfImage) {
	//根据传入的SizeOfImage确认需要新分配的内存的大小
	char* newbuffer;
	newbuffer = (char*)malloc(SizeOfImage);

	if (newbuffer != NULL) {
		return newbuffer;
	}
	else {
		return NULL;
	}
}

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

	printf("%x\n", e_lfanew);
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

int* ptrVirtualAddress(char* ptrSection){
	int* pVirtualAddress;
	pVirtualAddress = (int*)(ptrSection + 12);

	return pVirtualAddress;
}

int* ptrSizeOfRawData(char* ptrSection){
	int* pSizeOfRawData;
	pSizeOfRawData = (int*)(ptrSection + 16);

	return pSizeOfRawData;
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

//1、将文件加载到内存中，默认参数是将文件1:1复制到fbuffer内存的指针。
char* FbufferToNbuffer(char* fbuffer, char* newbuffer) {
	//头大小
	int SizeOfHeaders;
	SizeOfHeaders = *ptrSizeOfHeaders(fbuffer);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(fbuffer);

	//节表起始地址偏移
	int sectionoffset;
	sectionoffset = SectionOffset(fbuffer);

	//映像基址
	int ImageBase;
	ImageBase = *ptrImageBase(fbuffer);

	printf("头大小 = %x\t节数量 = %d\t节表起始地址偏移量 = %x\t映像基址 = %x", SizeOfHeaders, NumberOfSection, sectionoffset, ImageBase);


	if (newbuffer != NULL && fbuffer != NULL) {
		//首先将头+节表复制到newbuffer中
		memcpy(newbuffer, fbuffer, SizeOfHeaders);

		//将节循环复制到newbuffer中

		int VirtualAddress;
		int SizeOfRawData;
		int PointerToRawData;

		for (int i = 0; i < NumberOfSection; i++) {
			//pSection + 40 * i;
			VirtualAddress = *ptrVirtualAddress(ptrSection(fbuffer) + 40 * i);
			SizeOfRawData = *ptrSizeOfRawData(ptrSection(fbuffer) + 40 * i);
			PointerToRawData = *ptrPointerToRawData(ptrSection(fbuffer) + 40 * i);

			memcpy(newbuffer + VirtualAddress, fbuffer + PointerToRawData, SizeOfRawData);


		}

	}
	else {
		printf("newbuffer defeat~");
		return NULL;
	}



	return newbuffer;
}

int main() {
	char* fbuffer;
	char* newbuffer;

	char fpath[] = "C:\\Windows\\System32\\notepad.exe";
	//1:1拷贝文件到内存
	fbuffer = *ReadFileToMem(fpath);

	int SizeOfImage;
	SizeOfImage = *ptrSizeOfImage(fbuffer);
	printf("%x\n", SizeOfImage);
	//按SizeOfImage重新申请一块内存
	newbuffer = NewBuffer(SizeOfImage);

	FbufferToNbuffer(fbuffer, newbuffer);



	free(newbuffer);
	free(fbuffer);
}

