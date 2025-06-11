//����������Ҫ�Ƕ�ȡnotepad.exe�Ľڱ���Ϣ
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

//C:\Windows\System32\notepad.exe
int main() {
	FILE* pfile;
	long lSize;
	char* buffer;

	pfile = fopen("C:\\Windows\\System32\\notepad.exe", "rb");

	fseek(pfile, 0, SEEK_END);
	lSize = ftell(pfile);

	rewind(pfile);

	buffer = (char*)malloc(lSize);

	if (buffer != NULL) {
		//��exe�ļ���ȡ��buffer��
		fread(buffer, 1, lSize, pfile);
		
		//˼·������DOSͷ��a_lfanewȷ��PEͷ������PEͷ�ĵ�9��WORDȷ��option PE�Ĵ�С���ڱ�ͽ�����option PE����
		int a_lfanew;
		a_lfanew = *((int*)(buffer) + 15);						//���PEͷ�ľ���ƫ����
		printf("DOS -> a_lfanew(4) = 0x%x\t\t\/\/���PEͷƫ����\n", a_lfanew);

		char* PEHeader;
		PEHeader = a_lfanew + buffer;							//����a_lfanew���PEͷ�ľ���λ��
		printf("PE -> Signature(4) = %c%c\t\t\/\/ȷ��PEͷλ��\n", *(PEHeader), *(PEHeader+1));

		short* SizeOfOptionHeader;
		SizeOfOptionHeader = (short*)PEHeader + 10;				//����PEͷƫ��10�����֡��õ�option PE�Ĵ�С0xf0
		printf("PE -> SizeOfOptionHeader(2) = 0x%x\t\/\/ȷ��option PE��С\n", *SizeOfOptionHeader);

		//׼���׶Σ�������ڱ���Ϣ����Ҫ��Ϣ������section���ļ�����ߴ硣����
		short* NumberOfSections;
		NumberOfSections = (short*)(PEHeader + 6);
		printf("PE->NumberOfSections(2) = 0x%x\n", *NumberOfSections);



		
		
		printf("------------------------��ʾ�ڱ�ģ��-----------------------\n");
		printf("����\tԭ��С\t�ڴ��ַ\t�ڿ��С\t��λ��\t������\n");
		char* psection;
		for (int i = 0; i < *NumberOfSections; i++) {

			psection = buffer + a_lfanew + 24 + *SizeOfOptionHeader + i*40;//�ļ�ͷ(buffer) + PEͷ��ƫ����/a_lfanew(0x108) + PE Signature(4) + PE(20) + option PE(0xf0)

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

	}
	else {
		printf("malloc defeat~");
	}

	free(buffer);

	fclose(pfile);

	return 0;
}