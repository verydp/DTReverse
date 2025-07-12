#include "defDLL.h"
#include <Windows.h>
#include <stdio.h>

//2��ʹ�ù��ߴ��޸ĺ��DLL ���ܷ���������.
//3����DLL������һ���ڣ������ض�λ���ƶ�������µĽ���.
//4���޸�DLL��ImageBase,�����ض�λ��������Ȼ�����.��DLL�Ƿ����ʹ��.

//1����DLL����һ���ڣ�������������Ϣ�ƶ�������µĽ���.
/*
 *
 *
 *
 * �ƶ�addressoffunctions��
 * �ƶ�nameofordinal��
 * �ƶ����ֱ�
 *
 *
 *
 *
 *
 *
 *
 *
 */
 //����ڱ�����ʣ��ռ�
int SectionRestSpace(char* fbuffer) {
	//�ڱ���ʼ��ַ
	char* pSection = ptrSection(fbuffer);

	//������
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(fbuffer);

	//�ڱ������FOA
	int SectionEndFOA;
	SectionEndFOA = pSection + 40 * NumberOfSection - fbuffer;

	int SpaceMin, CurrentSpace;

	SpaceMin = *ptrPointerToRawData(pSection) - SectionEndFOA;
	//ͨ�������ڱ���Ϣ����ȡÿһ���ڵ������ַ��Χ��VirtualAddress ~ VirtualAddress + SizeRawData��
	for (int i = 0; i < NumberOfSection - 1; i++) {
		//printf("%d\n", SpaceMin);
		CurrentSpace = *ptrPointerToRawData(ptrSection(fbuffer) + 40 * i) - SectionEndFOA;

		if (SpaceMin < 0) {
			SpaceMin = CurrentSpace;
		}
		else {
			if (SpaceMin > CurrentSpace) {
				SpaceMin = CurrentSpace;
			}
		}

	}
	printf("The Min Rest Space is %x\n", SpaceMin);
	printf("There are %d Sections Can be added~\n", (SpaceMin - 40) / 40);
	return SpaceMin;
}

int GetSizeOfFbuffer(char* fbuffer) {
	//��һ���ڱ�ָ��
	char* pSection;
	pSection = ptrSection(fbuffer);

	//������
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(fbuffer);

	//��ʼ���ļ���СΪ0
	int FileSize;
	FileSize = 0;

	int PointerToRawData;
	int SizeOfRawData;

	for (int i = 0; i < NumberOfSection; i++) {
		PointerToRawData = *ptrPointerToRawData(pSection + i * 40);
		SizeOfRawData = *ptrSizeOfRawData(pSection + i * 40);

		if (PointerToRawData + SizeOfRawData > FileSize) {
			FileSize = PointerToRawData + SizeOfRawData;
		}
		//printf("%x\n", FileSize);
	}
	printf("FileSize = %x\n", FileSize);
	return FileSize;
}

//3������һ���������ܹ����ض����Ĵ�СAlign(int x, int y)
int FileAlignment(char* fbuffer, int LengthOfData) {
	int result;
	printf("LengthOfData = %x\n", LengthOfData);
	int LengthOfFileAlign;
	LengthOfFileAlign = *ptrFileAlignment(fbuffer);
	printf("LengthOfFileAlign = %x\n", LengthOfFileAlign);

	result = (LengthOfData / LengthOfFileAlign + 1) * LengthOfFileAlign;
	printf("result = %x\n", result);

	return result;
}

int SectionAlignment(char* fbuffer, int LengthOfData) {
	int result;
	printf("LengthOfData = %x\n", LengthOfData);
	int LengthOfMemoryAlign;
	LengthOfMemoryAlign = *ptrMemoryAlignment(fbuffer);
	printf("LengthOfFileAlign = %x\n", LengthOfMemoryAlign);

	result = (LengthOfData / LengthOfMemoryAlign + 1) * LengthOfMemoryAlign;
	printf("result = %x\n", result);

	return result;
}

int SectionCharacteristics(char* fbuffer, int rva) {
	char* pSection;
	IMAGE_SECTION_HEADER* Section;
	int VirtualAddressRVA;
	int SizeOfRawData;

	int numberofsection;
	numberofsection = *ptrNumberOfSection(fbuffer);

	for (int i = 0; i < numberofsection; i++) {
		pSection = ptrSection(fbuffer) + i * 40;
		Section = (IMAGE_SECTION_HEADER*)pSection;
		if (Section->VirtualAddress <= rva && Section->VirtualAddress + Section->SizeOfRawData) {
			return Section->Characteristics;
		}
	}
	printf("can\'t find the RVA in which Section~\n");
	return 0;

}

typedef struct PEstruct64 {
	IMAGE_DOS_HEADER DOSHEADER;
	IMAGE_FILE_HEADER PEHEADER;
	IMAGE_OPTIONAL_HEADER64 OPEHEADER;
	IMAGE_EXPORT_DIRECTORY EXPORT;
	IMAGE_RELOCATION RELOCATION;
	IMAGE_IMPORT_DESCRIPTOR IMPORT;
};

PEstruct64 InitPEstruct(char* fbuffer) {
	PEstruct64 PE;

	memcpy(&PE.DOSHEADER, fbuffer, sizeof(IMAGE_DOS_HEADER));
	memcpy(&PE.PEHEADER, ptrPE(fbuffer), sizeof(IMAGE_FILE_HEADER));
	memcpy(&PE.OPEHEADER, ptrOptionPE(fbuffer), sizeof(IMAGE_OPTIONAL_HEADER64));
	memcpy(&PE.EXPORT, fbuffer + RVA_TO_FOA(fbuffer, PE.OPEHEADER.DataDirectory[0].VirtualAddress), sizeof(IMAGE_EXPORT_DIRECTORY));
	memcpy(&PE.RELOCATION, fbuffer + RVA_TO_FOA(fbuffer, PE.OPEHEADER.DataDirectory[5].VirtualAddress), sizeof(IMAGE_RELOCATION));
	memcpy(&PE.IMPORT, fbuffer + RVA_TO_FOA(fbuffer, PE.OPEHEADER.DataDirectory[1].VirtualAddress), sizeof(IMAGE_IMPORT_DESCRIPTOR));
	//PE.PEHEADER = (IMAGE_FILE_HEADER*)(ptrPE(fbuffer));
	//PE.OPEHEADER = (IMAGE_OPTIONAL_HEADER64*)(ptrOptionPE(fbuffer));

	return PE;
}

void MoveExportDirectory(char* fbuffer) {
	//��ȡ��ǰfbuffer�ĵ�����
	_IMAGE_OPTIONAL_HEADER64* OPE64;
	OPE64 = (_IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	int ExportTableRVA;
	ExportTableRVA = OPE64->DataDirectory[0].VirtualAddress;

	int ExportTableFOA;
	ExportTableFOA = RVA_TO_FOA(fbuffer, ExportTableRVA);

	IMAGE_EXPORT_DIRECTORY* ExportTable;
	ExportTable = (IMAGE_EXPORT_DIRECTORY*)(fbuffer + ExportTableFOA);

	//��ȡ������ļ���������Ϣ��������ַ�����ֱ�������ű��������������ָ���
	int AddrFuncTableFOA;
	int NameTablePtrFOA;
	int NameOrdinalFOA;
	int NumberOfFunction;
	int NumberOfName;
	//������ַ�����ֱ�������ű�
	AddrFuncTableFOA = RVA_TO_FOA(fbuffer, ExportTable->AddressOfFunctions);
	NameTablePtrFOA = RVA_TO_FOA(fbuffer, ExportTable->AddressOfNames);
	NameOrdinalFOA = RVA_TO_FOA(fbuffer, ExportTable->AddressOfNameOrdinals);
	//�������������ָ���
	NumberOfFunction = ExportTable->NumberOfFunctions;
	NumberOfName = ExportTable->NumberOfNames;

	//�ж��ܹ����������ڱ���Ϣ
	int space;
	space = SectionRestSpace(fbuffer);

	//�жϵ�����ռ�ÿռ䣺40�ֽ�
	int sizeofExportDict;
	sizeofExportDict = sizeof(_IMAGE_EXPORT_DIRECTORY);

	//�жϵ�ַ��ռ�ÿռ䣺(9-2+1)*4 = 32�ֽ�
	int SizeOfAddrFuncTable;
	SizeOfAddrFuncTable = NumberOfFunction * 4;

	//�ж����ֱ�ռ�ÿռ䣺(4*4)=16�ֽ�
	int SizeOfNameTable;
	SizeOfNameTable = NumberOfName * 4;

	//�ж�������ű�ռ�ÿռ䣺(4*2)=8�ֽ�
	int SizeOfNameOrdinalsTable;
	SizeOfNameOrdinalsTable = NumberOfName * 2;

	//�ж�����ռ�ÿռ䣺5 + 4 + 6 + 6 = 21�ֽ�
	int SizeOfName;
	int* ptrNameTable;
	int NamePtrFOA;
	char* ptrName;
	int NameLength;
	NameLength = 0;
	for (int i = 0; i < NumberOfName; i++) {
		ptrNameTable = (int*)(fbuffer + NameTablePtrFOA) + i;
		NamePtrFOA = RVA_TO_FOA(fbuffer, *ptrNameTable);
		ptrName = fbuffer + NamePtrFOA;
		NameLength = NameLength + strlen(ptrName);
		//printf("NameLength = %d\n", NameLength);
	}
	//ÿ�������ַ������涼������һ���ֽڣ�Ϊ�ַ���������0
	SizeOfName = NameLength + NumberOfName;

	//��ȡ���������ֳ���
	int SizeOfDLLName;
	int DLLNameFOA;
	DLLNameFOA = RVA_TO_FOA(fbuffer, ExportTable->Name);

	SizeOfDLLName = strlen(fbuffer + DLLNameFOA) + 1;


	//��ȡ��ǰ�ļ��Ĵ��̴�С��ec00
	int SizeOfFile;
	SizeOfFile = GetSizeOfFbuffer(fbuffer);

	//��ȡ�ļ����ļ����������200
	int FileAlign;
	FileAlign = *ptrFileAlignment(fbuffer);

	//�����ļ����������������Ҫ���ӵ��ڴ�ռ�
	int PhysicalSize;
	int SizeOfRawData;
	PhysicalSize = sizeofExportDict + SizeOfAddrFuncTable + SizeOfNameTable + SizeOfNameOrdinalsTable + SizeOfName + SizeOfDLLName;
	//printf("%d\t%d\t%d\t%d\t%d\n", sizeofExportDict , SizeOfAddrFuncTable, SizeOfNameTable , SizeOfNameOrdinalsTable , NameLength);
	SizeOfRawData = FileAlignment(fbuffer, PhysicalSize);

	//����һ�����ڴ�
	char* nbuffer;
	nbuffer = (char*)malloc(SizeOfRawData + SizeOfFile);





	//д���½ڱ���Ϣ
	if (nbuffer != NULL) {

		memset(nbuffer, 0, SizeOfRawData + SizeOfFile);
		memcpy(nbuffer, fbuffer, SizeOfFile);

		//��ȡPEָ��ṹ��
		PEstruct64 NewPE;
		NewPE = InitPEstruct(nbuffer);
		/*****************************************��ʼ������*****************************************************/
		//׼���½�����
		char Section_Name[] = "\.newout";												//������
		int Section_PhysicalSize;
		Section_PhysicalSize = PhysicalSize;										//�������С
		int Section_VirtualAddress;
		Section_VirtualAddress = *ptrSizeOfImage(nbuffer);							//�������ַ
		int Section_PtrRawDataFOA;
		Section_PtrRawDataFOA = SizeOfFile;											//��FOA
		int Section_SizeOfRawData;
		Section_SizeOfRawData = SizeOfRawData;										//���ļ�����Ĵ�С
		//��ȡ�����ԣ���Ҫ��ԭʼ��������ͬ
		int ExportTableRVA;
		ExportTableRVA = NewPE.OPEHEADER.DataDirectory[0].VirtualAddress;			//��ȡ�������RVA
		int Section_Characteristics;
		Section_Characteristics = SectionCharacteristics(nbuffer, ExportTableRVA);	//������SectionCharacteristics

		IMAGE_SECTION_HEADER* ptrNewSection;
		ptrNewSection = (IMAGE_SECTION_HEADER*)(ptrSection(nbuffer) + 40 * NewPE.PEHEADER.NumberOfSections);	//�½���ʼ��ַ
		//��ʼд���½�
		memcpy(ptrNewSection, Section_Name, sizeof(Section_Name));					//��ֵ������
		ptrNewSection->Misc.PhysicalAddress = Section_PhysicalSize;					//��ֵ����ʵ��С
		ptrNewSection->VirtualAddress = Section_VirtualAddress;						//��ֵ�ڵ������ַ��sizeofimage�Ľ�����ַ
		ptrNewSection->PointerToRawData = Section_PtrRawDataFOA;					//��ֵ�ڵ��ļ�ƫ��
		ptrNewSection->SizeOfRawData = Section_SizeOfRawData;						//��ֵ�ļ������Ĵ�С
		ptrNewSection->Characteristics = Section_Characteristics;					//��ֵ�ļ�����
		/*****************************************���������*****************************************************/
		//�޸�sizeofimage;
		*ptrSizeOfImage(nbuffer) = *ptrSizeOfImage(nbuffer) + SectionAlignment(nbuffer, Section_PhysicalSize);
		//�޸Ľ�����
		*ptrNumberOfSection(nbuffer) = *ptrNumberOfSection(nbuffer) + 1;

		char* CurPtr;

		CurPtr = nbuffer + SizeOfFile;

		//�ƶ�������
		printf("------try to move ExportTable table------\n");
		memcpy(CurPtr, &NewPE.EXPORT, sizeof(NewPE.EXPORT));

		IMAGE_EXPORT_DIRECTORY* NewExportTable;
		NewExportTable = (IMAGE_EXPORT_DIRECTORY*)CurPtr;

		IMAGE_OPTIONAL_HEADER64* OPE;
		OPE = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(nbuffer);

		OPE->DataDirectory[0].VirtualAddress = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);	//�޸ĵ�����ĵ�ַ

		CurPtr = CurPtr + sizeof(NewPE.EXPORT);

		//�ƶ������������磺XXX.dll
		printf("------try to move DLLname------\n");
		int DLLNameFOA;
		DLLNameFOA = RVA_TO_FOA(nbuffer, NewExportTable->Name);
		char* ptrDLLName;
		ptrDLLName = nbuffer + DLLNameFOA;

		memcpy(CurPtr, ptrDLLName, strlen(ptrDLLName));

		NewExportTable->Name = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);		//�޸ĵ�����Ŀ���RVA

		CurPtr = CurPtr + strlen(ptrDLLName) + 1;

		//�ƶ���ַ����ȡ��ַ���FOA��Ŀ�ĵ�ַ����ַ���С(32byte)
		printf("------try to move function address table------\n");
		int FuncAddrTableFOA;
		FuncAddrTableFOA = RVA_TO_FOA(nbuffer, NewPE.EXPORT.AddressOfFunctions);
		memcpy(CurPtr, nbuffer + FuncAddrTableFOA, SizeOfAddrFuncTable);

		FuncAddrTableFOA = SizeOfFile;

		NewExportTable->AddressOfFunctions = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);//�޸ĵ�����ĺ�����ַ��

		CurPtr = CurPtr + SizeOfAddrFuncTable;
		//�ƶ���ű���ű�FOA��Ŀ�ĵ�ַ����ű��С(16byte)
		printf("------try to move name ordinals table------\n");
		int OrdinalTableFOA;
		OrdinalTableFOA = RVA_TO_FOA(nbuffer, NewPE.EXPORT.AddressOfNameOrdinals);
		memcpy(CurPtr, nbuffer + OrdinalTableFOA, SizeOfNameOrdinalsTable);

		NewExportTable->AddressOfNameOrdinals = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);//�޸ĵ��������ű��ַ

		CurPtr = CurPtr + SizeOfNameOrdinalsTable;
		//�ƶ����ֱ� & ����
		//�ҵ�����ָ�롢�ҵ����֡���ȡ���ֳ��ȡ���ʼ����
		printf("------try to move name table------\n");
		int NamePtrArrayRVA;
		NamePtrArrayRVA = NewPE.EXPORT.AddressOfNames;

		int NamePtrArrayFOA;
		NamePtrArrayFOA = RVA_TO_FOA(nbuffer, NamePtrArrayRVA);

		int* NamePtr;
		int NameFOA;
		int LengthOfName;

		char* NameInSection;
		NameInSection = CurPtr + 4 * NumberOfName;

		int* NamePtrInSection;
		NamePtrInSection = (int*)CurPtr;

		int NewNamePtrRVA;

		for (int i = 0; i < NewPE.EXPORT.NumberOfNames; i++) {

			NamePtr = (int*)(nbuffer + NamePtrArrayFOA) + i;		//����ָ������
			NameFOA = RVA_TO_FOA(nbuffer, *NamePtr);				//����FOA

			LengthOfName = strlen(nbuffer + NameFOA);				//���ֳ���

			memcpy(NameInSection, nbuffer + NameFOA, LengthOfName);	//��ʼ����

			NewNamePtrRVA = FOA_TO_RVA(nbuffer, NameInSection - nbuffer);//�������ֵ�FOA�������ֵ�RVA

			*(NamePtrInSection + i) = NewNamePtrRVA;				//���������RVA�ŵ��µ�����ָ�����

			NameInSection = NameInSection + LengthOfName + 1;		//�������ֿ������ָ��λ�ã�+1��Ԥ��һ��0�Ľ������

		}
		NewExportTable->AddressOfNames = FOA_TO_RVA(nbuffer, CurPtr - nbuffer);


		//���ļ�����
		char fpath[] = "E:\\Github\\repo\\Demo\\StudyDLL\\x64\\Debug\\0326\\DLLMoveExport.dll";
		BufferToFile(nbuffer, fpath);


	}

	printf("\n");
}

int SizeOfFile(char* fbuffer) {
	//��ȡ�������
	short numberofsection;
	numberofsection = *ptrNumberOfSection(fbuffer);

	//��ȡ�����ʼ��ַ
	char* section0;
	section0 = ptrSection(fbuffer);

	//��ȡ���һ�ڿ����ʼ��ַ
	char* section;
	section = section0 + (numberofsection - 1) * 40;

	_IMAGE_SECTION_HEADER* SectionHeader;
	SectionHeader = (_IMAGE_SECTION_HEADER*)section;

	return SectionHeader->SizeOfRawData + SectionHeader->PointerToRawData;
}

void MoveRelocationDirectory(char* fbuffer, unsigned long long imagebase) {

	int FileSize;
	FileSize = SizeOfFile(fbuffer);
	printf("FileSize = %x\n", FileSize);

	char* nbuffer;
	nbuffer = NewBuffer(FileSize);

	memcpy(nbuffer, fbuffer, FileSize);

	//�����ҵ��ض�λ��
	IMAGE_OPTIONAL_HEADER64* OPE;
	OPE = (IMAGE_OPTIONAL_HEADER64*)(ptrOptionPE(nbuffer));

	int RelocatedRVA;
	RelocatedRVA = OPE->DataDirectory[5].VirtualAddress;

	int RelocatedFOA;
	RelocatedFOA = RVA_TO_FOA(nbuffer, RelocatedRVA);

	char* ptrRelocated;
	ptrRelocated = nbuffer + RelocatedFOA;

	_IMAGE_BASE_RELOCATION* Relocation;
	Relocation = (_IMAGE_BASE_RELOCATION*)ptrRelocated;

	//printf("Relocation->VirtualAddress = %x\n", Relocation->VirtualAddress);
	//printf("Relocation->SizeOfBlock = %x\n", Relocation->SizeOfBlock);
	short* ptrRawAddr;

	int offset;
	offset = imagebase - OPE->ImageBase;		//��ȡimagebase��ƫ����

	OPE->ImageBase = imagebase;

	unsigned long long* PublicArg;

	while (Relocation->VirtualAddress != 0 && Relocation->SizeOfBlock != 0) {
		printf("Current VirtualAddress: %x\n", Relocation->VirtualAddress);
		printf("Current SizeOfBlock: %x\n", Relocation->SizeOfBlock);

		unsigned long sizeofblock = (Relocation->SizeOfBlock - 8) / 2;

		for (int i = 0; i < sizeofblock; i++) {
			//�ڸ��ض�λ�����ϣ��������еĶ���(2�ֽ�Ϊһ���ض�λ����)
			ptrRawAddr = (short*)(ptrRelocated + 8) + i;

			unsigned short Raw;
			Raw = *ptrRawAddr;

			int RawRVA;
			int RawFOA;

			//ע��64bit�ĸ���λΪ0xaʱ��˵���ö�����Ҫ���޸ģ���32bit�����ֵ��Ϊ3��˵����Ҫ���޸�
			if (Raw >> 12 == 0xa) {
				//��λ�ȡ����12λ��ֵ��
				//��16λ(2�ֽ�)��ֵ - 0xa000 = ��12λ��ֵ
				Raw = Raw - 0xa000;
				RawRVA = Relocation->VirtualAddress + Raw;
				RawFOA = RVA_TO_FOA(nbuffer, RawRVA);

				printf("RVA = %x (FOA = %x) need to be relocated\n", RawRVA, RawFOA);

				PublicArg = (unsigned long long*)(nbuffer + RawFOA);
				//printf("PublicArg = %x\n", *PublicArg);
				*PublicArg = *PublicArg + (unsigned long long)offset;
			}
			//printf("%x\n", Raw >>12);
		}

		ptrRelocated = ptrRelocated + Relocation->SizeOfBlock;
		Relocation = (_IMAGE_BASE_RELOCATION*)ptrRelocated;
	}

	char fpath[] = "E:\\Github\\repo\\Demo\\StudyDLL\\x64\\Debug\\0326\\DLLMoveReloction.dll";
	BufferToFile(nbuffer, fpath);
}

int main() {
	char fpath[] = "E:\\Github\\repo\\Demo\\StudyDLL\\x64\\Debug\\0326\\StudyDLL.dll";
	char* fbuffer;
	fbuffer = FileToBuffer(fpath);

	MoveExportDirectory(fbuffer);

	MoveRelocationDirectory(fbuffer, 0x190000000);
}