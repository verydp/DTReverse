#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>
#include <string.h>

//遍历导出表信息、根据名字获取函数RVA地址、根据序号获取函数RVA地址
//1、编写程序打印所有的导出表信息
// E:\Github\repo\Mylib\defDLL\x64\Debug
void function1() {

	char fpath[] = "E:\\Github\\repo\\MyDLL\\StudyDLL\\x64\\Debug\\StudyDLL3.dll";

	char* fbuffer;

	fbuffer = FileToBuffer(fpath);

	IMAGE_OPTIONAL_HEADER64* opeheader;
	opeheader = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	//printf("%x\n", opeheader->DataDirectory[0].VirtualAddress);
	_IMAGE_EXPORT_DIRECTORY* ExportDirectory;
	ExportDirectory = (_IMAGE_EXPORT_DIRECTORY*)(fbuffer + RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));

	printf("Characteristics = %x\n", ExportDirectory->Characteristics);
	printf("TimeDateStamp = %x\n", ExportDirectory->TimeDateStamp);
	printf("MajorVersion = %x\n", ExportDirectory->MajorVersion);
	printf("MinorVersion = %x\n", ExportDirectory->MinorVersion);
	printf("Name = %x\t\t\t//DLL库名\n", ExportDirectory->Name);
	printf("Base = %x\t\t\t//名字序号表的Base\n", ExportDirectory->Base);
	printf("NumberOfFunctions = %x\t\t//函数数量\n", ExportDirectory->NumberOfFunctions);				//函数数量
	printf("NumberOfNames = %x\t\t//名字数量\n", ExportDirectory->NumberOfNames);						//名字数量
	printf("AddressOfFunctions = %x\t//函数地址\n", ExportDirectory->AddressOfFunctions);			//函数地址
	printf("AddressOfNames = %x\t\t//名字地址\n", ExportDirectory->AddressOfNames);					//名字地址
	printf("AddressOfNameOrdinals = %x\t//名字序号地址\n", ExportDirectory->AddressOfNameOrdinals);	//名字序号地址

	//尝试打印
	printf("==================函数名字====================\n");
	//获取导出“名字”的数量
	int numberofnames;
	numberofnames = ExportDirectory->NumberOfNames;
	//printf("%x\n", numberofnames);

	//根据导出表的结构体，获取函数名指针表的RVA
	int NamesPtrArrayRVA;
	NamesPtrArrayRVA = ExportDirectory->AddressOfNames;
	//printf("NamesPtrArrayRVA = %x\n", NamesPtrArrayRVA);

	//根据函数名指针表的RVA，获取函数名指针表的FOA
	int NamesPtrArrayFOA;
	NamesPtrArrayFOA = RVA_TO_FOA(fbuffer, NamesPtrArrayRVA);
	//printf("NamesPtrArrayFOA = %x\n", NamesPtrArrayFOA);
	/********************此时已经获取到了函数名指针表**********************/

	char* ptrName;
	//根据函数名指针表的FOA，遍历获取函数名指针的RVA
	int NameRVA;
	int NameFOA;

	for (int i = 0; i < numberofnames; i++) {
		//每获取一个函数名的RVA地址，转换成FOA地址
		NameRVA = *((int*)(fbuffer + NamesPtrArrayFOA + i * 4));
		NameFOA = RVA_TO_FOA(fbuffer, NameRVA);
		ptrName = fbuffer + NameFOA;
		showstring(ptrName);
		//printf("%x\n", NameFOA);

	}

	/*
	* 函数名字指针表
	* 89 B9 01 00		dim			@5
	* 8D B9 01 00		func2		@7
	* 93 B9 01 00		func3		@9
	* 99 B9 01 00		plus		@2
	*/
	///////////////////上下对应//////////////////////
	/*
	* 函数名字
	* 64 69 6D 00			dim			@5
	* 66 75 6E 63 32 00		func2		@7
	* 66 75 6E 63 33 00		func3		@9
	* 70 6C 75 73 00		plus		@2
	*/

	printf("==================函数序号====================\n");
	//根据序号可以直接推导出函数地址的指针地址。从而获取函数的起始内存地址
	int NumberOfNames;
	int OrdinalsRVA;
	int OrdinalsFOA;
	short* ptrOrdinals;

	NumberOfNames = ExportDirectory->NumberOfNames;
	OrdinalsRVA = ExportDirectory->AddressOfNameOrdinals;
	OrdinalsFOA = RVA_TO_FOA(fbuffer, OrdinalsRVA);
	ptrOrdinals = (short*)(fbuffer + OrdinalsFOA);

	for (int i = 0; i < NumberOfNames; i++) {
		printf("%x\n", *(ptrOrdinals + i));
	}
	/*
	* 名字序号表
	* 03 00		dim		@5		//5 = 3 + 2
	* 05 00		func2	@7		//7 = 5 + 2
	* 07 00		func3	@9		//9 = 7 + 2
	* 00 00		plus	@2		//2 = 0 + 2
	*/

	printf("==================函数地址====================\n");
	int numberoffunctions;
	numberoffunctions = ExportDirectory->NumberOfFunctions;

	int* ptrFunctionAddr;
	int AddressOfFunctionsFOA;

	AddressOfFunctionsFOA = RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfFunctions);
	ptrFunctionAddr = (int*)(fbuffer + AddressOfFunctionsFOA);

	for (int i = 0; i < numberoffunctions; i++) {
		printf("%x\t%x\n", i + 1, *(ptrFunctionAddr + i));
	}

	/*实际DLL导出时的配置如下
	LIBRARY CDLL

	EXPORTS
	plus		@2
	dim			@5
	func1		@6	Noname
	func2		@7
	func3		@9
	*/

	/***********文件中函数地址的存放如下**********/
	/*
	* 此处是AddrOfFunctions内存真实数据
	* 07 13 01 00		@2		plus
	* 00 00 00 00		@3		//此处没有函数，补0
	* 00 00 00 00		@4		//此处没有函数，补0
	* 9F 11 01 00		@5		dim
	* 22 11 01 00		@6		func1
	* 28 10 01 00		@7		func2
	* 00 00 00 00  		@8		//此处没有函数，补0
	* 49 12 01 00		@9		func3
	*
	* 由此推出：ExportDirectory->NumberOfFunctions = 9 - 2 + 1 = 8
	*/
}

//2、GetFunctionAddrByName(FileBuffer指针，函数名),此处传出的是函数名的FOA
int GetFunctionAddrByName(char* fbuffer, char FunctionName[]) {

	IMAGE_OPTIONAL_HEADER64* opeheader;
	opeheader = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	//printf("%x\n", opeheader->DataDirectory[0].VirtualAddress);
	_IMAGE_EXPORT_DIRECTORY* ExportDirectory;
	ExportDirectory = (_IMAGE_EXPORT_DIRECTORY*)(fbuffer + RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));
	//printf("%x\n", RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));


	for (unsigned int i = 0; i < ExportDirectory->NumberOfNames; i++) {

		int NamePtrArrayRVA;
		NamePtrArrayRVA = ExportDirectory->AddressOfNames;

		int NamePtrArrayFOA;
		NamePtrArrayFOA = RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfNames);

		int* NamePtrArray;
		NamePtrArray = (int*)(fbuffer + NamePtrArrayFOA) + i;

		int NamePtrRVA;
		NamePtrRVA = *NamePtrArray;

		int NamePtrFOA;
		NamePtrFOA = RVA_TO_FOA(fbuffer, NamePtrRVA);

		char* NamePtr;
		NamePtr = fbuffer + NamePtrFOA;

		if (StringCMP(NamePtr, FunctionName)) {
			//根据名字的索引，获取名字序号
			int NameOrdinalRVA;
			NameOrdinalRVA = ExportDirectory->AddressOfNameOrdinals;

			int NameOrdinalFOA;
			NameOrdinalFOA = RVA_TO_FOA(fbuffer, NameOrdinalRVA);
			//根据名字序号表，获取函数序号指针
			short* ptrNameOrdinal;
			ptrNameOrdinal = (short*)(fbuffer + NameOrdinalFOA) + i;
			//根据函数序号指针，获取函数序号
			short Ordinal;
			Ordinal = *ptrNameOrdinal;
			//获取函数地址指针
			int FuncAddrPtrArrayRVA;
			FuncAddrPtrArrayRVA = ExportDirectory->AddressOfFunctions;

			int FuncAddrPtrArrayFOA;
			FuncAddrPtrArrayFOA = RVA_TO_FOA(fbuffer, FuncAddrPtrArrayRVA);

			int* ptrFuncAddrArray;
			ptrFuncAddrArray = (int*)(fbuffer + FuncAddrPtrArrayFOA) + Ordinal;

			printf("FuncAddrRVA = %x\nFcuntionFOA = %x\n", *ptrFuncAddrArray, RVA_TO_FOA(fbuffer, *ptrFuncAddrArray));

			//返回函数地址的RVA
			return *ptrFuncAddrArray;

		}

	}
	printf("without this function name");
	return 0;
}

//3、GetFunctionAddrByOrdinals(FileBuffer指针，函数名导出序号),返回值是函数的RVA
int GetFunctionAddrByOrdinals(char* fbuffer, int Ordinary) {
	int FunctionAddrRVA;

	IMAGE_OPTIONAL_HEADER64* opeheader;
	opeheader = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	//printf("%x\n", opeheader->DataDirectory[0].VirtualAddress);
	IMAGE_EXPORT_DIRECTORY* ExportDirectory;
	ExportDirectory = (IMAGE_EXPORT_DIRECTORY*)(fbuffer + RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));

	int FuncAddrPtrArrayRVA;
	FuncAddrPtrArrayRVA = ExportDirectory->AddressOfFunctions;

	int FuncAddrPtrArrayFOA;
	FuncAddrPtrArrayFOA = RVA_TO_FOA(fbuffer, FuncAddrPtrArrayRVA);



	int OrdinalBase;
	OrdinalBase = ExportDirectory->Base;

	Ordinary = Ordinary - OrdinalBase;

	int* ptrFuncAddrRVA;
	ptrFuncAddrRVA = (int*)(fbuffer + FuncAddrPtrArrayFOA) + Ordinary;



	printf("%x\n", *ptrFuncAddrRVA);

	//返回函数地址的RVA
	return *ptrFuncAddrRVA;

}


int main() {
	char fpath[] = "E:\\Github\\repo\\Demo\\StudyDLL\\x64\\Debug\\StudyDLL.dll";
	char* fbuffer;

	fbuffer = FileToBuffer(fpath);

	//function1();

	char FunctionName[] = "ptrCharacteristics";
	//char FunctionName1[] = "ptrCharacteristics";

	//GetFunctionAddrByName(fbuffer, FunctionName);
	//printf("%d\n", sizeof(FunctionName));

	//此处Demo的DLL文件中，序号为6的函数是Noname隐藏起来的，即名字序号表中没有信息、名字表中没有信息。
	GetFunctionAddrByOrdinals(fbuffer, 6);


}