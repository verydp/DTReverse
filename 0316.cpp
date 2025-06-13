#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PEpointer.h"

#pragma warning(disable : 4996)


//��дһ���������Զ����ļ�����һ���ڴ��У����ҷ��ظÿ��ڴ��ָ��
char* ReadFileToMem(char fpath[]) {
	FILE* pfile;
	long lSize;
	char* fbuffer;

	pfile = fopen(fpath, "rb");

	fseek(pfile, 0, SEEK_END);
	lSize = ftell(pfile);

	rewind(pfile);

	fbuffer = (char*)malloc(lSize);

	if (fbuffer != NULL) {
		fread(fbuffer, 1, lSize, pfile);
		fclose(pfile);
		return fbuffer;
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

	printf("ͷ��С = %x\t������ = %d\t�ڱ���ʼ��ַƫ���� = %x\tӳ���ַ = %x\n", SizeOfHeaders, NumberOfSection, sectionoffset, ImageBase);


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

//2�����ڴ��е��ļ�����д��ĳ���ļ���
void NbufferToRbuffer(char* Newbuffer, char fpath[]) {
	FILE* pfile;
	pfile = fopen(fpath, "wb");

	//��ȡ�ļ�ͷ�Ĵ�С
	int SizeOfHeaders;
	SizeOfHeaders = *ptrSizeOfHeaders(Newbuffer);
	//���ļ�д��PEͷ
	fwrite(Newbuffer, SizeOfHeaders, 1, pfile);

	//������
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(Newbuffer);
	//�ڱ���ʼ��ַ
	char* pSection = ptrSection(Newbuffer);

	int VirtualAddress;
	int SizeOfRawData;
	int PointerToRawData;
	//ѭ�������ڵ��ڴ��ַ��Ȼ��ʼ���ļ�д��
	for (int i = 0; i < NumberOfSection; i++) {
		VirtualAddress = *ptrVirtualAddress(ptrSection(Newbuffer) + 40 * i);
		SizeOfRawData = *ptrSizeOfRawData(ptrSection(Newbuffer) + 40 * i);
		PointerToRawData = *ptrPointerToRawData(ptrSection(Newbuffer) + 40 * i);

		fseek(pfile, PointerToRawData, SEEK_SET);
		fwrite(Newbuffer + VirtualAddress, SizeOfRawData,1, pfile);
	}
}


//3��RVA to FOA�����ڴ��еĵ�ַת�����ļ��еĵ�ַ��
//��Ҫ����һ��ָ���ַ�����Ƿ���һ����ַƫ�ƣ�

int RVA_TO_FOA(char* buffer,char* rva) {
	int FOA;
	// ���ڴ��ַ - ImageBase = �����ڴ��ַ��RVA��
	int virtualaddress;
	virtualaddress = rva - buffer;

	//�ڱ���ʼ��ַ
	char* pSection = ptrSection(buffer);

	//������
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(buffer);

	int VirtualAddress;
	int SizeOfRawData;
	int PointerToRawData;
	//ͨ�������ڱ���Ϣ����ȡÿһ���ڵ������ַ��Χ��VirtualAddress ~ VirtualAddress + SizeRawData��
	for (int i = 0; i < NumberOfSection; i++) {
		VirtualAddress = *ptrVirtualAddress(ptrSection(buffer) + 40 * i);
		SizeOfRawData = *ptrSizeOfRawData(ptrSection(buffer) + 40 * i);
		PointerToRawData = *ptrPointerToRawData(ptrSection(buffer) + 40 * i);
		//�ж�RVA�Ƿ��ڽڷ�Χ����
		if (VirtualAddress < virtualaddress && virtualaddress < VirtualAddress + SizeOfRawData) {
			//��RVA - VirtualAddress + PointerToData = FOA
			FOA = virtualaddress - VirtualAddress + PointerToRawData;
			//printf("%x", FOA);
			return FOA;
		}
	}
	printf("VirtualAddress Error");
	return NULL;
}


int main() {
	char* fbuffer;
	char* newbuffer;

	char fpath[] = "C:\\Windows\\System32\\notepad.exe";
	//1:1�����ļ����ڴ�
	fbuffer = ReadFileToMem(fpath);

	int SizeOfImage;
	SizeOfImage = *ptrSizeOfImage(fbuffer);
	printf("%x\n", SizeOfImage);

	//��SizeOfImage��������һ���ڴ�
	newbuffer = NewBuffer(SizeOfImage);
	//ģ��PE���صĹ��̣����ļ��������ص�һ���ڴ���
	FbufferToNbuffer(fbuffer, newbuffer);

	//�����ڴ����������ļ�������д�뵽������
	char newpath[] = "C:\\Users\\84519\\Desktop\\newnotepad.exe";
	NbufferToRbuffer(newbuffer, newpath);

	int foa;
	foa = RVA_TO_FOA(newbuffer, newbuffer + 0x37001);
	printf("\n%x\n", foa);

	free(newbuffer);
	free(fbuffer);
}

