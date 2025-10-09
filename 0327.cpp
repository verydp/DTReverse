#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>


//D:\\IDA_Pro_7.5SP3\\IDA Pro 7.5 SP3(x86, x64, ARM, ARM64, PPC, PPC64, MIPS)\\ida.exe


//�����ж����ŵ����
//ÿ��ȡһ�ŵ������������ĺ��������ļ���ַ

void ImportDirectory(char* fbuffer){

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

	//�ж��Ƿ���ڵ���������
	for (; ImportDescriptor->Characteristics != 0; ImportDescriptor += 1) {
		printf("###########ImportDescriptorFOA = %x###########\n", (char*)ImportDescriptor - fbuffer);

		//��ʼ��ȡ����������

		//��ʾDLL������
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

		IMAGE_THUNK_DATA64* INThunk;
		INThunk = (IMAGE_THUNK_DATA64*)(fbuffer + INTFOA);

		for (;INThunk->u1.ForwarderString != 0; INThunk += 1) {
			//printf("this is new Thunk~\n");

			if (INThunk->u1.ForwarderString >> 63 == 1) {
				//�ж����λ�Ƿ�Ϊ1,������λΪ1 ��ô��ȥ���λ��ֵ���Ǻ����ĵ������
				printf("The Ordinal of Function is %x\n", INThunk->u1.ForwarderString - 0x8000000000000000);
				//printf("INTFOA = %x\n", INTFOA);
			}
			else {
				//������λ����1���������Ժ���������
				ULONGLONG INThunkNameRVA;
				ULONGLONG INThunkNameFOA;
				INThunkNameRVA = INThunk->u1.AddressOfData;
				INThunkNameFOA = RVA_TO_FOA(fbuffer, INThunk->u1.AddressOfData);

				//printf("The address of data import by name %x\n", INThunkNameFOA);


				IMAGE_IMPORT_BY_NAME* FunctionName;
				FunctionName = (IMAGE_IMPORT_BY_NAME*)(fbuffer + INThunkNameFOA);
				
				showstring(FunctionName->Name);
				printf("\n");
			}
		}

		




		printf("///////////////////////////////////////////////////////\n");



		int IATRVA;
		int IATFOA;
		IATRVA = ImportDescriptor->FirstThunk;
		IATFOA = RVA_TO_FOA(fbuffer, IATRVA);
		printf("IAThunkFOA = %x\n", IATFOA);

		IMAGE_THUNK_DATA64* IAThunk;
		IAThunk = (IMAGE_THUNK_DATA64*)(fbuffer + IATFOA);

		for (; IAThunk->u1.ForwarderString != 0; IAThunk += 1) {
			//printf("this is new Thunk~\n");

			if (IAThunk->u1.ForwarderString >> 63 == 1) {
				//�ж����λ�Ƿ�Ϊ1 ���ʱ ��ô��ȥ���λ��ֵ���Ǻ����ĵ������
				printf("The Ordinal of Function is %x\n", IAThunk->u1.ForwarderString - 0x8000000000000000);
				//printf("INTFOA = %x\n", INTFOA);
			}
			else {
				//������λ����1���������Ժ���������
				ULONGLONG IAThunkNameRVA;
				ULONGLONG IAThunkNameFOA;
				IAThunkNameRVA = INThunk->u1.AddressOfData;
				IAThunkNameFOA = RVA_TO_FOA(fbuffer, IAThunk->u1.AddressOfData);

				//printf("The address of data import by name %x\n", INThunkNameFOA);


				IMAGE_IMPORT_BY_NAME* FunctionName;
				FunctionName = (IMAGE_IMPORT_BY_NAME*)(fbuffer + IAThunkNameFOA);

				showstring(FunctionName->Name);
				printf("\n");
			}
		}


	}







}

int main() {
	//���ļ���ȡ���ڴ���
	char fpath[] = "D:\\7-Zip\\7z.exe";
	//char fpath[] = "D:\\IDA_Pro_7.5SP3\\IDA Pro 7.5 SP3(x86, x64, ARM, ARM64, PPC, PPC64, MIPS)\\ida.exe";
	char* fbuffer;
	fbuffer = FileToBuffer(fpath);

	ImportDirectory(fbuffer);


}