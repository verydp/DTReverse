//下述代码主要是读取notepad.exe的节表信息
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

//C:\Windows\System32\notepad.exe
int main() {
	FILE* pfile;
	long lSize;
	char* buffer;

	pfile = fopen("C:\\Windows\\System32\\notepad.exe", "rb");

	fseek(pfile, 0, SEEK_END);
	lSize = ftell(pfile);

	rewind(pfile);

	buffer = (char*)malloc(lSize);

	if (buffer != NULL) {
		//将exe文件读取到buffer上
		fread(buffer, 1, lSize, pfile);
		
		//思路：根据DOS头的a_lfanew确定PE头，根据PE头的第9个WORD确定option PE的大小，节表就紧跟在option PE后面
		int a_lfanew;
		a_lfanew = *((int*)(buffer) + 15);						//获得PE头的具体偏移量
		printf("DOS -> a_lfanew(4) = 0x%x\t\t\/\/获得PE头偏移量\n", a_lfanew);

		char* PEHeader;
		PEHeader = a_lfanew + buffer;							//根据a_lfanew获得PE头的具体位置
		printf("PE -> Signature(4) = %c%c\t\t\/\/确认PE头位置\n", *(PEHeader), *(PEHeader+1));

		short* SizeOfOptionHeader;
		SizeOfOptionHeader = (short*)PEHeader + 10;				//根据PE头偏移10个“字”得到option PE的大小0xf0
		printf("PE -> SizeOfOptionHeader(2) = 0x%x\t\/\/确认option PE大小\n", *SizeOfOptionHeader);

		//准备阶段，在梳理节表信息，需要信息：几个section、文件对齐尺寸。。。
		short* NumberOfSections;
		NumberOfSections = (short*)(PEHeader + 6);
		printf("PE->NumberOfSections(2) = 0x%x\n", *NumberOfSections);



		
		
		printf("------------------------显示节表模块-----------------------\n");
		printf("节名\t原大小\t内存地址\t节块大小\t节位置\t节属性\n");
		char* psection;
		for (int i = 0; i < *NumberOfSections; i++) {

			psection = buffer + a_lfanew + 24 + *SizeOfOptionHeader + i*40;//文件头(buffer) + PE头的偏移量/a_lfanew(0x108) + PE Signature(4) + PE(20) + option PE(0xf0)

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

	}
	else {
		printf("malloc defeat~");
	}

	free(buffer);

	fclose(pfile);

	return 0;
}