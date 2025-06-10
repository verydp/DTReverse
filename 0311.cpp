/***************32λ***************
DOSͷ��
typedef struct _IMAGE_DOS_HEADER {
****WORD e_magic;
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
****LONG e_lfanew;
} IMAGE_DOS_HEADER,*PIMAGE_DOS_HEADER;


��׼PEͷ + ��չPEͷ = NTͷ
typedef struct _IMAGE_NT_HEADERS{
	****DWORD Signature;							//PEͷ��ǣ�ͨ��ֵΪ4550
		IMAGE_FILE_HEADER FileHeader;				//��׼PEͷ 20�ֽڡ�
		IMAGE_OPTIONAL_HEADER32 OptionalHeader32;	//��չPEͷ ��С�ǿɱ�ġ�
}IMAGE_NT_HEADERS32,*PIMAGE_NT_HEADERS32;



��׼PEͷ��20�ֽڣ�
typedef struct _IMAGE_FILE_HEADER {
	****WORD  Machine;								//2�ֽڣ������ں���CPU�ϣ�0�������⣻014C��interl386���Ժ�8664����x64Ҳ����64λϵͳ�ϣ�
	****WORD  NumberOfSections;						//2�ֽڣ�����ڵ�������.data��.text
		DWORD TimeDateStamp;						//4�ֽڣ��ɱ�������д��ʱ��������ļ��Ĵ������޸�ʱ���޹ء�
		DWORD PointerToSymbolTable;					//4�ֽڣ��������
		DWORD NumberOfSymbols;						//4�ֽڣ��������
	****WORD  SizeOfOptionalHeader;					//2�ֽ� �ɱ�����չPEͷ�Ĵ�С��32λĬ����0xE0(224�ֽ�)��64λĬ����0xF0(240�ֽ�)����С�����Զ���
		WORD  Characteristics;						//2�ֽڣ��ⲿ�ֿɸ����������λ���Ƿ���1�����жϸ�PE�ļ���������ԡ�
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
	****WORD  Magic;								//2�ֽڣ���׼ȷʶ����ļ���λ����Ҫ�Ǹó�Աλ����Ϊ0x10B����32λ����0x20B����64λ����
		BYTE  MajorLinkerVersion;					//1�ֽڣ����������汾�š�
		BYTE  MinorLinkerVersion;					//1�ֽڣ��������ΰ汾�š�
		DWORD SizeOfCode;							//4�ֽڣ����д���ڵ��ܺͣ��ļ������Ĵ�С��
		DWORD SizeOfInitializedData;				//4�ֽڣ����������Ѿ���ʼ�����ݵĽڵ��ܺͣ��ļ������Ĵ�С��
		DWORD SizeOfUninitializedData;				//4�ֽڣ���������δ��ʼ�����ݵĽڵ��ܺͣ��ļ������Ĵ�С��
	****DWORD AddressOfEntryPoint;					//4�ֽڣ��������ڡ�
	****DWORD BaseOfCode;							//4�ֽڣ����뿪ʼ�Ļ�ַ��
	****DWORD BaseOfData;							//4�ֽڣ����ݿ�ʼ�Ļ�ַ���ɸ�
	****DWORD ImageBase;							//4�ֽڣ��ڴ澵���ַ��ÿ����������ʱ����һ��4GB�������ڴ棬�������ַ���Ǹ�PE�ļ�����4GB�������ʼ��ַ��
		DWORD SectionAlignment;						//4�ֽڣ��ڴ���룬PE�ļ����ڴ�������ʱ��ÿ�������ݵĴ�С�������������Ĵ�С��PE��DOSͷ+PEͷ+�ڱ�Ĵ�СҲ����������
		DWORD FileAlignment;						//4�ֽڣ��ļ����룬PE�ļ��ڴ�����ʱ��ÿ�������ݵĴ�С�������������Ĵ�С��PE��DOSͷ+PEͷ+�ڱ�Ĵ�СҲ������������
		WORD  MajorOperatingSystemVersion;			//2�ֽڣ�����ϵͳ���汾��
		WORD  MinorOperatingSystemVersion;			//2�ֽڣ�����ϵͳ�ΰ汾��
		WORD  MajorImageVersion;					//2�ֽڣ�PE�ļ����汾��
		WORD  MinorImageVersion;					//2�ֽڣ�PE�ļ��ΰ汾��
		WORD  MajorSubsystemVersion;				//2�ֽڣ�����ʱ����ϵͳ���汾��
		WORD  MinorSubsystemVersion;				//2�ֽڣ�����ʱ����ϵͳ�ΰ汾��
		DWORD Win32VersionValue;					//4�ֽڣ���ϵͳ�汾��ֵ������0
	****DWORD SizeOfImage;							//4�ֽڣ��ڴ�������PE�ļ��Ĵ�С�����Ա�Ӳ���ϵ��ļ��󣬵�������SectionAlignmentֵ������������������PE���ڴ��д�С
	****DWORD SizeOfHeaders;						//4�ֽڣ�����ͷ+�ڱ����ļ������Ĵ�С��������ػ����
		DWORD CheckSum;								//4�ֽڣ�У��ͣ�һЩϵͳ�ļ���Ҫ���ж��ļ��Ƿ��޸ġ�
		WORD  Subsystem;							//4�ֽڣ���ϵͳ������ó�ԱֵΪ1��������������2����ͼ�ν��棻3���ǿ���̨����DLL��
		WORD  DllCharacteristics;					//2�ֽڣ��ļ����ԡ���ͨ�����Ʊ�׼PEͷ���ļ����Բ��������
	****DWORD SizeOfStackReserve;					//4�ֽڣ���ʼ��ʱ����ջ�Ĵ�С��
	****DWORD SizeOfStackCommit;					//4�ֽڣ���ʼ��ʱʵ���ύ��ջ��С��
	****DWORD SizeOfHeapReserve;					//4�ֽڣ���ʼ��ʱ�����ĶѴ�С��
	****DOWRD SizeOfHeapCommit;						//4�ֽڣ���ʼ��ʱʵ���ύ�ĶѴ�С��
		DWORD LoaderFlags;							//4�ֽڣ�������ء�
		DWORD NumberOfRvaAndSizes;					//4�ֽڣ�Ŀ¼��Ŀ������֪�����ļ�ʹ���˶��ٸ���
		IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];//�ж����ű������ݵĴ�С���Ƕ��١�
}IMAGE_OPTIONAL_HEADER32,*PIMAGE_OPTINAL_HEADER32;

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

�ڱ�
��PE�ļ��нڱ���һ���ṹ���飬Ҳ�����ж���ڱ��ɣ��ⲿ�����ݰ����ļ�����Ĵ�С�洢������ͨ��FILEAlignment(line 78)�鿴��
�Ҹýṹ��ĳ�Ա����Ǳ�ʶ�����ݵ�������ԣ��������ļ������ڴ��е���ʼ��ַ�����ļ��гߴ硣
*/