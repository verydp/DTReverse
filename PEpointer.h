#ifndef PE_POINTER_H
#define PE_POINTER_H



/*
//PEpointer函数

int* ptre_lfanew(char* fbuffer) {}

char* ptrPE(char* fbuffer) {}

char* ptrOptionPE(char* fbuffer) {}

int* ptrSizeOfImage(char* fbuffer) {}

int* ptrSizeOfHeaders(char* fbuffer) {}

short* ptrSizeOfOptionalHeader(char* fbuffer) {}

char* ptrSection(char* fbuffer) {}

int SectionOffset(char* fbuffer) {}

int* ptrVirtualAddress(char* ptrSection) {}

int* ptrSizeOfRawData(char* ptrSection) {}

int* ptrPointerToRawData(char* ptrSection) {}

int* ptrCharacteristics(char* ptrSection) {}

short* ptrNumberOfSection(char* fbuffer) {}

int* ptrImageBase(char* fbuffer) {}
*/

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

#endif // !PE_POINTER
#pragma once
