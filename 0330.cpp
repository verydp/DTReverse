#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

//D:\\IDA_Pro_7.5SP3\\IDA Pro 7.5 SP3(x86, x64, ARM, ARM64, PPC, PPC64, MIPS)\\ida.exe


//�����ж����ŵ����
//ÿ��ȡһ�ŵ������������ĺ��������ļ���ַ

void BindImportDirectory(char* fbuffer) {

	//��ȡ�ļ��ĵ����
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
		//��ʼ��ȡ����������
		
		//��ʾDLL������
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
		
		//ʱ���Ϊ0��ʾû�а󶨣�ʱ���Ϊ1��ʾ�Ѿ���

		//��������Ҫע��offset��ʾ����dll�͵�һ��BindImportDescriptor��ƫ����������
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
		


		//���������ʱ�����ʾ�Ѿ����˵������ʼ�鿴

	}

	/*
	
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

		IMAGE_THUNK_DATA32* INThunk;
		INThunk = (IMAGE_THUNK_DATA32*)(fbuffer + INTFOA);

		for (; INThunk->u1.ForwarderString != 0; INThunk += 1) {
			//printf("this is new Thunk~\n");

			if (INThunk->u1.ForwarderString >> 31 == 1) {
				//�ж����λ�Ƿ�Ϊ1 ���ʱ ��ô��ȥ���λ��ֵ���Ǻ����ĵ������
				printf("The Ordinal of Function is %x\n", INThunk->u1.ForwarderString - 0x80000000);
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
				//�ж����λ�Ƿ�Ϊ1 ���ʱ ��ô��ȥ���λ��ֵ���Ǻ����ĵ������
				printf("The Ordinal of Function is %x\n", IAThunk->u1.ForwarderString - 0x80000000);
				//printf("INTFOA = %x\n", INTFOA);
			}
			else {
				//������λ����1���������Ժ���������
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
	//���ļ���ȡ���ڴ���
	char fpath[] = "E:\\Studies\\��ˮ�μ�\\2015-03-30�󶨵����\\notepad.exe";
	//char fpath[] = "D:\\IDA_Pro_7.5SP3\\IDA Pro 7.5 SP3(x86, x64, ARM, ARM64, PPC, PPC64, MIPS)\\ida.exe";
	char* fbuffer;
	fbuffer = FileToBuffer(fpath);

	BindImportDirectory(fbuffer);


}