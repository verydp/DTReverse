#include <Windows.h>
#include <stdio.h>
#include "PEpointer.h"
#include "FileHandler.h"




// 此处拿D盘的7z来做实验，下面尝试整理一下关于shellcode注入的相关内容
// 首先通过自己编写一个函数





// 进入反汇编模式，查看该MessageBoxA()函数的内存地址(貌似重启以后也是0x7ffa2ae22e70这个地址)
// 整理可得，我需要将如下部分的硬编码放到7z.exe文件中。
/************************************************************************************************/
// push rax						50
// mov rax,0x7ffa2ae22e70		48 b8 70 2e e2 2a fa 7f 00 00（rax需要16个字节，即4字）
//////////////////////////////////////////////////
// xor r9d, r9d					45 33 C9		//该部分为调用messageBoxA函数的部分
// xor r8d, r8d					45 33 C0		//
// xor edx, edx					33 D2			//
// xor ecx, ecx					33 C9			//
// call rax						ff d0			//
//////////////////////////////////////////////////
// pop rax						58
// jmp 偏移量					E9 偏移量
/************************************************************************************************/

/**************************思路******************************/
// 1、将上述硬编码写入到文件的某个代码段.text，并记录下shellcode的起始地址
//////////////////////////首先找到.text的节表信息////////////////////////////////
// 00000200  2E 74 65 78 74 00 00 00  96 F3 05 00 00 10 00 00.text...........
// 00000210  00 F4 05 00 00 04 00 00  00 00 00 00 00 00 00 00  ................
// 00000220  00 00 00 00 20 00 00 60      
//////////////////////////可以得出.text的代码段的///////////////////////////////
// virtualaddress = 0x1000		该节在内存中的0x1000开始
// SizeRawData = 0x5f400		说明该节在内存中的尺寸是0x5f400
// PointerToRawData = 0x400		说明该节在文件中是以0x400开始
//////////////////////////找到.text段的空余部分//////////////////////////////////
// Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
// 0005F780  FF 25 72 0D 00 00 CC CC  48 8D 0D 79 3A 02 00 48  . % r.....H..y:..H
// 0005F790  FF 25 62 0D 00 00 00 00  00 00 00 00 00 00 00 00  . % b.............
// 0005F7A0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
// 0005F7B0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
// 0005F7C0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
// 0005F7D0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
// 0005F7E0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
// 0005F7F0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
// 0005F800  B8 1A 08 00 00 00 00 00  2A 1B 08 00 00 00 00 00  ........*.......
// 可以发现从0x5f7a0行--0x5f7f0行都是空白区，所以选择0x5f7e0为shellcode起始地址
// 0005F7E0  50 48 B8 70 2E E2 2A FA  7F 00 00 45 33 C9 45 33  PH.p..*....E3.E3
// 0005F7F0  C0 33 D2 33 C9 FF D0 58  E9 E3 F0 FF FF 00 00 00  .3.3...X........
// PS：可以根据shellcode起始地址的foa(0x5f7e0)计算出rva。即要将OEP修改成这个rva(0x5f7e0 - 0x400 + 0x1000 = 0x603e0)


// 2、从PE头的扩展PE头中，获取原始程序的OEP（内存中的偏移），并计算jmp指令需要的偏移量（文件中的相对偏移）
// 首先从扩展PE头中获取程序的原始OEP（rva）= 0x5f4e0。
// Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
// 00000110  0B 02 08 00 00 F4 05 00  00 DE 02 00 00 00 00 00  ................
// 00000120  E0 F4 05 00 00 10 00 00  00 00 40 00 00 00 00 00  ..........@.....
// 计算出原始OEP的foa = 0x5f4e0 - 0x1000 + 0x400 = 0x5E8E0（原始程序入口的foa）
//	偏移量	= 将要跳转的地址 - （jmp指令地址 + 5）
//			= 原始程序入口的foa - （E9指令地址 + 5）
//			= 0x5E8E0 - （0x5f7f9 + 5）
//			= 0xFFFFF0E3
// 
// 3、再将PE头的程序入口地址，修改为shellcode的起始地址（注意：此时需要写入的是shellcode在内存中的地址）
// 可以根据shellcode起始地址的foa(0x5f7e0)计算出rva。即要将OEP修改成这个rva(0x5f7e0 - 0x400 + 0x1000 = 0x603e0)
// Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
// 00000110  0B 02 08 00 00 F4 05 00  00 DE 02 00 00 00 00 00  ................
// 00000120  E0 03 06 00 00 10 00 00  00 00 40 00 00 00 00 00  ..........@.....

/*
*	 文件中				内存中
|->* 0x5E8E0			0x5f4e0(原入口)
|  * ···
|  * ···
|  * ···
|  * 0x5f7e0			0x603e0(新入口)	push rax						50
|  *									mov rax,0x7ffa2ae22e70			48 b8 70 2e e2 2a fa 7f 00 00（rax需要16个字节，即4字）
|  *									xor r9d, r9d					45 33 C9
|  *									xor r8d, r8d					45 33 C0
|  *									xor edx, edx					33 D2
|  *									xor ecx, ecx					33 C9
|  *									call rax						ff d0
|  *									pop rax							58
|<-* 0x5f7f8			0x603F8			jmp 偏移量						e9 e3 f0 ff ff
   * 0x5f7fd			0x603fd
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//尝试用代码添加一段shellcode到某个文件中
char shellcode[29] = {
	0x50, 0x48, 0xB8, 0x70, 0x2E, 0xE2, 0x2A, 0xFA, 0x7F, 0x00, 0x00, 0x45, 0x33, 0xC9, 0x45, 0x33,
	0xC0, 0x33, 0xD2 ,0x33, 0xC9, 0xFF, 0xD0, 0x58, 0xE9, 0x00, 0x00, 0x00, 0x00
};
//写入一段shellcode
//读出内存.text段的空闲区域，并确认shellcode是否超长。如果不超长，写入shellcode并返回shellcode的foa
int ShellcodeInject(char* fbuffer, char shellcode[]) {
	char* psection;
	psection = ptrSection(fbuffer);
	char* shellcodeFOA;

	//printf("%x\n", *ptrPointerToRawData(psection));

	if (*ptrSizeOfRawData(psection) - *ptrPhysicalAddress(psection) > 29) {
		char* shellcodeBegin;
		shellcodeBegin = fbuffer + *ptrPointerToRawData(psection) + *ptrPhysicalAddress(psection);
		memcpy(shellcodeBegin, shellcode, 29);
		printf("foa is %x\n", *ptrPointerToRawData(psection) + *ptrPhysicalAddress(psection));
		//此时就得到了shellcode起始地址的文件偏移
		shellcodeFOA = fbuffer + *ptrPointerToRawData(psection) + *ptrPhysicalAddress(psection);
		
	}
	else {
		printf("there's no space can inject shellcode");
		return NULL;
	}
	//计算shellcode的E9跳转地址和程序指针的偏移量，并修改shellcode结束后的跳转地址。
	//1、获取程序的入口地址
	int* pAddressOfEntryPoint;
	pAddressOfEntryPoint = ptrAddressOfEntryPoint(fbuffer);

	char* sAddressOfEntryPointFOA;
	sAddressOfEntryPointFOA = fbuffer + RVA_TO_FOA(fbuffer, *pAddressOfEntryPoint);
	printf("AddressOfEntryPointFOA = %x\n", RVA_TO_FOA(fbuffer, *pAddressOfEntryPoint));

	//2、计算shellcode跳转地址和程序入口的偏移量0x5E8E0 5F7B2  FFFF F12E
	int shellcodeJMPFOA;
	shellcodeJMPFOA = sAddressOfEntryPointFOA - (shellcodeFOA + 24 + 5);
	//printf("%x\n", *(int*)AddressOfEntryPointFOA);
	*((int*)(shellcodeFOA + 25)) = shellcodeJMPFOA;
	printf("test%x\n", *(shellcodeFOA + 25));

	//2、修改程序的入口地址
	//将shellcodeFOA转换成RVA
	*pAddressOfEntryPoint = FOA_TO_RVA(fbuffer, shellcodeFOA - fbuffer);
	//printf("%x\n", *pAddressOfEntryPoint);

	return 1;
}





int main() {
	
	//MessageBoxA(0, 0, 0, 0);固定地址为0x7ffa2ae22e70
	//long MessageBoxA = 0x7ffa2ae22e70;

	char fpath[] = "D:\\7-Zip\\7z.exe";
	char* fbuffer;
	fbuffer = ReadFileToMem(fpath);

	char retpath[] = "C:\\Users\\84519\\Desktop\\7zshellcode.exe";
	//获取shellcode的FOA
	if (ShellcodeInject(fbuffer, shellcode)) {
		BufferToFile(fbuffer, retpath);
	}
	else {
		printf("shellcode inject defeat~");
	}
	
	//MessageBoxA(0, 0, 0, 0);




	return 0;
}