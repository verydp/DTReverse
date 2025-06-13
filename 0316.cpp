#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PEpointer.h"

#pragma warning(disable : 4996)


//编写一个函数能自动将文件读到一块内存中，并且返回该块内存的指针
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

	printf("头大小 = %x\t节数量 = %d\t节表起始地址偏移量 = %x\t映像基址 = %x\n", SizeOfHeaders, NumberOfSection, sectionoffset, ImageBase);


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

//2、将内存中的文件重新写到某个文件中
void NbufferToRbuffer(char* Newbuffer, char fpath[]) {
	FILE* pfile;
	pfile = fopen(fpath, "wb");

	//获取文件头的大小
	int SizeOfHeaders;
	SizeOfHeaders = *ptrSizeOfHeaders(Newbuffer);
	//向文件写入PE头
	fwrite(Newbuffer, SizeOfHeaders, 1, pfile);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(Newbuffer);
	//节表起始地址
	char* pSection = ptrSection(Newbuffer);

	int VirtualAddress;
	int SizeOfRawData;
	int PointerToRawData;
	//循环遍历节的内存地址，然后开始向文件写入
	for (int i = 0; i < NumberOfSection; i++) {
		VirtualAddress = *ptrVirtualAddress(ptrSection(Newbuffer) + 40 * i);
		SizeOfRawData = *ptrSizeOfRawData(ptrSection(Newbuffer) + 40 * i);
		PointerToRawData = *ptrPointerToRawData(ptrSection(Newbuffer) + 40 * i);

		fseek(pfile, PointerToRawData, SEEK_SET);
		fwrite(Newbuffer + VirtualAddress, SizeOfRawData,1, pfile);
	}
}


//3、RVA to FOA（将内存中的地址转换到文件中的地址）
//是要返回一个指针地址？还是返回一个地址偏移？

int RVA_TO_FOA(char* buffer,char* rva) {
	int FOA;
	// 用内存地址 - ImageBase = 虚拟内存地址（RVA）
	int virtualaddress;
	virtualaddress = rva - buffer;

	//节表起始地址
	char* pSection = ptrSection(buffer);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(buffer);

	int VirtualAddress;
	int SizeOfRawData;
	int PointerToRawData;
	//通过遍历节表信息，获取每一个节的虚拟地址范围（VirtualAddress ~ VirtualAddress + SizeRawData）
	for (int i = 0; i < NumberOfSection; i++) {
		VirtualAddress = *ptrVirtualAddress(ptrSection(buffer) + 40 * i);
		SizeOfRawData = *ptrSizeOfRawData(ptrSection(buffer) + 40 * i);
		PointerToRawData = *ptrPointerToRawData(ptrSection(buffer) + 40 * i);
		//判断RVA是否在节范围里面
		if (VirtualAddress < virtualaddress && virtualaddress < VirtualAddress + SizeOfRawData) {
			//用RVA - VirtualAddress + PointerToData = FOA
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
	//1:1拷贝文件到内存
	fbuffer = ReadFileToMem(fpath);

	int SizeOfImage;
	SizeOfImage = *ptrSizeOfImage(fbuffer);
	printf("%x\n", SizeOfImage);

	//按SizeOfImage重新申请一块内存
	newbuffer = NewBuffer(SizeOfImage);
	//模仿PE加载的过程，将文件拉伸后加载到一块内存中
	FbufferToNbuffer(fbuffer, newbuffer);

	//将在内存中拉伸后的文件，重新写入到磁盘中
	char newpath[] = "C:\\Users\\84519\\Desktop\\newnotepad.exe";
	NbufferToRbuffer(newbuffer, newpath);

	int foa;
	foa = RVA_TO_FOA(newbuffer, newbuffer + 0x37001);
	printf("\n%x\n", foa);

	free(newbuffer);
	free(fbuffer);
}

