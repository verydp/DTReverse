#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>
#include <string.h>

//������������Ϣ���������ֻ�ȡ����RVA��ַ��������Ż�ȡ����RVA��ַ
//1����д�����ӡ���еĵ�������Ϣ
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
	printf("Name = %x\t\t\t//DLL����\n", ExportDirectory->Name);
	printf("Base = %x\t\t\t//������ű��Base\n", ExportDirectory->Base);
	printf("NumberOfFunctions = %x\t\t//��������\n", ExportDirectory->NumberOfFunctions);				//��������
	printf("NumberOfNames = %x\t\t//��������\n", ExportDirectory->NumberOfNames);						//��������
	printf("AddressOfFunctions = %x\t//������ַ\n", ExportDirectory->AddressOfFunctions);			//������ַ
	printf("AddressOfNames = %x\t\t//���ֵ�ַ\n", ExportDirectory->AddressOfNames);					//���ֵ�ַ
	printf("AddressOfNameOrdinals = %x\t//������ŵ�ַ\n", ExportDirectory->AddressOfNameOrdinals);	//������ŵ�ַ

	//���Դ�ӡ
	printf("==================��������====================\n");
	//��ȡ���������֡�������
	int numberofnames;
	numberofnames = ExportDirectory->NumberOfNames;
	//printf("%x\n", numberofnames);

	//���ݵ�����Ľṹ�壬��ȡ������ָ����RVA
	int NamesPtrArrayRVA;
	NamesPtrArrayRVA = ExportDirectory->AddressOfNames;
	//printf("NamesPtrArrayRVA = %x\n", NamesPtrArrayRVA);

	//���ݺ�����ָ����RVA����ȡ������ָ����FOA
	int NamesPtrArrayFOA;
	NamesPtrArrayFOA = RVA_TO_FOA(fbuffer, NamesPtrArrayRVA);
	//printf("NamesPtrArrayFOA = %x\n", NamesPtrArrayFOA);
	/********************��ʱ�Ѿ���ȡ���˺�����ָ���**********************/

	char* ptrName;
	//���ݺ�����ָ����FOA��������ȡ������ָ���RVA
	int NameRVA;
	int NameFOA;

	for (int i = 0; i < numberofnames; i++) {
		//ÿ��ȡһ����������RVA��ַ��ת����FOA��ַ
		NameRVA = *((int*)(fbuffer + NamesPtrArrayFOA + i * 4));
		NameFOA = RVA_TO_FOA(fbuffer, NameRVA);
		ptrName = fbuffer + NameFOA;
		showstring(ptrName);
		//printf("%x\n", NameFOA);

	}

	/*
	* ��������ָ���
	* 89 B9 01 00		dim			@5
	* 8D B9 01 00		func2		@7
	* 93 B9 01 00		func3		@9
	* 99 B9 01 00		plus		@2
	*/
	///////////////////���¶�Ӧ//////////////////////
	/*
	* ��������
	* 64 69 6D 00			dim			@5
	* 66 75 6E 63 32 00		func2		@7
	* 66 75 6E 63 33 00		func3		@9
	* 70 6C 75 73 00		plus		@2
	*/

	printf("==================�������====================\n");
	//������ſ���ֱ���Ƶ���������ַ��ָ���ַ���Ӷ���ȡ��������ʼ�ڴ��ַ
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
	* ������ű�
	* 03 00		dim		@5		//5 = 3 + 2
	* 05 00		func2	@7		//7 = 5 + 2
	* 07 00		func3	@9		//9 = 7 + 2
	* 00 00		plus	@2		//2 = 0 + 2
	*/

	printf("==================������ַ====================\n");
	int numberoffunctions;
	numberoffunctions = ExportDirectory->NumberOfFunctions;

	int* ptrFunctionAddr;
	int AddressOfFunctionsFOA;

	AddressOfFunctionsFOA = RVA_TO_FOA(fbuffer, ExportDirectory->AddressOfFunctions);
	ptrFunctionAddr = (int*)(fbuffer + AddressOfFunctionsFOA);

	for (int i = 0; i < numberoffunctions; i++) {
		printf("%x\t%x\n", i + 1, *(ptrFunctionAddr + i));
	}

	/*ʵ��DLL����ʱ����������
	LIBRARY CDLL

	EXPORTS
	plus		@2
	dim			@5
	func1		@6	Noname
	func2		@7
	func3		@9
	*/

	/***********�ļ��к�����ַ�Ĵ������**********/
	/*
	* �˴���AddrOfFunctions�ڴ���ʵ����
	* 07 13 01 00		@2		plus
	* 00 00 00 00		@3		//�˴�û�к�������0
	* 00 00 00 00		@4		//�˴�û�к�������0
	* 9F 11 01 00		@5		dim
	* 22 11 01 00		@6		func1
	* 28 10 01 00		@7		func2
	* 00 00 00 00  		@8		//�˴�û�к�������0
	* 49 12 01 00		@9		func3
	*
	* �ɴ��Ƴ���ExportDirectory->NumberOfFunctions = 9 - 2 + 1 = 8
	*/
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
			//�������ֵ���������ȡ�������
			int NameOrdinalRVA;
			NameOrdinalRVA = ExportDirectory->AddressOfNameOrdinals;

			int NameOrdinalFOA;
			NameOrdinalFOA = RVA_TO_FOA(fbuffer, NameOrdinalRVA);
			//����������ű���ȡ�������ָ��
			short* ptrNameOrdinal;
			ptrNameOrdinal = (short*)(fbuffer + NameOrdinalFOA) + i;
			//���ݺ������ָ�룬��ȡ�������
			short Ordinal;
			Ordinal = *ptrNameOrdinal;
			//��ȡ������ַָ��
			int FuncAddrPtrArrayRVA;
			FuncAddrPtrArrayRVA = ExportDirectory->AddressOfFunctions;

			int FuncAddrPtrArrayFOA;
			FuncAddrPtrArrayFOA = RVA_TO_FOA(fbuffer, FuncAddrPtrArrayRVA);

			int* ptrFuncAddrArray;
			ptrFuncAddrArray = (int*)(fbuffer + FuncAddrPtrArrayFOA) + Ordinal;

			printf("FuncAddrRVA = %x\nFcuntionFOA = %x\n", *ptrFuncAddrArray, RVA_TO_FOA(fbuffer, *ptrFuncAddrArray));

			//���غ�����ַ��RVA
			return *ptrFuncAddrArray;

		}

	}
	printf("without this function name");
	return 0;
}

//3��GetFunctionAddrByOrdinals(FileBufferָ�룬�������������),����ֵ�Ǻ�����RVA
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

	//���غ�����ַ��RVA
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

	//�˴�Demo��DLL�ļ��У����Ϊ6�ĺ�����Noname���������ģ���������ű���û����Ϣ�����ֱ���û����Ϣ��
	GetFunctionAddrByOrdinals(fbuffer, 6);


}