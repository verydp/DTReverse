#include "defDLL.h"
#include <Windows.h>
#include <stdio.h>

//打印所有的重定位表信息

void DisplayRelocatedData(char* fbuffer) {
	IMAGE_OPTIONAL_HEADER64* OPE;
	OPE = (IMAGE_OPTIONAL_HEADER64*)(ptrOptionPE(fbuffer));

	int RelocatedRVA;
	RelocatedRVA = OPE->DataDirectory[5].VirtualAddress;
	
	int RelocatedFOA;
	RelocatedFOA = RVA_TO_FOA(fbuffer, RelocatedRVA);

	char* ptrRelocated;
	ptrRelocated = fbuffer + RelocatedFOA;

	_IMAGE_BASE_RELOCATION* Relocation;
	Relocation = (_IMAGE_BASE_RELOCATION*)ptrRelocated;

	//printf("Relocation->VirtualAddress = %x\n", Relocation->VirtualAddress);
	//printf("Relocation->SizeOfBlock = %x\n", Relocation->SizeOfBlock);
	short* ptrRawAddr;
	

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
				RawFOA = RVA_TO_FOA(fbuffer, RawRVA);

				printf("RVA = %x (FOA = %x) need to be relocated\n", RawRVA, RawFOA);
			}
			//printf("%x\n", Raw >>12);
		}

		ptrRelocated = ptrRelocated + Relocation->SizeOfBlock;
		Relocation = (_IMAGE_BASE_RELOCATION*)ptrRelocated;
	}
	
}

int main() {
	char fpath[] = "E:\\Github\\repo\\Demo\\StudyDLL\\x64\\Debug\\StudyDLL3.dll";
	char* fbuffer;
	fbuffer = FileToBuffer(fpath);

	printf("%x\n", *ptrImageBase(fbuffer));

	DisplayRelocatedData(fbuffer);

}