#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

//����PE�ļ���ȡ�ļ��ľ����С
int SizePE(char* fbuffer) {
	int NumSection;
	NumSection = *ptrNumberOfSection(fbuffer);

	IMAGE_SECTION_HEADER* section;
	int sizePE = 0;
	for (int i = 0; i < NumSection; i++) {
		section = (IMAGE_SECTION_HEADER*)(ptrSection(fbuffer) + 40 * i);
		if (section->PointerToRawData + section->Characteristics > sizePE) {
			sizePE = section->PointerToRawData + section->SizeOfRawData;
		}
	}

	return sizePE;
}

int len(char* string) {
	int i = 0;
	while (string[i] != 0) {
		i++;
	}
	return i + 1;
}

//�Զ������ڣ���Ӱ��ԭʼ�����ʹ��

//˼·(��û��ǽ���PE���ߣ������޸ĵĽ���Ϣ�Ƚ�ֱ��)
// �漰�޸ĵ�ԭ������sizeofimage��numberofsection
// �漰������ֻ�н����ݺͽڱ�
// �����Ľڱ���չ�������Ľڱ���Ϣ�������ֱ�۵��޸�
//1���޸Ľڵ�����
//2�������ڱ�
//3����ӽڡ��޸Ľڱ����������(pointertorawdata��sizeofrawdata��virtualaddress)
//4���޸�sizeofimage

//�����߼�����ȡ�ļ�buffer���жϽڱ��Ƿ��������ռ䣬ȷ����Ҫ�����ڵĴ�С�����������ڴ�ռ䣬д����������
//����Ϊԭʼfbuffer��Ҫд��Ľڴ�С
//����ֵΪһ��char*ָ�룬һ���µ��ڴ�
char* AddSection(char* fbuffer ,int sizeofshellcode, char SectionName[]) {
	//��ȡ�ڵĻ�����Ϣ������������ͷ��С���ڵ���ʼFOA
	int NumSection;
	NumSection = *ptrNumberOfSection(fbuffer);

	int CurSectionSize;
	CurSectionSize = sizeof(IMAGE_SECTION_HEADER) * (NumSection + 1);

	int section0FOA;
	section0FOA = ptrSection(fbuffer) - fbuffer;

	//��ģ����Ҫ���ж��Ƿ����㹻�Ŀռ����һ���½�
	IMAGE_SECTION_HEADER* section;
	int X = 1;
	for (int i = 0; i < NumSection; i++) {
		section = (IMAGE_SECTION_HEADER*)(ptrSection(fbuffer) + 40 * i);

		if (section->PointerToRawData - (section0FOA + CurSectionSize) < 40) {
			printf("The rest space between header and section less than 40 ,can not add a new sectionheader~\n");
			X = 0;
		}
	}



	if (X) {
		int FileAlign;
		int MemAlign;
		FileAlign = *ptrFileAlignment(fbuffer);
		MemAlign = *ptrMemoryAlignment(fbuffer);


		//��ȡ��ǰ�ļ��Ĵ�С

		int NewSectionFileSize;
		int NewSectionMemSize;
		int SrcFileSize;
		int Filesize;

		SrcFileSize = SizePE(fbuffer);
		NewSectionFileSize = ((sizeofshellcode / FileAlign) + 1) * FileAlign;
		NewSectionMemSize = ((sizeofshellcode / MemAlign) + 1) * MemAlign;
		Filesize = SrcFileSize + NewSectionFileSize;

		//����һ���ڴ�ռ���ã���չ����ļ�buffer��
		char* nbuffer;
		nbuffer = NewBuffer(Filesize);

		memcpy(nbuffer, fbuffer, SrcFileSize);

		IMAGE_OPTIONAL_HEADER64* OPE;
		OPE = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(nbuffer);

		IMAGE_FILE_HEADER* PE;
		PE = (IMAGE_FILE_HEADER*)ptrPE(nbuffer);

		//�޸Ľڱ����������(pointertorawdata��sizeofrawdata��virtualaddress)
		IMAGE_SECTION_HEADER* sectionEnd;
		sectionEnd = (IMAGE_SECTION_HEADER*)(ptrSection(nbuffer) + 40 * (PE->NumberOfSections - 1) );
		
		int EndFOA;
		int EndRVA;
		int EndSectionFileAlign;
		int EndSectionMemAlign;

		EndFOA = sectionEnd->PointerToRawData + (sectionEnd->SizeOfRawData % FileAlign == 0 ? sectionEnd->SizeOfRawData / FileAlign: sectionEnd->SizeOfRawData / FileAlign + 1) * FileAlign;
		EndRVA = sectionEnd->VirtualAddress + (sectionEnd->SizeOfRawData % MemAlign == 0 ? sectionEnd->SizeOfRawData / MemAlign: sectionEnd->SizeOfRawData / MemAlign + 1) * MemAlign;

		printf("EndFOA = %x EndRVA = %x\n", EndFOA, EndRVA);

		IMAGE_SECTION_HEADER* sectionNew;
		sectionNew = (IMAGE_SECTION_HEADER*)(ptrSection(nbuffer) + 40 * PE->NumberOfSections);

		sectionNew->PointerToRawData = EndFOA ;
		sectionNew->VirtualAddress = EndRVA;
		sectionNew->SizeOfRawData = (sizeofshellcode % FileAlign == 0 ? sizeofshellcode / FileAlign : sizeofshellcode / FileAlign + 1)* FileAlign;
		sectionNew->Characteristics = sectionEnd->Characteristics;
		sectionNew->Misc.PhysicalAddress = sizeofshellcode;
		//�޸��½ڵ�����
		memcpy(sectionNew->Name, SectionName, len(SectionName));

		printf("sectionNew->SizeOfRawData = %x\n", sectionNew->SizeOfRawData);

		//1���޸Ľڵ�����
		PE->NumberOfSections++;

		//printf("%d\n", PE->NumberOfSections);

		//4���޸�sizeofimage
		OPE->SizeOfImage = NewSectionMemSize + EndRVA;

		printf("sectionNew: PointerToRawData = %x VirtualAddress = %x SizeOfRawData = %x Characteristics = %x\n", 
			sectionNew->PointerToRawData, sectionNew->VirtualAddress, sectionNew->SizeOfRawData, sectionNew->Characteristics);


		printf("sizePE = %d\n", SizePE(nbuffer));

		return nbuffer;

	}

}

void ImportTableInjection(char* fbuffer, char* dllName, char* funcName) {
	//��ģ���ǵ����
	//��ȡ�ļ��ĵ����
	IMAGE_OPTIONAL_HEADER64* OPE;
	OPE = (IMAGE_OPTIONAL_HEADER64*)ptrOptionPE(fbuffer);

	int ImportDescriptorRVA;
	int ImportDescriptorFOA;
 	ImportDescriptorRVA = OPE->DataDirectory[1].VirtualAddress;
	ImportDescriptorFOA = RVA_TO_FOA(fbuffer, ImportDescriptorRVA);


	IMAGE_IMPORT_DESCRIPTOR* ImportDescriptor;
	ImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)(fbuffer + ImportDescriptorFOA);
	//printf("%d\n", sizeof(IMAGE_IMPORT_DESCRIPTOR));



	//�����Ƿ������ڳɹ���

	char* psection;
	for (int i = 0; i < *ptrNumberOfSection(fbuffer); i++) {

		psection = ptrSection(fbuffer) + i * 40;//�ļ�ͷ(buffer) + PEͷ��ƫ����/a_lfanew(0x108) + PE Signature(4) + PE(20) + option PE(0xf0)

		char (*name)[8];
		name = (char (*)[8])psection;			//8�ֽڣ���ʾ�ڱ����֡�
		//printf("%s\n", *name);					

		int* Misc;
		Misc = (int*)(psection + 8);			//4�ֽڣ�δ����ĳߴ磬���޸�
		//printf("%x\n", *Misc);					

		int* VirtualAddress;
		VirtualAddress = (int*)(psection + 12);	//4�ֽڣ��ý����ڴ��о���ImageBase��ַ�ľ���
		//printf("%x\n", *VirtualAddress);

		int* SizeOfRawData;
		SizeOfRawData = (int*)(psection + 16);	//4�ֽڣ��ý����ļ������Ĵ�С
		//printf("%x\n", *SizeOfRawData);

		int* PointerToRawData;
		PointerToRawData = (int*)(psection + 20);//4�ֽڣ��ý����ļ��е���ʼ��ַ
		//printf("%x\n", *PointerToRawData);

		int* Characteristics;
		Characteristics = (int*)(psection + 36);//4�ֽڣ��ý����ڴ��е����ԡ�r-w-x
		//printf("%x\n", *Characteristics);

		printf("%s\t%x\t%x\t\t%x\t\t%x\t%x\n", *name, *Misc, *VirtualAddress, *SizeOfRawData, *PointerToRawData, *Characteristics);
	}



	char* tmptr;
	tmptr = fbuffer + 0x1e00;

	ULONGLONG* FuncAddress;

	int NumImportDescriptor;
	NumImportDescriptor = 0;

	for (; ImportDescriptor->Characteristics != 0; ImportDescriptor += 1) {
		//��ʾDLL������
		int NameRVA;
		int NameFOA;
		NameRVA = ImportDescriptor->Name;
		NameFOA = RVA_TO_FOA(fbuffer, NameRVA);
		printf("===========Import DLL ");
		showstring(fbuffer + NameFOA);
		printf("===========\n");

		//������������������临�Ƶ��������С�

		memcpy(tmptr, ImportDescriptor, 20);

		tmptr += 20;

		NumImportDescriptor++;

	}
	//�����޸���չPEͷ�ĵ������ʼ��ַ
	OPE->DataDirectory[1].VirtualAddress = FOA_TO_RVA(fbuffer, tmptr - NumImportDescriptor * 20 - fbuffer);
	
	//һ�ŵ����20���ֽڣ�ԭ�������ŵ����������һ�ŵ�����հ�20��0��һ��80���ֽ�=0x50
	OPE->DataDirectory[1].Size = 0x50;
	
	//ע��!!!������һ�����������������Ҫһ��20���ֽڵ�0������������š�
	memset(tmptr, 0, 100);

	ImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)tmptr;

	/***************************************************************************************************************
	**20�ֽڵ��������� + ��20�ֽ� + 16�ֽ�IMAGE_TRUNK_DATA64 + 16�ֽ�IMAGE_TRUNK_DATA64 + dll���� + function������**
	****************************************************************************************************************/
	
	//����DLL����StudyDLL.dll��function��window
	memcpy(tmptr + 80, dllName, len(dllName));

	memcpy(tmptr + 82 + len(dllName), funcName, len(funcName));

	//�����޸ĵ���������������ַ
	ImportDescriptor->Name = FOA_TO_RVA(fbuffer, tmptr - fbuffer + 80 );

	ImportDescriptor->FirstThunk = FOA_TO_RVA(fbuffer, tmptr - fbuffer + 40);

	ImportDescriptor->OriginalFirstThunk = FOA_TO_RVA(fbuffer, tmptr - fbuffer + 56);

	//������������ַ
	FuncAddress = (ULONGLONG*)(tmptr + 40);

	*FuncAddress = (ULONGLONG)FOA_TO_RVA(fbuffer, tmptr + 82 + len(dllName) - fbuffer);

	FuncAddress = (ULONGLONG*)(tmptr + 56);

	*FuncAddress = (ULONGLONG)FOA_TO_RVA(fbuffer, tmptr + 82 + len(dllName) - fbuffer);

	char finalpath[] = "E:\\Studies\\��ˮ�μ�\\2015-03-31�����ע��\\DLLinjectionmain.exe";

	BufferToFile(fbuffer, finalpath);
}


int main() {
	//���ļ���ȡ���ڴ���
	char fpath[] = "E:\\Studies\\��ˮ�μ�\\2015-03-31�����ע��\\main.exe";
	//char fpath[] = "D:\\IDA_Pro_7.5SP3\\IDA Pro 7.5 SP3(x86, x64, ARM, ARM64, PPC, PPC64, MIPS)\\ida.exe";
	char* fbuffer;
	fbuffer = FileToBuffer(fpath);

	char* nbuffer;
	char sectionname[] = ".new0";
	nbuffer = AddSection(fbuffer, 100, sectionname);

	char DllName[] = "StudyDLL\.dll";
	char funcName[] = "window";
	ImportTableInjection(nbuffer, DllName, funcName);





}
