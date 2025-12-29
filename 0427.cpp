#include <Windows.h>
#include <stdio.h>
#include "defDLL.h"
#include <locale.h>
//#include <wchar.h>
/*
| 资源类型宏             | 数值 ID | 含义            |
| ----------------- | ----- | ------------- |
| `RT_CURSOR`       | `1`   | 光标（旧式）        |
| `RT_BITMAP`       | `2`   | 位图            |
| `RT_ICON`         | `3`   | 图标            |
| `RT_MENU`         | `4`   | 菜单            |
| `RT_DIALOG`       | `5`   | 对话框           |
| `RT_STRING`       | `6`   | 字符串表          |
| `RT_FONTDIR`      | `7`   | 字体目录          |
| `RT_FONT`         | `8`   | 字体            |
| `RT_ACCELERATOR`  | `9`   | 加速键           |
| `RT_RCDATA`       | `10`  | 原始二进制数据       |
| `RT_MESSAGETABLE` | `11`  | 消息表           |
| `RT_GROUP_CURSOR` | `12`  | 光标组           |
						13
| `RT_GROUP_ICON`   | `14`  | 图标组           |
						15
| `RT_VERSION`      | `16`  | 版本信息          |
| `RT_DLGINCLUDE`   | `17`  | 对话框 include   |
						18
| `RT_PLUGPLAY`     | `19`  | 即插即用（很少见）     |
| `RT_VXD`          | `20`  | VxD（16/9x 遗留） |
| `RT_ANICURSOR`    | `21`  | 动画光标          |
| `RT_ANIICON`      | `22`  | 动画图标          |
| `RT_HTML`         | `23`  | HTML 资源       |
| `RT_MANIFEST`     | `24`  | 应用清单          |

*/




wchar_t ID00[] = L"光标"; 
wchar_t ID01[] = L"位图"; 
wchar_t ID02[] = L"图标"; 
wchar_t ID03[] = L"菜单"; 
wchar_t ID04[] = L"对话框"; 
wchar_t ID05[] = L"字符串表"; 
wchar_t ID06[] = L"字体目录"; 
wchar_t ID07[] = L"字体"; 
wchar_t ID08[] = L"加速键"; 
wchar_t ID09[] = L"原始二进制数据"; 
wchar_t ID10[] = L"消息表"; 
wchar_t ID11[] = L"光标组"; 
wchar_t ID12[] = L"";
wchar_t ID13[] = L"图标组";
wchar_t ID14[] = L"";
wchar_t ID15[] = L"版本信息";
wchar_t ID16[] = L"对话框";
wchar_t ID17[] = L"";
wchar_t ID18[] = L"即插即用";
wchar_t ID19[] = L"VxD";
wchar_t ID20[] = L"动画光标";
wchar_t ID21[] = L"动画图标";
wchar_t ID22[] = L"HTML";
wchar_t ID23[] = L"应用清单";


wchar_t* ArrayID[24] = {ID00, ID01, ID02,ID03,ID04,ID05,ID06,ID07,ID08,ID09,ID10,ID11,ID12,ID13,ID14,ID15,ID16,ID17,ID18,ID19,ID20,ID21,ID22,ID23 };




VOID showstring(char* string, int length)
{
	for (int i = 0; i < length; i++)
	{
		wprintf(L"%lc", string + i);
	}
}

char* ptrResourceDirectory(char* fbuffer)
{
	IMAGE_OPTIONAL_HEADER64* OPE;
	OPE = (IMAGE_OPTIONAL_HEADER64*)(ptrOptionPE(fbuffer));

	int ResourceRVA;
	ResourceRVA = OPE->DataDirectory[2].VirtualAddress;

	int ResourceFOA;
	ResourceFOA = RVA_TO_FOA(fbuffer, ResourceRVA);

	//printf("ResourceFOA = %x\n", ResourceFOA);

	//获取资源目录
	IMAGE_RESOURCE_DIRECTORY* ptrResourceDirectory;

	ptrResourceDirectory = (IMAGE_RESOURCE_DIRECTORY*)(fbuffer + ResourceFOA);

	return (char*)ptrResourceDirectory;
}


VOID DisplaySubDirectory(IMAGE_RESOURCE_DIRECTORY* ptrResourceDir, char* fbuffer)
{
	//获取该资源目录下面的入口点数量
	int NumberOfIdEntries, NumberOfNameEntries;

	NumberOfIdEntries = ptrResourceDir->NumberOfIdEntries;

	NumberOfNameEntries = ptrResourceDir->NumberOfNamedEntries;

	//获取资源项入口地址
	IMAGE_RESOURCE_DIRECTORY_ENTRY* ptrDirEntry;

	ptrDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(ptrResourceDir + 1);


	unsigned long long* tmp;

	for (int i = 0; NumberOfIdEntries > 0 || NumberOfNameEntries > 0; i++)
	{
		//判断目录入口是名字类型？id类型？




		if (ptrDirEntry->NameIsString == 0)
		{
			wprintf(L"\t====该资源是ID类型   = %ls\t\t============\n", ArrayID[ptrDirEntry->Id - 1]);
			//printf("============该资源是ID类型 = %d\n", ptrDirEntry->Id);

			NumberOfIdEntries--;
		}
		else
		{
			IMAGE_RESOURCE_DIR_STRING_U* ptrNameString;

			ptrNameString = (IMAGE_RESOURCE_DIR_STRING_U*)((char*)ptrResourceDir + ptrDirEntry->NameOffset);

			//printf("ptrDirEntry->NameOffset = %x\n", ptrDirEntry->NameOffset);

			wprintf(L"\t====该资源是Name类型 = %ls\t============\n", ptrNameString->NameString);

			NumberOfNameEntries--;

		}
		//如果目录项指针指向的不是资源目录项 ==> 资源内容
		if (ptrDirEntry->DataIsDirectory == 0)
		{
			IMAGE_RESOURCE_DATA_ENTRY* dataEntry;

			dataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)(ptrDirEntry->OffsetToData + (char*)ptrResourceDir);

			printf("\t%d ptrDirEntry = %x\t OffsetToData = %x\n", i + 1, (char*)ptrDirEntry - fbuffer, ptrDirEntry->OffsetToData);

			printf("\tRVA = %x \tSize = %x\n", dataEntry->OffsetToData, dataEntry->Size);
		}
		else
			//指向的是下一层，目录项
		{
			printf("\t%d ptrDirEntry = %x\t OffsetToDirectory = %x\n", i + 1, (char*)ptrDirEntry - fbuffer, ptrDirEntry->OffsetToDirectory);

			IMAGE_RESOURCE_DIRECTORY* newPtrResourceDirectory;
			newPtrResourceDirectory = (IMAGE_RESOURCE_DIRECTORY*)(ptrDirEntry->OffsetToDirectory + ptrResourceDirectory(fbuffer));
			//printf("OffsetToDirectory = %x\n", ptrDirEntry->OffsetToDirectory);
			//DisplayResourceDirectory(newPtrResourceDirectory, fbuffer);
		}

		ptrDirEntry++;

		tmp = (unsigned long long*)ptrDirEntry;

		if (*tmp == 0)
		{
			break;
		}
	}

}

VOID Space(int x)
{
	for (int i = 0; i < x * 4; i++)
	{
		printf(" ");
	}
}

VOID DisplayResourceDirectory(IMAGE_RESOURCE_DIRECTORY* ptrResourceDir,char* fbuffer,int x)
{
	int t;

	t = x;

	x++;

	IMAGE_RESOURCE_DIRECTORY* ptrBaseResourceDir;

	ptrBaseResourceDir = (IMAGE_RESOURCE_DIRECTORY*)ptrResourceDirectory(fbuffer);

	//获取该资源目录下面的入口点数量
	int NumberOfIdEntries, NumberOfNameEntries;

	NumberOfIdEntries = ptrResourceDir->NumberOfIdEntries;

	NumberOfNameEntries = ptrResourceDir->NumberOfNamedEntries;

	Space(t);

	printf("NumberOfIdEntries = %d NumberOfNameEntries = %d\n", NumberOfIdEntries, NumberOfNameEntries);

	//获取资源项入口地址
	IMAGE_RESOURCE_DIR_STRING_U* ptrNameString;

	IMAGE_RESOURCE_DATA_ENTRY* dataEntry;

	IMAGE_RESOURCE_DIRECTORY_ENTRY* ptrDirEntry;

	ptrDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(ptrResourceDir + 1);

	for (int i = 0; i < NumberOfIdEntries + NumberOfNameEntries ; i++)
	{
		//判断目录入口是名字类型？id类型？
		if (ptrDirEntry->NameIsString == 0)
		{
			Space(t);

			wprintf(L"============该资源是ID类型   = %ls\t\t============\n", ArrayID[ptrDirEntry->Id - 1]);
		}
		else
		{
			ptrNameString = (IMAGE_RESOURCE_DIR_STRING_U*)((char*)ptrBaseResourceDir + ptrDirEntry->NameOffset);

			Space(t);

			wprintf(L"============该资源是Name类型 = %ls============\n",  ptrNameString->NameString);
		}
		//如果目录项指针指向的不是资源目录项 ==> 资源内容
		if (ptrDirEntry->DataIsDirectory == 0)
		{
			dataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)(ptrDirEntry->OffsetToData + (char*)ptrBaseResourceDir);

			Space(t);

			printf("=>%d-%d ptrDirEntry = %x\t OffsetToData = %x\n",t + 1, i + 1, (char*)ptrDirEntry - fbuffer, ptrDirEntry->OffsetToData);

			Space(t);

			printf("=>RVA = %x \tSize = %x\n", dataEntry->OffsetToData, dataEntry->Size);
		}
		else
		//指向的是下一层，目录项
		{
			Space(t);

			printf("=>%d-%d ptrDirEntry = %x\t OffsetToDirectory = %x\n",t + 1, i + 1, (char*)ptrDirEntry - fbuffer, ptrDirEntry->OffsetToDirectory);

			IMAGE_RESOURCE_DIRECTORY* newPtrResourceDirectory;
			
			newPtrResourceDirectory = (IMAGE_RESOURCE_DIRECTORY*)(ptrDirEntry->OffsetToDirectory + (char*)ptrBaseResourceDir);

			DisplayResourceDirectory(newPtrResourceDirectory, fbuffer,x);
		}

		ptrDirEntry++;
	}
}




int main() {
	char* fbuffer;

	char fpath[] = "E:\\Github\\repo\\WinDIY\\x64\\Debug\\WinDIY\.exe";
	
	fbuffer = FileToBuffer(fpath);

	IMAGE_RESOURCE_DIRECTORY* ptrResourceDir;

	ptrResourceDir = (IMAGE_RESOURCE_DIRECTORY*)ptrResourceDirectory(fbuffer);

	setlocale(LC_ALL, "");

	DisplayResourceDirectory(ptrResourceDir, fbuffer,0);

}