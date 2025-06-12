#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)


//2�����ڴ��е��ļ����¼��ص��ļ���
//3����дһ���������Խ��ڴ��ַת�����ļ���ַ��RVAToFOA��

//��дһ���������Զ����ļ�����һ���ڴ��У����ҷ��ظÿ��ڴ��ָ��
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

//�������ڴ�ĺ���
char* NewBuffer(int SizeOfImage) {
	//���ݴ����SizeOfImageȷ����Ҫ�·�����ڴ�Ĵ�С
	char* newbuffer;
	newbuffer = (char*)malloc(SizeOfImage);

	if (newbuffer != NULL) {
		return newbuffer;
	}
	else {
		return NULL;
	}
}

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

//1�����ļ����ص��ڴ��У�Ĭ�ϲ����ǽ��ļ�1:1���Ƶ�fbuffer�ڴ��ָ�롣
char* FbufferToNbuffer(char* fbuffer, char* newbuffer) {
	//ͷ��С
	int SizeOfHeaders;
	SizeOfHeaders = *ptrSizeOfHeaders(fbuffer);

	//������
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(fbuffer);

	//�ڱ���ʼ��ַƫ��
	int sectionoffset;
	sectionoffset = SectionOffset(fbuffer);

	//ӳ���ַ
	int ImageBase;
	ImageBase = *ptrImageBase(fbuffer);

	printf("ͷ��С = %x\t������ = %d\t�ڱ���ʼ��ַƫ���� = %x\tӳ���ַ = %x", SizeOfHeaders, NumberOfSection, sectionoffset, ImageBase);


	if (newbuffer != NULL && fbuffer != NULL) {
		//���Ƚ�ͷ+�ڱ��Ƶ�newbuffer��
		memcpy(newbuffer, fbuffer, SizeOfHeaders);

		//����ѭ�����Ƶ�newbuffer��

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
	//1:1�����ļ����ڴ�
	fbuffer = *ReadFileToMem(fpath);

	int SizeOfImage;
	SizeOfImage = *ptrSizeOfImage(fbuffer);
	printf("%x\n", SizeOfImage);
	//��SizeOfImage��������һ���ڴ�
	newbuffer = NewBuffer(SizeOfImage);

	FbufferToNbuffer(fbuffer, newbuffer);



	free(newbuffer);
	free(fbuffer);
}

