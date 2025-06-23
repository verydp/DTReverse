#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

//1����д�����ӡ���еĵ�������Ϣ
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
	printf("NumberOfFunctions = %x\t//��������\n", ExportDirectory->NumberOfFunctions);				//��������
	printf("NumberOfNames = %x\t//��������\n", ExportDirectory->NumberOfNames);						//��������
	printf("AddressOfFunctions = %x\t//������ַ\n", ExportDirectory->AddressOfFunctions);			//������ַ
	printf("AddressOfNames = %x\t//���ֵ�ַ\n", ExportDirectory->AddressOfNames);					//���ֵ�ַ
	printf("AddressOfNameOrdinals = %x\t//������ŵ�ַ\n", ExportDirectory->AddressOfNameOrdinals);	//������ŵ�ַ
	

	//���Դ�ӡ
	printf("==================������ַ====================\n");
	for (unsigned int i = 0; i < ExportDirectory->NumberOfFunctions; i++) {
		printf("%x\t%x\n",i+1 ,*( (int*)(fbuffer + RVA_TO_FOA(fbuffer,ExportDirectory->AddressOfFunctions + i * 4) ) ));
	}

	printf("==================��������====================\n");
	for (unsigned int i = 0; i < ExportDirectory->NumberOfNames; i++) {
		int* addressnameRVA;
		//�˴�����������ָ����ʼ��ַ����Ҫ��AddressOfNames�������ַת�����ļ���ַ
		addressnameRVA = (int*)(fbuffer + RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfNames));
		//printf("%x\n", RVA_TO_FOA(fbuffer, *(addressnameRVA + i)));
		//��ȡ��ÿ����������ָ�룬��Ҫ���ݺ�����ָ���ַ(�ڴ��ַ)��ȡ��������ַ(�ļ���ַ)
		for (int x = 0; *(fbuffer + RVA_TO_FOA(fbuffer, *(addressnameRVA + i)) + x) != 0; x++) {
			printf("%c", *(fbuffer + RVA_TO_FOA(fbuffer, *(addressnameRVA + i)) + x) );
		}
		printf("\n");
	}
	printf("==================�������====================\n");
	//������ſ���ֱ���Ƶ���������ַ��ָ���ַ���Ӷ���ȡ��������ʼ�ڴ��ַ
	int Ordinals;
	for (int i = 0; i < ExportDirectory->NumberOfFunctions; i++) {
		Ordinals = ExportDirectory->Base + *(fbuffer + RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfNameOrdinals + 2 * i));
		printf("%x\n", Ordinals);
	}
	
}


//2��GetFunctionAddrByName(FileBufferָ�룬������),�˴��������Ǻ�������FOA
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
		//�˴�����������ָ����ʼ��ַ����Ҫ��AddressOfNames�������ַת�����ļ���ַ
		paddressnameRVA = (int*)(fbuffer + RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfNames));
		//printf("%x\n", RVA_TO_FOA(fbuffer, *(addressnameRVA + i)));
		//��ȡ��ÿ����������ָ�룬��Ҫ���ݺ�����ָ���ַ(�ڴ��ַ)��ȡ��������ַ(�ļ���ַ)

		//printf("%x\n", *paddressnameRVA);
		//��ȡ��������FOA
		addressnameFOA = (char*)(fbuffer + RVA_TO_FOA(fbuffer, *(paddressnameRVA + i)));
		//printf("%x\n", RVA_TO_FOA(fbuffer, *(addressnameRVA + i)));
		//NameContain = ()*addressnameFOA

		int x;
		x = 0;
		//��ʼ�жϺ������ʹ�������Ƿ���ͬ��
		while (x <= sizeof(FunctionName)) {
			if (x == sizeof(FunctionName)) {
				//���غ�������FOA
				return RVA_TO_FOA(fbuffer, *(paddressnameRVA + i));
				//���غ�������RVA
				return *(paddressnameRVA + i);
			}
			else {
				//����������ĵ�x���ֽںʹ������FunctionName�ĵ�x�ֽ���ͬ
				if (FunctionName[x] == *(addressnameFOA + x)) {
					x++;
				}
				//�����ͬ����ֱ���Ƴ�while�ж�
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

//3��GetFunctionAddrByOrdinals(FileBufferָ�룬�������������),����ֵ�Ǻ�����RVA
int GetFunctionAddrByOrdinals(char* fbuffer, int Ordinary) {
	int FunctionAddrRVA;

	IMAGE_OPTIONAL_HEADER64* opeheader;
	opeheader = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	//printf("%x\n", opeheader->DataDirectory[0].VirtualAddress);
	_IMAGE_EXPORT_DIRECTORY* ExportDirectory;
	ExportDirectory = (_IMAGE_EXPORT_DIRECTORY*)(fbuffer + RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));
	//printf("%x\n", RVA_TO_FOA(fbuffer, opeheader->DataDirectory[0].VirtualAddress));

	//1�����Ȼ�ȡ�������ordinaryBase
	int OrdinaryBase;
	OrdinaryBase = ExportDirectory->Base;
	printf("%d\n", OrdinaryBase);
	//2�������������Ordinary - ordinaryBase = ������ַ���ƫ�Ƹ���
	int offset;
	offset = Ordinary - OrdinaryBase;
	//3��ȡ��������ַ��ľ���RVA��ַ
	int pAddrOfFunctionsRVA;
	pAddrOfFunctionsRVA = ExportDirectory->AddressOfFunctions;
	//4����������ַ���RVA��ַת����FOA��ַ��
	int* pAddrOfFunctionsFOA;
	pAddrOfFunctionsFOA = (int*)(fbuffer + RVA_TO_FOA(fbuffer, pAddrOfFunctionsRVA));
	//5�����ݺ�����ַ���FOAȡ��������RVA��ַ
	int pFunctionRVA = *pAddrOfFunctionsFOA;
	//6�����ݺ�����RVA��ַ���ó�������FOA��ַ��
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