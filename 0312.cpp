#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

int main() {
	FILE* pfile;
	long lSize;
	char* buffer;

	pfile = fopen("F:\\chap01\\1.3 Windows操作系统\\1.3.1  Win32 API函数\\MessageBox\\MessageBoxA\\MessageBoxA.exe", "rb");

	fseek(pfile, 0, SEEK_END);
	lSize = ftell(pfile);
	rewind(pfile);

	//printf("%d\n", lSize);

	buffer = (char*)malloc(lSize);

	if (buffer != NULL) {
		fread(buffer, 1, lSize, pfile);
		/********************此处开始尝试解析32位PE文件结构************************/
		//创建三个容器指针，1字节、2字节、4字节。
		char* w1;
		short* w2;
		int* w4;

		//DOS头的两个重要字段WORD e_magic(0byte)(short)、LONG e_lfanew(36byte)int ;
		printf("-------------DOS头---------------\n");
		printf("e_magic(2)\t\t%c%c\n", *(buffer), *(buffer + 1));

		w4 = (int*)buffer;
		printf("e_lfanew(4)\t\t%x\n", *(w4 + 15));

		//根据获取到的d0，可以知道NT头开始的地方是0xd0，下面继续解析nt头。
		char* nt;
		nt = buffer + *(w4 + 15);	//nt头起始位置，可以从该表达式获取(buffer为文件起始地址)
		printf("-------------NT头----------------\n");
		printf("Signature(2)\t\t%c%c\n", *(nt), *(nt + 1));
		printf("\n");
		//获取到NT头后可以开始解析标准PE头。
		char* pe;
		pe = nt + 4;
		w2 = (short*)pe;
		printf("-------------PE头----------------\n");
		printf("Machine(2)\t\t%x\t\\\\运行在何种CPU上；0就是任意；014C是interl386及以后；8664则是x64也就是64位系统上；\n", *(w2));
		printf("NumberOfSections(2)\t%x\t\\\\保存节的数量。.data、.text\n", *(w2 + 1));
		printf("SizeOfOptionalHeader(2)\t%x\t\\\\可保存扩展PE头的大小，32位默认是0xE0(224字节)，64位默认是0xF0(240字节)，大小可以自定义\n", *(w2 + 8));
		printf("\n");
		//PE头是固定的20字节，接下来就可以解析扩展PE头的文件。
		char* ope;
		ope = nt + 24;				//optional PE开始的位置(即NT头偏移24字节)
		w2 = (short*)ope;
		w4 = (int*)ope;
		printf("----------Optional PE头----------\n");
		printf("Magic(2)\t\t%x\t\\\\0x10B则是32位程序，0x20B则是64位程序\n", *w2);
		printf("AddressOfEntryPoint(4)\t%x\t\\\\程序的入口\n", *(w4 + 4));
		printf("BaseOfCode(4)\t\t%x\t\\\\代码开始的基址\n", *(w4 + 5));
		printf("BaseOfData(4)\t\t%x\t\\\\数据开始的基址\n", *(w4 + 6));
		printf("ImageBase(4)\t\t%x\t\\\\内存映像基址\n", *(w4 + 7));
		printf("SectionAlignment(4)\t%x\t\\\\内存对齐，PE文件在内存上运行时，每个节数据的大小就是其整数倍的大小\n", *(w4 + 8));
		printf("FileAlignment(4)\t%x\t\\\\文件对齐，PE文件在磁盘上时，每个节数据的大小就是其整数倍的大小\n", *(w4 + 9));
		printf("SizeOfImage(4)\t\t%x\t\\\\内存中PE文件的大小，可以比硬盘上的文件大，但必须是SectionAlignment值的整数倍\n", *(w4 + 14));
		printf("SizeOfHeaders(4)\t%x\t\\\\所有头+节表按照FileAlignment对齐后的大小，否则加载会出错。\n", *(w4 + 15));
		printf("SizeOfStackReserve(4)\t%x\t\\\\初始化时保留栈的大小\n", *(w4 + 18));
		printf("SizeOfStackCommit(4)\t%x\t\\\\初始化时实际提交的栈大小\n", *(w4 + 19));
		printf("SizeOfHeapReserve(4)\t%x\t\\\\初始化时保留的堆大小\n", *(w4 + 20));
		printf("SizeOfHeapCommit(4)\t%x\t\\\\初始化时实际提交的堆大小\n", *(w4 + 21));





	}
	else {
		printf("malloc defeat~");
	}


	free(buffer);

	fclose(pfile);
	return 0;
}