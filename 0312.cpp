#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

int main() {
	FILE* pfile;
	long lSize;
	char* buffer;

	pfile = fopen("F:\\chap01\\1.3 Windows����ϵͳ\\1.3.1  Win32 API����\\MessageBox\\MessageBoxA\\MessageBoxA.exe", "rb");

	fseek(pfile, 0, SEEK_END);
	lSize = ftell(pfile);
	rewind(pfile);

	//printf("%d\n", lSize);

	buffer = (char*)malloc(lSize);

	if (buffer != NULL) {
		fread(buffer, 1, lSize, pfile);
		/********************�˴���ʼ���Խ���32λPE�ļ��ṹ************************/
		//������������ָ�룬1�ֽڡ�2�ֽڡ�4�ֽڡ�
		char* w1;
		short* w2;
		int* w4;

		//DOSͷ��������Ҫ�ֶ�WORD e_magic(0byte)(short)��LONG e_lfanew(36byte)int ;
		printf("-------------DOSͷ---------------\n");
		printf("e_magic(2)\t\t%c%c\n", *(buffer), *(buffer + 1));

		w4 = (int*)buffer;
		printf("e_lfanew(4)\t\t%x\n", *(w4 + 15));

		//���ݻ�ȡ����d0������֪��NTͷ��ʼ�ĵط���0xd0�������������ntͷ��
		char* nt;
		nt = buffer + *(w4 + 15);	//ntͷ��ʼλ�ã����ԴӸñ��ʽ��ȡ(bufferΪ�ļ���ʼ��ַ)
		printf("-------------NTͷ----------------\n");
		printf("Signature(2)\t\t%c%c\n", *(nt), *(nt + 1));
		printf("\n");
		//��ȡ��NTͷ����Կ�ʼ������׼PEͷ��
		char* pe;
		pe = nt + 4;
		w2 = (short*)pe;
		printf("-------------PEͷ----------------\n");
		printf("Machine(2)\t\t%x\t\\\\�����ں���CPU�ϣ�0�������⣻014C��interl386���Ժ�8664����x64Ҳ����64λϵͳ�ϣ�\n", *(w2));
		printf("NumberOfSections(2)\t%x\t\\\\����ڵ�������.data��.text\n", *(w2 + 1));
		printf("SizeOfOptionalHeader(2)\t%x\t\\\\�ɱ�����չPEͷ�Ĵ�С��32λĬ����0xE0(224�ֽ�)��64λĬ����0xF0(240�ֽ�)����С�����Զ���\n", *(w2 + 8));
		printf("\n");
		//PEͷ�ǹ̶���20�ֽڣ��������Ϳ��Խ�����չPEͷ���ļ���
		char* ope;
		ope = nt + 24;				//optional PE��ʼ��λ��(��NTͷƫ��24�ֽ�)
		w2 = (short*)ope;
		w4 = (int*)ope;
		printf("----------Optional PEͷ----------\n");
		printf("Magic(2)\t\t%x\t\\\\0x10B����32λ����0x20B����64λ����\n", *w2);
		printf("AddressOfEntryPoint(4)\t%x\t\\\\��������\n", *(w4 + 4));
		printf("BaseOfCode(4)\t\t%x\t\\\\���뿪ʼ�Ļ�ַ\n", *(w4 + 5));
		printf("BaseOfData(4)\t\t%x\t\\\\���ݿ�ʼ�Ļ�ַ\n", *(w4 + 6));
		printf("ImageBase(4)\t\t%x\t\\\\�ڴ�ӳ���ַ\n", *(w4 + 7));
		printf("SectionAlignment(4)\t%x\t\\\\�ڴ���룬PE�ļ����ڴ�������ʱ��ÿ�������ݵĴ�С�������������Ĵ�С\n", *(w4 + 8));
		printf("FileAlignment(4)\t%x\t\\\\�ļ����룬PE�ļ��ڴ�����ʱ��ÿ�������ݵĴ�С�������������Ĵ�С\n", *(w4 + 9));
		printf("SizeOfImage(4)\t\t%x\t\\\\�ڴ���PE�ļ��Ĵ�С�����Ա�Ӳ���ϵ��ļ��󣬵�������SectionAlignmentֵ��������\n", *(w4 + 14));
		printf("SizeOfHeaders(4)\t%x\t\\\\����ͷ+�ڱ���FileAlignment�����Ĵ�С��������ػ����\n", *(w4 + 15));
		printf("SizeOfStackReserve(4)\t%x\t\\\\��ʼ��ʱ����ջ�Ĵ�С\n", *(w4 + 18));
		printf("SizeOfStackCommit(4)\t%x\t\\\\��ʼ��ʱʵ���ύ��ջ��С\n", *(w4 + 19));
		printf("SizeOfHeapReserve(4)\t%x\t\\\\��ʼ��ʱ�����ĶѴ�С\n", *(w4 + 20));
		printf("SizeOfHeapCommit(4)\t%x\t\\\\��ʼ��ʱʵ���ύ�ĶѴ�С\n", *(w4 + 21));





	}
	else {
		printf("malloc defeat~");
	}


	free(buffer);

	fclose(pfile);
	return 0;
}