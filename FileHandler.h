#ifndef FILE_HANDERLE_H
#define FILE_HANDERLE_H

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

//2、将内存运行中的文件重新写到某个文件中
void MemBufferToFile(char* Newbuffer, char fpath[]) {
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
		fwrite(Newbuffer + VirtualAddress, SizeOfRawData, 1, pfile);
	}
}

//2-1、将内存中一块数据写到某个文件中
void BufferToFile(char* fbuffer, char fpath[]) {
	FILE* pfile;
	pfile = fopen(fpath, "wb");

	//获取最后一节数据，然后确认文件的大小
	int iSize;
	int NumberOfSection;
	int SizeOfRawData;
	int PointerToRawData;
	NumberOfSection = *ptrNumberOfSection(fbuffer);
	SizeOfRawData = *ptrSizeOfRawData(ptrSection(fbuffer) + 40 * (NumberOfSection -1));
	PointerToRawData = *ptrPointerToRawData(ptrSection(fbuffer) + 40 * (NumberOfSection - 1));
	iSize = PointerToRawData + SizeOfRawData;
	
	//printf("%x\n", PointerToRawData);
	//向文件写入PE头
	fwrite(fbuffer,iSize , 1, pfile);


}

//3、RVA to FOA（将内存中的地址转换到文件中的地址）
int RVA_TO_FOA(char* buffer, __int64 rva) {
	int FOA;
	// 用内存地址 - ImageBase = 虚拟内存地址（RVA）
	__int64 virtualaddress;
	virtualaddress = rva;

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

int FOA_TO_RVA(char* buffer, __int64 foa) {
	int RVA;
	//节表起始地址
	char* pSection = ptrSection(buffer);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(buffer);

	int VirtualAddress;
	int SizeOfRawData;
	int PointerToRawData;
	//循环遍历节表，确认foa所在的节
	for (int i = 0; i < NumberOfSection; i++) {
		VirtualAddress = *ptrVirtualAddress(ptrSection(buffer) + 40 * i);
		SizeOfRawData = *ptrSizeOfRawData(ptrSection(buffer) + 40 * i);
		PointerToRawData = *ptrPointerToRawData(ptrSection(buffer) + 40 * i);

		if (PointerToRawData < foa && foa < PointerToRawData + SizeOfRawData) {
			//用RVA - VirtualAddress + PointerToData = FOA
			RVA = foa - PointerToRawData + VirtualAddress;
			//printf("%x", FOA);
			return RVA;
		}
	}
}
#endif

#pragma once
