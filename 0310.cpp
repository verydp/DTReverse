#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable : 4996)

/*
int fseek(FILE *stream, long int offset, int whence)
	stream -- 这是指向 FILE 对象的指针，该 FILE 对象标识了流。
	offset -- 这是相对 whence 的偏移量，以字节为单位。
	whence -- 这是表示开始添加偏移 offset 的位置。它一般指定为下列常量之一：
		SEEK_SET	文件的开头
		SEEK_CUR	文件指针的当前位置
		SEEK_END	文件的末尾
*/

int main() {

	//创建一个文件对象
	FILE* pfile;
	FILE* retfile;
	//声明一个用来存放文件大小的变量
	long lSize;
	//声明一个指针，用来指向内存中存放文件的内存起始地址
	char* buffer;
	//声明一个变量，用来判断读取到内存的空间大小，是否和文件本身大小一致。
	size_t result;

	//以二进制的方式打开一个文件。
	pfile = fopen("C:\\Program Files\\WindowsApps\\Microsoft.WindowsNotepad_11.2503.16.0_x64__8wekyb3d8bbwe\\Notepad\\Notepad.exe", "rb");

	//通过fseek()将文件指针位置设置到END，通过ftell()获取该文件的大小
	fseek(pfile,0,SEEK_END);
	lSize = ftell(pfile);

	printf("%d\n", lSize);

	//C 库函数 void rewind(FILE * stream) 设置文件位置为给定流 pfile 文件的开头。
	rewind(pfile);

	//向操作系统申请一块内存空间。
	buffer = (char*)malloc(sizeof(char)*lSize);

	//通过fread()函数，将文件拷贝到buffer中。
	if (buffer != NULL) {
		fread(buffer, 1, lSize, pfile);
	}
	else {
		printf("Can\'t copy the file to buffer~");
	}
	//从程序当中的某个内存块中向外写到某个文件中。
	retfile = fopen("C:\\Users\\84519\\Desktop\\notepad.exe", "w+");
	if (buffer != NULL) {
		fwrite(buffer, 1, lSize, retfile);
	}
	else {
		printf("Can\'t get buffer~");
	}
	

	fclose(pfile);
	free(buffer);
	return 0;
}


