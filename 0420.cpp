#include <Windows.h>
#include <stdio.h>
#include <locale.h>
#include <Strsafe.h>


/* C语言中的宽字符和多字符	

  多字符		宽字符										
* char			wchar_t		//多字节字符类型   宽字符类型
							
* printf		wprintf		//打印到控制台函数
							
* strlen		wcslen		//获取长度
							
* strcpy		wcscpy		//字符串复制
							
* strcat		wcscat		//字符串拼接
							
* strcmp		wcscmp		//字符串比较
							
* strstr		wcsstr		//字符串查找

*/

/*
* WinMain 函数 
* int __clrcall WinMain(
  [in]           HINSTANCE hInstance,
  [in, optional] HINSTANCE hPrevInstance,
  [in]           LPSTR     lpCmdLine,
  [in]           int       nShowCmd
);

[in] hInstance

类型：HINSTANCE

应用程序的当前实例的句柄。

****************************************
* 
* [in, optional] hPrevInstance

类型：HINSTANCE

应用程序的上一个实例的句柄。 

****************************************
* [in] lpCmdLine

类型：LPSTR

应用程序的命令行，不包括程序名称。 若要检索整个命令行，请使用 GetCommandLine 函数。

***************************************
* [in] nShowCmd

类型：int

控制窗口的显示方式。 

*/


//1、分别使用wchar_t/wprintf/wcslen/wcscpy/wcscat/wcscmp/wcsstr写一个例子.


int main() {

	const wchar_t* x0 = L"中";	//0x4e2d

	char x1 = '中';				//0xd0 0x00

	wchar_t x2 = '中';			//0xd6d0 0x0000

	char x3[] = "中国";			//0xd6 0xd0 0xb9 0xfa 0x00

	wchar_t x4[8] = L"中国";		//0x4e2d 0x56fd 0x0000

	wchar_t x5[] = L"好人哦";

	wchar_t x6[] = L"好人哦";

	//默认使用英文	告诉编译器，使用控制台默认的编码格式
	//(1) 包含头文件 #include <locale.h>					
	//(2) setlocale(LC_ALL, "");						//使用控制台默认的编码	
	setlocale(LC_ALL, "");
	wprintf(L"x4 = %s\n", x4);						
	wprintf(L"x5 = %s\n", x5);						
												
	printf("wcslen(x4) = %d\n", wcslen(x4));

	wcscpy_s(x5,x4);

	wprintf(L"x5 = %s\n", x5);

	printf("wcslen(x4) = %d\n", wcslen(x4));

	//printf("strlen(x3) = %d\/\/中国\nwcslen(x4) = %d\/\/中国\n", strlen(x3), wcslen(x4));
	//字符拼接函数。
	wcscat_s(x4,x5);

	//StringCchCatA((char*)x4,sizeof(x5), (char*)x5);

	wprintf(L"wcscat_s(x4,x5) : x4 = %s\n", x4);

	if (wcscmp(x5, x6))
	{
		printf("x5 same as x6\n");
	}
	else
	{
		printf("x5 and x6 is different\n");
	}
	wprintf(L"x4 = %s\n", x4);
	wprintf(L"x5 = %s\n", x5);
	wprintf(L"x6 = %s\n", x6);

	TCHAR szContent[] = TEXT("内容");

	TCHAR szTitle[] = TEXT("标题");

	//MessageBox((HWND)0, szContent, szTitle, MB_OK);
	MessageBox((HWND)0, szContent, szTitle, MB_OK);		//errorCode = 0
	DWORD errorCode1 = GetLastError();
	printf("GetLastError() = %d\n", errorCode1);

	MessageBox((HWND)1, 0, 0, MB_OK);		//errorCode = 1400
	DWORD errorCode2 = GetLastError();
	printf("GetLastError() = %d\n", errorCode2);

	return 0;
}

