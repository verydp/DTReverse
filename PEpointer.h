#ifndef PE_POINTER_H
#define PE_POINTER_H

//PEpointer����

int* ptre_lfanew(char* fbuffer) {
	//�Ȼ�ȡ�ڱ���ʼ��ַ����e_lfanew + 24 + SizeOfOptionalHeader
	//e_lfanew
	int* pe_lfanew;
	pe_lfanew = (int*)(fbuffer + 60);
	//int e_lfanew;
	//e_lfanew = *pe_lfanew;
	//printf("e_lfanew = %x\n", e_lfanew);

	return pe_lfanew;
}

char* ptrPE(char* fbuffer) {
	char* ret;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	//printf("%x\n", e_lfanew);
	ret = fbuffer + e_lfanew;

	return ret;
}

char* ptrOptionPE(char* fbuffer) {
	char* ret;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);
	///printf("%x\n", e_lfanew);
	ret = fbuffer + e_lfanew + 24;

	//printf("ptrOptionPE = %x\n", *(ret));

	return ret;

}

int* ptrAddressOfEntryPoint(char* fbuffer) {
	int* pAddressOfEntryPoint;
	pAddressOfEntryPoint = (int*)( ptrOptionPE(fbuffer) + 16 );

	return pAddressOfEntryPoint;
}

int* ptrSizeOfImage(char* fbuffer) {
	int* pSizeOfImage;
	//���Ȼ�ȡimage���ڴ��еĴ�С������չPEͷ����ƫ��56���ֽڵ�SizeOfImage(4byte)
	pSizeOfImage = (int*)(ptrOptionPE(fbuffer) + 56);

	//printf("function in ptrSizeOfImage()%x   %x\n", *((int*)( ptrOptionPE(fbuffer) + 56 )),1);
	//printf("function in ptrSizeOfImage()%x   %x\n", *pSizeOfImage,1);

	return pSizeOfImage;
}

int* ptrSizeOfHeaders(char* fbuffer) {
	//�ȿ���ͷ�ļ�+�ڱ�����ȡ��չPEͷƫ��60���ֽڵ�SizeOfHeaders(4 byte)�����ֵ
	int* pSizeOfHeaders;
	pSizeOfHeaders = (int*)(ptrOptionPE(fbuffer) + 60);
	//int SizeOfHeaders;
	//SizeOfHeaders = *pSizeOfHeaders;
	//printf("SizeOfHeaders = %x\n", SizeOfHeaders);

	return pSizeOfHeaders;
}

short* ptrSizeOfOptionalHeader(char* fbuffer) {
	//PE�����ʼ��ַ����e_lfanew
	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	//SizeOfOptionalHeader
	short* pSizeOfOptionalHeader;
	pSizeOfOptionalHeader = (short*)(fbuffer + e_lfanew + 20);
	//short SizeOfOptionalHeader;
	//SizeOfOptionalHeader = *pSizeOfOptionalHeader;
	//printf("SizeOfOptionalHeader = %x\n", SizeOfOptionalHeader);

	return pSizeOfOptionalHeader;
}

char* ptrSection(char* fbuffer) {
	char* ret;

	ret = fbuffer + *ptre_lfanew(fbuffer) + 24 + *ptrSizeOfOptionalHeader(fbuffer);

	return ret;
}

int SectionOffset(char* fbuffer) {
	int sectionOffset;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	short SizeOfOptionalHeader;
	SizeOfOptionalHeader = *ptrSizeOfOptionalHeader(fbuffer);

	//����������ʽ��ýڱ����ʼ��ַ
	sectionOffset = e_lfanew + 24 + SizeOfOptionalHeader;
	return sectionOffset;

}

int* ptrVirtualAddress(char* ptrSection) {
	int* pVirtualAddress;
	pVirtualAddress = (int*)(ptrSection + 12);

	return pVirtualAddress;
}

int* ptrSizeOfRawData(char* ptrSection) {
	int* pSizeOfRawData;
	pSizeOfRawData = (int*)(ptrSection + 16);

	return pSizeOfRawData;
}

int* ptrPhysicalAddress(char* ptrSection) {
	int* pPhysicalAddress;
	pPhysicalAddress = (int*)(ptrSection + 8);

	return pPhysicalAddress;
}

int* ptrPointerToRawData(char* ptrSection) {
	int* pPointerToRawData;
	pPointerToRawData = (int*)(ptrSection + 20);

	return pPointerToRawData;
}

int* ptrCharacteristics(char* ptrSection) {
	int* pCharacteristics;
	pCharacteristics = (int*)(ptrSection + 36);

	return pCharacteristics;
}

short* ptrNumberOfSection(char* fbuffer) {
	//����PEͷ��ȡ�ڵ���������PEͷƫ��6���ֽڵ�NumberOfSection(4)
	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	short* pNumberOfSection;
	pNumberOfSection = (short*)(fbuffer + e_lfanew + 6);
	//short NumberOfSection;
	//NumberOfSection = *pNumberOfSection;
	//printf("NumberOfSection = %d\n", NumberOfSection);

	return pNumberOfSection;
}

int* ptrImageBase(char* fbuffer) {
	char* pOPE = ptrOptionPE(fbuffer);

	int* pImageBase = (int*)(pOPE + 32);

	return pImageBase;
}


typedef struct _IMAGE_DOS_HEADER {
	WORD e_magic;
	WORD e_cblp;
	WORD e_cp;
	WORD e_crlc;
	WORD e_cparhdr;
	WORD e_minalloc;
	WORD e_maxalloc;
	WORD e_ss;
	WORD e_sp;
	WORD e_csum;
	WORD e_ip;
	WORD e_cs;
	WORD e_lfarlc;
	WORD e_ovno;
	WORD e_res[4];
	WORD e_oemid;
	WORD e_oeminfo;
	WORD e_res2[10];
	LONG e_lfanew;
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;

typedef struct _IMAGE_NT_HEADERS {
	DWORD Signature;							//0		PEͷ��ǣ�ͨ��ֵΪ4550
	IMAGE_FILE_HEADER FileHeader;				//4		��׼PEͷ 20�ֽڡ�
	IMAGE_OPTIONAL_HEADER32 OptionalHeader32;	//24	��չPEͷ ��С�ǿɱ�ġ�
}IMAGE_NT_HEADERS32, * PIMAGE_NT_HEADERS32;

typedef struct _IMAGE_FILE_HEADER {
	WORD  Machine;								//0		�����ں���CPU�ϣ�0�������⣻014C��interl386���Ժ�8664����x64Ҳ����64λϵͳ�ϣ�
	WORD  NumberOfSections;						//2		����ڵ�������.data��.text
	DWORD TimeDateStamp;						//4		�ɱ�������д��ʱ��������ļ��Ĵ������޸�ʱ���޹ء�
	DWORD PointerToSymbolTable;					//8		�������
	DWORD NumberOfSymbols;						//12	�������
	WORD  SizeOfOptionalHeader;					//16	�ɱ�����չPEͷ�Ĵ�С��32λĬ����0xE0(224�ֽ�)��64λĬ����0xF0(240�ֽ�)����С�����Զ���
	WORD  Characteristics;						//18	�ⲿ�ֿɸ����������λ���Ƿ���1�����жϸ�PE�ļ���������ԡ�
}IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;
/*
*--------------Characteristics------------------ *
*0		�ļ��������ض�λ��Ϣ *
*1		�ļ��ǿ�ִ�е� *
*2		����������Ϣ *
*3		�����ڷ�����Ϣ *
*4		���������� *
*5		Ӧ�ó���ɵ���2GB�ĵ�ַ��64λ�� *
*6		�˱�־���� *
*7		Сβ��ʽ *
*8		ֻ��32λƽ̨������ *
*9		������������Ϣ *
*10	���ܴӿ��ƶ������� *
*11	���ܴ��������� *
*12	ϵͳ�ļ������������򣩣�����ֱ�����С� *
*13	����һ��DLL�ļ��� *
*14	�ļ������ڶദ��������������� *
*15	��β��ʽ *
*--------------Characteristics------------------ *
*/

typedef struct _IMAGE_OPTIONAL_HEADER {
	WORD  Magic;								//��׼ȷʶ����ļ���λ����Ҫ�Ǹó�Աλ����Ϊ0x10B����32λ����0x20B����64λ����
	BYTE  MajorLinkerVersion;					//���������汾�š�
	BYTE  MinorLinkerVersion;					//�������ΰ汾�š�
	DWORD SizeOfCode;							//���д���ڵ��ܺͣ��ļ������Ĵ�С��
	DWORD SizeOfInitializedData;				//���������Ѿ���ʼ�����ݵĽڵ��ܺͣ��ļ������Ĵ�С��
	DWORD SizeOfUninitializedData;				//��������δ��ʼ�����ݵĽڵ��ܺͣ��ļ������Ĵ�С��
	DWORD AddressOfEntryPoint;					//�������ڡ�
	DWORD BaseOfCode;							//���뿪ʼ�Ļ�ַ��
	DWORD BaseOfData;							//���ݿ�ʼ�Ļ�ַ���ɸ�
	DWORD ImageBase;							//�ڴ澵���ַ��ÿ����������ʱ����һ��4GB�������ڴ棬�������ַ���Ǹ�PE�ļ�����4GB�������ʼ��ַ��
	DWORD SectionAlignment;						//�ڴ���룬PE�ļ����ڴ�������ʱ��ÿ�������ݵĴ�С�������������Ĵ�С��PE��DOSͷ+PEͷ+�ڱ�Ĵ�СҲ����������
	DWORD FileAlignment;						//�ļ����룬PE�ļ��ڴ�����ʱ��ÿ�������ݵĴ�С�������������Ĵ�С��PE��DOSͷ+PEͷ+�ڱ�Ĵ�СҲ������������
	WORD  MajorOperatingSystemVersion;			//����ϵͳ���汾��
	WORD  MinorOperatingSystemVersion;			//����ϵͳ�ΰ汾��
	WORD  MajorImageVersion;					//PE�ļ����汾��
	WORD  MinorImageVersion;					//PE�ļ��ΰ汾��
	WORD  MajorSubsystemVersion;				//����ʱ����ϵͳ���汾��
	WORD  MinorSubsystemVersion;				//����ʱ����ϵͳ�ΰ汾��
	DWORD Win32VersionValue;					//��ϵͳ�汾��ֵ������0
	DWORD SizeOfImage;							//�ڴ�������PE�ļ��Ĵ�С�����Ա�Ӳ���ϵ��ļ��󣬵�������SectionAlignmentֵ������������������PE���ڴ��д�С
	DWORD SizeOfHeaders;						//����ͷ+�ڱ����ļ������Ĵ�С��������ػ����
	DWORD CheckSum;								//У��ͣ�һЩϵͳ�ļ���Ҫ���ж��ļ��Ƿ��޸ġ�
	WORD  Subsystem;							//��ϵͳ������ó�ԱֵΪ1��������������2����ͼ�ν��棻3���ǿ���̨����DLL��
	WORD  DllCharacteristics;					//�ļ����ԡ���ͨ�����Ʊ�׼PEͷ���ļ����Բ��������
	DWORD SizeOfStackReserve;					//��ʼ��ʱ����ջ�Ĵ�С��
	DWORD SizeOfStackCommit;					//��ʼ��ʱʵ���ύ��ջ��С��
	DWORD SizeOfHeapReserve;					//��ʼ��ʱ�����ĶѴ�С��
	DWORD SizeOfHeapCommit;						//��ʼ��ʱʵ���ύ�ĶѴ�С��
	DWORD LoaderFlags;							//������ء�
	DWORD NumberOfRvaAndSizes;					//Ŀ¼��Ŀ������֪�����ļ�ʹ���˶��ٸ���
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];//�ж����ű������ݵĴ�С���Ƕ��١�
}IMAGE_OPTIONAL_HEADER32, * PIMAGE_OPTINAL_HEADER32;

/*
*--------------DllCharacteristics---------------*
* 0		����������Ϊ��							*
* 1		����������Ϊ��							*
* 2		����������Ϊ��							*
* 3		����������Ϊ��							*
* 4		DLL���ڼ���ʱ���ض�λ					*
* 5		ǿ�ƴ���ʵʩ������						*
* 6		��ӳ�����DEP							*
* 7		���Ը��룬�����������ӳ��			*
* 8		ӳ��ʹ��SEH							*
* 9		����ӳ��								*
* 10	����������Ϊ��							*
* 11	��ӳ��Ϊһ��WDM driver					*
* 12											*
* 13											*
* 14	����������Ϊ��							*
* 15	�������ն˷�����						*
*--------------DllCharacteristics---------------*
*/
#define IMAGE_SIZEOF_SHORT_NAME 8
typedef struct _IMAGE_SECTION_HEADER {
	BYTE NAME[IMAGE_SIZEOF_SHORT_NAME];				//8�ֽ�,�ڱ�����֣�ascii�ַ���ֻ��ȡ8���ֽ�
	union {
		DWORD PhysicalAddress;						//4�ֽڣ����Ǹý���û�ж���ǰ�ĳߴ磬���Բ�׼ȷ��
		DWORD VirtualSize;							//�����干��һ��4�ֽڵ��ڴ�.
	}Misc;
	DWORD VirtualAddress;							//4�ֽ�,�ý����ڴ��е�ƫ����,����ImageBase�������ڴ��еĵ�ַ
	DWORD SizeOfRawData;							//4�ֽ�,�ý����ļ������Ĵ�С.
	DWORD PointerToRawData;							//4�ֽ�,�ý����ļ��е�ƫ����,�������������ļ���λ��.
	DWORD PointerToRelocations;						//4�ֽ�,�������
	DWORD PointerToLinenumbers;						//4�ֽ�,�������
	WORD  NumberOfRelocations;						//4�ֽ�,�������
	WORD  NumberOfLinenumbers;						//4�ֽ�,�������
	DWORD Characteristics;							//4�ֽ�,������,����ж�������.
}IMAGE_SECTION_HEADER, * PIMAGE_SECTION_HEADER;
/*
************************�ڱ�����***********************************************************
* 5		IMAGE_SCN_CODE						//���а�������
* 6		IMAGE_SCN_CNT_INTIALIZED_DATA		//���а����ѳ�ʼ������
* 7		IMAGE_SCN_CUT_UNINITIALIZED_DATA	//���а���δ��ʼ������
* 8		IMAGE_SCN_LNK_OTHER					//����������ʹ��
* 25	IMAGE_SCN_MEM_DISCARDABLE			//���е������ڽ��̿�ʼ�Ժ󽫱���������.reloc
* 26	IMAGE_SCN_MEM_NOT_CACHED			//���е����ݲ��ᾭ������
* 27	IMAGE_SCN_MEM_NOT_PAGED				//���е����ݲ��ᱻ����������
* 28	IMAGE_SCN_MEM_SHARED				//��ʾ���е����ݽ�����ͬ�Ľ���������
* 29	IMAGE_SCN_MEM_EXECUTE				//ӳ�䵽�ڴ���ҳ�������ִ�е�����
* 30	MAGE_SCN_MEM_READ					//ӳ�䵽�ڴ���ҳ������ɶ�����
* 31	IMAGE_SCN_MEM_WRITE					//ӳ�䵽�ڴ���ҳ�������д����
* ******************************************************************************************
*/
#endif // !PE_POINTER_H
#pragma once
