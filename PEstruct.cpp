/***************32λ***************
DOSͷ��
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
} IMAGE_DOS_HEADER,*PIMAGE_DOS_HEADER;


��׼PEͷ + ��չPEͷ = NTͷ
typedef struct _IMAGE_NT_HEADERS{
	DWORD Signature;								//PEͷ��ǣ�ͨ��ֵΪ4550
	IMAGE_FILE_HEADER FileHeader;					//��׼PEͷ 20�ֽڡ�
	IMAGE_OPTIONAL_HEADER32 OptionalHeader32;		//��չPEͷ ��С�ǿɱ�ġ�
}IMAGE_NT_HEADERS32,*PIMAGE_NT_HEADERS32;



��׼PEͷ
typedef struct _IMAGE_FILE_HEADER {
	WORD  Machine;									//2�ֽڣ������ں���CPU�ϣ�0�������⣻014C��interl386���Ժ�8664����x64Ҳ����64λϵͳ�ϣ�
	WORD  NumberOfSections;							//2�ֽڣ�����ڵ�������.data��.text
	DWORD TimeDateStamp;							//4�ֽڣ��ɱ�������д��ʱ��������ļ��Ĵ������޸�ʱ���޹ء�
	DWORD PointerToSymbolTable;						//4�ֽڣ��������
	DWORD NumberOfSymbols;							//4�ֽڣ��������
	WORD  SizeOfOptionalHeader;						//2�ֽ� �ɱ�����չPEͷ�Ĵ�С��32λ��0xE0(224�ֽ�)��64λ��0xF0(240�ֽ�)
	WORD  Characteristics;							//2�ֽڣ��ⲿ�ֿɸ����������λ���Ƿ���1�����жϸ�PE�ļ���������ԡ�
}IMAGE_FILE_HEADER,*PIMAGE_FILE_HEADER;

*--------------Characteristics------------------*
* 0		�ļ��������ض�λ��Ϣ					*
* 1		�ļ��ǿ�ִ�е�							*
* 2		����������Ϣ							*
* 3		�����ڷ�����Ϣ							*
* 4		����������								*
* 5		Ӧ�ó���ɵ���2GB�ĵ�ַ��64λ��			*
* 6		�˱�־����								*
* 7		Сβ��ʽ								*
* 8		ֻ��32λƽ̨������						*
* 9		������������Ϣ							*
* 10	���ܴӿ��ƶ�������						*
* 11	���ܴ���������							*
* 12	ϵͳ�ļ������������򣩣�����ֱ�����С�	*
* 13	����һ��DLL�ļ���						*
* 14	�ļ������ڶദ���������������			*
* 15	��β��ʽ								*
*--------------Characteristics------------------*
//https://blog.csdn.net/qq_51521220/article/details/122045878
��չPEͷ
typedef struct _IMAGE_OPTIONAL_HEADER  {
	WORD  Magic;									//2�ֽڣ���׼ȷʶ����ļ���λ����Ҫ�Ǹó�Աλ����Ϊ0x10B����32λ����0x20B����64λ����
	BYTE  MajorLinkerVersion;						//1�ֽڣ��������汾�š�
	BYTE  MinorLinkerVersion;						//1�ֽڣ��������汾�š�
	DWORD SizeOfCode;								//4�ֽڣ����д���ڵ��ܺͣ��ļ������Ĵ�С��
	DWORD SizeOfInitializedData;					//4�ֽڣ����������Ѿ���ʼ�����ݵĽڵ��ܺͣ��ļ������Ĵ�С��
	DWORD SizeOfUninitializedData;					//4�ֽڣ���������δ��ʼ�����ݵĽڵ��ܺͣ��ļ������Ĵ�С��
	DWORD AddressOfEntryPoint;						//4�ֽڣ��������ڡ�
	DWORD BaseOfCode;								//4�ֽڣ����뿪ʼ�Ļ�ַ��
	DWORD BaseOfData;								//4�ֽڣ����ݿ�ʼ�Ļ�ַ��
	DWORD ImageBase;								//4�ֽڣ��ڴ澵���ַ��ÿ����������ʱ����һ��4GB�������ڴ棬�������ַ���Ǹ�PE�ļ�����4GB�������ʼ��ַ��
	DWORD SectionAlignment;							//4�ֽڣ��ڴ���룬PE�ļ����ڴ�������ʱ��ÿ�������ݵĴ�С�������������Ĵ�С��PE��DOSͷ+PEͷ+�ڱ�Ĵ�СҲ����������
	DWORD FileAlignment;							//4�ֽڣ��ļ����룬PE�ļ��ڴ�����ʱ��ÿ�������ݵĴ�С�������������Ĵ�С��PE��DOSͷ+PEͷ+�ڱ�Ĵ�СҲ������������
	WORD  MajorOperatingSystemVersion;				//2�ֽڣ�����ϵͳ���汾��
	WORD  MinorOperatingSystemVersion;				//2�ֽڣ�����ϵͳ�ΰ汾��
	WORD  MajorImageVersion;						//2�ֽڣ�PE�ļ����汾��
	WORD  MinorImageVersion;						//2�ֽڣ�PE�ļ��ΰ汾��
	WORD  MajorSubsystemVersion;					//2�ֽڣ�����ʱ����ϵͳ���汾��
	WORD  MinorSubsystemVersion;					//2�ֽڣ�����ʱ����ϵͳ�ΰ汾��
	DWORD Win32VersionValue;						//4�ֽڣ���ϵͳ�汾��ֵ������0
	DWORD SizeOfImage;								//4�ֽڣ��ڴ�������PE�ļ��Ĵ�С�����Ա�Ӳ���ϵ��ļ��󣬵�������SectionAlignmentֵ������������������PE���ڴ��д�С
	DWORD SizeOfHeaders;							//
	DWORD CheckSum;
	WORD  Subsystem;
	WORD  DllCharacteristics;
	DWORD SizeOfStackReserve;
	DWORD SizeOfStackCommit;
	DWORD SizeOfHeapReserve;
	DOWRD SizeOfHeapCommit;
	DWORD LoaderFlags;
	DWORD NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
}IMAGE_OPTIONAL_HEADER32,*PIMAGE_OPTINAL_HEADER32;

*/