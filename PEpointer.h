#ifndef PE_POINTER_H
#define PE_POINTER_H



/*
//PEpointer����

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

//PEpointer����

int* ptre_lfanew(char* fbuffer) {
	//�Ȼ�ȡ�ڱ���ʼ��ַ����e_lfanew + 24 + SizeOfOptionalHeader
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
	//���Ȼ�ȡimage���ڴ��еĴ�С������չPEͷ����ƫ��56���ֽڵ�SizeOfImage(4byte)
	pSizeOfImage = (int*)(ptrOptionPE(fbuffer) + 56);

	//printf("function in ptrSizeOfImage()%x   %x\n", *((int*)( ptrOptionPE(fbuffer) + 56 )),1);
	//printf("function in ptrSizeOfImage()%x   %x\n", *pSizeOfImage,1);

	return pSizeOfImage;
}

int* ptrSizeOfHeaders(char* fbuffer) {
	//�ȿ���ͷ�ļ�+�ڱ�����ȡ��չPEͷƫ��60���ֽڵ�SizeOfHeaders(4 byte)�����ֵ
	int* pSizeOfHeaders;
	pSizeOfHeaders = (int*)(ptrOptionPE(fbuffer) + 60);
	//int SizeOfHeaders;
	//SizeOfHeaders = *pSizeOfHeaders;
	//printf("SizeOfHeaders = %x\n", SizeOfHeaders);

	return pSizeOfHeaders;
}

short* ptrSizeOfOptionalHeader(char* fbuffer) {
	//PE�����ʼ��ַ����e_lfanew
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

	//����������ʽ��ýڱ����ʼ��ַ
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
	//����PEͷ��ȡ�ڵ���������PEͷƫ��6���ֽڵ�NumberOfSection(4)
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
