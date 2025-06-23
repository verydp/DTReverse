#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

//1、编写程序打印所有的导出表信息
// E:\Github\repo\Mylib\defDLL\x64\Debug
void function1() {
	char fpath[] = "E:\\Github\\repo\\Mylib\\defDLL\\x64\\Debug\\defDLL.dll";
	char* fbuffer;

	fbuffer = FileToBuffer(fpath);

	IMAGE_OPTIONAL_HEADER64* opeheader;
	opeheader = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	//printf("%x\n", opeheader->DataDirectory[0].VirtualAddress);
	_IMAGE_EXPORT_DIRECTORY* ExportDirectory;
	ExportDirectory = (_IMAGE_EXPORT_DIRECTORY*)(fbuffer + RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));
	//printf("%x\n", RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));
	
	printf("Characteristics = %x\n", ExportDirectory->Characteristics);
	printf("TimeDateStamp = %x\n", ExportDirectory->TimeDateStamp);
	printf("MajorVersion = %x\n", ExportDirectory->MajorVersion);
	printf("MinorVersion = %x\n", ExportDirectory->MinorVersion);
	printf("Name = %x\n", ExportDirectory->Name);
	printf("Base = %x\n", ExportDirectory->Base);
	printf("NumberOfFunctions = %x\t//函数数量\n", ExportDirectory->NumberOfFunctions);				//函数数量
	printf("NumberOfNames = %x\t//名字数量\n", ExportDirectory->NumberOfNames);						//名字数量
	printf("AddressOfFunctions = %x\t//函数地址\n", ExportDirectory->AddressOfFunctions);			//函数地址
	printf("AddressOfNames = %x\t//名字地址\n", ExportDirectory->AddressOfNames);					//名字地址
	printf("AddressOfNameOrdinals = %x\t//名字序号地址\n", ExportDirectory->AddressOfNameOrdinals);	//名字序号地址
	

	//尝试打印
	printf("==================函数地址====================\n");
	for (unsigned int i = 0; i < ExportDirectory->NumberOfFunctions; i++) {
		printf("%x\t%x\n",i+1 ,*( (int*)(fbuffer + RVA_TO_FOA(fbuffer,ExportDirectory->AddressOfFunctions + i * 4) ) ));
	}

	printf("==================函数名字====================\n");
	for (unsigned int i = 0; i < ExportDirectory->NumberOfNames; i++) {
		int* addressnameRVA;
		//此处代表函数名的指针起始地址，需要将AddressOfNames的虚拟地址转换成文件地址
		addressnameRVA = (int*)(fbuffer + RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfNames));
		//printf("%x\n", RVA_TO_FOA(fbuffer, *(addressnameRVA + i)));
		//获取到每个函数名的指针，需要根据函数名指针地址(内存地址)获取函数名地址(文件地址)
		for (int x = 0; *(fbuffer + RVA_TO_FOA(fbuffer, *(addressnameRVA + i)) + x) != 0; x++) {
			printf("%c", *(fbuffer + RVA_TO_FOA(fbuffer, *(addressnameRVA + i)) + x) );
		}
		printf("\n");
	}
	printf("==================函数序号====================\n");
	//根据序号可以直接推导出函数地址的指针地址。从而获取函数的起始内存地址
	int Ordinals;
	for (int i = 0; i < ExportDirectory->NumberOfFunctions; i++) {
		Ordinals = ExportDirectory->Base + *(fbuffer + RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfNameOrdinals + 2 * i));
		printf("%x\n", Ordinals);
	}
	
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
		int* paddressnameRVA;
		char* addressnameFOA;
		//此处代表函数名的指针起始地址，需要将AddressOfNames的虚拟地址转换成文件地址
		paddressnameRVA = (int*)(fbuffer + RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfNames));
		//printf("%x\n", RVA_TO_FOA(fbuffer, *(addressnameRVA + i)));
		//获取到每个函数名的指针，需要根据函数名指针地址(内存地址)获取函数名地址(文件地址)

		//printf("%x\n", *paddressnameRVA);
		//获取函数名的FOA
		addressnameFOA = (char*)(fbuffer + RVA_TO_FOA(fbuffer, *(paddressnameRVA + i)));
		//printf("%x\n", RVA_TO_FOA(fbuffer, *(addressnameRVA + i)));
		//NameContain = ()*addressnameFOA

		int x;
		x = 0;
		//开始判断函数名和传入参数是否相同。
		while (x <= sizeof(FunctionName)) {
			if (x == sizeof(FunctionName)) {
				//返回函数名的FOA
				return RVA_TO_FOA(fbuffer, *(paddressnameRVA + i));
				//返回函数名从RVA
				return *(paddressnameRVA + i);
			}
			else {
				//如果函数名的第x个字节和传入参数FunctionName的第x字节相同
				if (FunctionName[x] == *(addressnameFOA + x)) {
					x++;
				}
				//如果不同，则直接推出while判断
				else 
				{
					break;
				}
				
			}
		}
	}
	printf("without this function name");
	return NULL;
}

//3、GetFunctionAddrByOrdinals(FileBuffer指针，函数名导出序号),返回值是函数的RVA
int GetFunctionAddrByOrdinals(char* fbuffer, int Ordinary) {
	int FunctionAddrRVA;

	IMAGE_OPTIONAL_HEADER64* opeheader;
	opeheader = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	//printf("%x\n", opeheader->DataDirectory[0].VirtualAddress);
	_IMAGE_EXPORT_DIRECTORY* ExportDirectory;
	ExportDirectory = (_IMAGE_EXPORT_DIRECTORY*)(fbuffer + RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));
	//printf("%x\n", RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));

	//1、首先获取导出表的ordinaryBase
	int OrdinaryBase;
	OrdinaryBase = ExportDirectory->Base;
	printf("%d\n", OrdinaryBase);
	//2、将传入参数的Ordinary - ordinaryBase = 函数地址表的偏移个数
	int offset;
	offset = Ordinary - OrdinaryBase;
	//3、取出函数地址表的具体RVA地址
	int pAddrOfFunctionsRVA;
	pAddrOfFunctionsRVA = ExportDirectory->AddressOfFunctions;
	//4、将函数地址表的RVA地址转换成FOA地址。
	int* pAddrOfFunctionsFOA;
	pAddrOfFunctionsFOA = (int*)(fbuffer + RVA_TO_FOA(fbuffer, pAddrOfFunctionsRVA));
	//5、根据函数地址表的FOA取出函数的RVA地址
	int pFunctionRVA = *pAddrOfFunctionsFOA;
	//6、根据函数的RVA地址，得出函数的FOA地址。
	//RVA_TO_FOA(fbuffer, pFunctionRVA);

	FunctionAddrRVA = pFunctionRVA;

	printf("%x\n", FunctionAddrRVA);
	return FunctionAddrRVA;
}


int main() {
	char fpath[] = "E:\\Github\\repo\\Mylib\\defDLL\\x64\\Debug\\defDLL.dll";
	char* fbuffer;

	fbuffer = FileToBuffer(fpath);

	//function1();

	char FunctionName[] = "ptrCharacteristics";
	//char FunctionName1[] = "ptrCharacteristics";

	printf("%x\n",GetFunctionAddrByName(fbuffer, FunctionName));
	//printf("%d\n", sizeof(FunctionName));

	GetFunctionAddrByOrdinals(fbuffer, 2);
	

}