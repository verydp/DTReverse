#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

//D:\\IDA_Pro_7.5SP3\\IDA Pro 7.5 SP3(x86, x64, ARM, ARM64, PPC, PPC64, MIPS)\\ida.exe


//遍历有多少张导入表
//每获取一张导入表，遍历里面的函数名和文件地址

void BindImportDirectory(char* fbuffer) {

	//获取文件的导入表
	IMAGE_OPTIONAL_HEADER32* OPE;
	OPE = (IMAGE_OPTIONAL_HEADER32*)ptrOptionPE(fbuffer);

	UINT ImportDescriptorRVA;
	UINT ImportDescriptorFOA;
	ImportDescriptorRVA = OPE->DataDirectory[1].VirtualAddress;
	ImportDescriptorFOA = RVA_TO_FOA(fbuffer, ImportDescriptorRVA);


	IMAGE_IMPORT_DESCRIPTOR* ImportDescriptor;
	ImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)(fbuffer + ImportDescriptorFOA);
	//printf("%d\n", sizeof(IMAGE_IMPORT_DESCRIPTOR));

	UINT BindImportDescriptorRVA;
	UINT BindImportDescriptorFOA;
	BindImportDescriptorRVA = OPE->DataDirectory[11].VirtualAddress;
	BindImportDescriptorFOA = RVA_TO_FOA(fbuffer, BindImportDescriptorRVA);

	

	for (; ImportDescriptor->Characteristics != 0; ImportDescriptor += 1) {
		//printf("###########ImportDescriptorFOA = %x###########\n", (char*)ImportDescriptor - fbuffer);
		printf("==================================\n");
		//开始读取导入描述表
		
		//显示DLL库名字
		int NameRVA;
		int NameFOA;
		NameRVA = ImportDescriptor->Name;
		NameFOA = RVA_TO_FOA(fbuffer, NameRVA);
		printf("===========");
		showstring(fbuffer + NameFOA);
		printf("===========\n");


		unsigned int ImportDescriptorTime;
		ImportDescriptorTime = ImportDescriptor->TimeDateStamp;

		printf("ImportDescriptorTime = %x\n", ImportDescriptorTime);
		printf("==================================\n");
		
		//时间戳为0表示没有绑定，时间戳为1表示已经绑定

		//！！！需要注意offset表示的是dll和第一个BindImportDescriptor的偏移量！！！
		if (ImportDescriptorTime == -1) {

			IMAGE_BOUND_IMPORT_DESCRIPTOR* BindImportDescriptor;
			BindImportDescriptor = (IMAGE_BOUND_IMPORT_DESCRIPTOR*)(fbuffer + BindImportDescriptorFOA);

			for (; BindImportDescriptor->OffsetModuleName != 0; BindImportDescriptor += 1) {
				//printf("++++++++++++++++++++++++++++++++++\n");
				printf("TimeDateStamp is %x\n", BindImportDescriptor->TimeDateStamp);
				printf("ModuleName is ");
 				showstring((char*)(fbuffer + BindImportDescriptorFOA + BindImportDescriptor->OffsetModuleName));
				printf("\n");
				printf("NumberOfModuleForwarderRefs = %d\n", BindImportDescriptor->NumberOfModuleForwarderRefs);
				//printf("++++++++++++++++++++++++++++++++++\n");
				
				if (BindImportDescriptor->NumberOfModuleForwarderRefs) {

					IMAGE_BOUND_FORWARDER_REF* BoundRef;
					BoundRef = (IMAGE_BOUND_FORWARDER_REF*)BindImportDescriptor;

					for (int i = 0; i < BindImportDescriptor->NumberOfModuleForwarderRefs; i++) {
						//printf("----------------------------------\n");
						printf("TimeDateStamp is %x\n", BoundRef->TimeDateStamp);
						printf("ModuleName is ");
						showstring((char*)(fbuffer + BindImportDescriptorFOA + BoundRef->OffsetModuleName));
						printf("\n");
						//printf("----------------------------------\n");
						BoundRef++;
					}
				}
				printf("\n");
			}



		}
		


		//如果导入表的时间戳表示已经绑定了导入表，开始查看

	}

	/*
	
	//判断是否存在导入描述表
	for (; ImportDescriptor->Characteristics != 0; ImportDescriptor += 1) {
		printf("###########ImportDescriptorFOA = %x###########\n", (char*)ImportDescriptor - fbuffer);

		//开始读取导入描述表

		//显示DLL库名字
		int NameRVA;
		int NameFOA;
		NameRVA = ImportDescriptor->Name;
		NameFOA = RVA_TO_FOA(fbuffer, NameRVA);
		printf("===========Import DLL ");
		showstring(fbuffer + NameFOA);


		unsigned int INTRVA;
		unsigned int INTFOA;
		INTRVA = ImportDescriptor->OriginalFirstThunk;
		INTFOA = RVA_TO_FOA(fbuffer, INTRVA);

		printf("INTFOA = %x\n", INTFOA);

		IMAGE_THUNK_DATA32* INThunk;
		INThunk = (IMAGE_THUNK_DATA32*)(fbuffer + INTFOA);

		for (; INThunk->u1.ForwarderString != 0; INThunk += 1) {
			//printf("this is new Thunk~\n");

			if (INThunk->u1.ForwarderString >> 31 == 1) {
				//判断最高位是否为1 如果时 那么除去最高位的值就是函数的导出序号
				printf("The Ordinal of Function is %x\n", INThunk->u1.ForwarderString - 0x80000000);
				//printf("INTFOA = %x\n", INTFOA);
			}
			else {
				//如果最高位不是1，则函数是以函数名导入
				ULONGLONG INThunkNameRVA;
				ULONGLONG INThunkNameFOA;
				INThunkNameRVA = INThunk->u1.AddressOfData;
				INThunkNameFOA = RVA_TO_FOA(fbuffer, INThunk->u1.AddressOfData);

				//printf("The address of data import by name %x\n", INThunkNameFOA);


				IMAGE_IMPORT_BY_NAME* FunctionName;
				FunctionName = (IMAGE_IMPORT_BY_NAME*)(fbuffer + INThunkNameFOA);

				showstring(FunctionName->Name);
			}
		}


		printf("///////////////////////////////////////////////////////\n");



		int IATRVA;
		int IATFOA;
		IATRVA = ImportDescriptor->FirstThunk;
		IATFOA = RVA_TO_FOA(fbuffer, IATRVA);
		printf("IAThunkFOA = %x\n", IATFOA);

		IMAGE_THUNK_DATA32* IAThunk;
		IAThunk = (IMAGE_THUNK_DATA32*)(fbuffer + IATFOA);

		for (; IAThunk->u1.ForwarderString != 0; IAThunk += 1) {
			//printf("this is new Thunk~\n");

			if (IAThunk->u1.ForwarderString >> 31 == 1) {
				//判断最高位是否为1 如果时 那么除去最高位的值就是函数的导出序号
				printf("The Ordinal of Function is %x\n", IAThunk->u1.ForwarderString - 0x80000000);
				//printf("INTFOA = %x\n", INTFOA);
			}
			else {
				//如果最高位不是1，则函数是以函数名导入
				int IAThunkNameRVA;
				int IAThunkNameFOA;
				IAThunkNameRVA = INThunk->u1.AddressOfData;
				IAThunkNameFOA = RVA_TO_FOA(fbuffer, IAThunk->u1.AddressOfData);

				//printf("The address of data import by name %x\n", INThunkNameFOA);


				IMAGE_IMPORT_BY_NAME* FunctionName;
				FunctionName = (IMAGE_IMPORT_BY_NAME*)(fbuffer + IAThunkNameFOA);

				showstring(FunctionName->Name);
			}
		}

	}
	*/

}

int main() {
	//将文件读取到内存中
	char fpath[] = "E:\\Studies\\滴水课件\\2015-03-30绑定导入表\\notepad.exe";
	//char fpath[] = "D:\\IDA_Pro_7.5SP3\\IDA Pro 7.5 SP3(x86, x64, ARM, ARM64, PPC, PPC64, MIPS)\\ida.exe";
	char* fbuffer;
	fbuffer = FileToBuffer(fpath);

	BindImportDirectory(fbuffer);


}