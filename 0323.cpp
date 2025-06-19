#include "MyFunctions.h"
#include "MyDLL.h"
#include <Windows.h>







int main() {
	///////////////////////////////////////////////////////////////////////////////////////
	//1、创建一个静态链接库，并在代码中使用.
	/*
	* 1st:reverse项目—>配置—>C++常规—>附件包含目录，即指定.h文件的路径
	* 2ed:reverse项目—>配置—>链接器—>常规—>附件库目录。即指定.lib文件的路径
	* 3rd:reverse项目—>配置—>链接器—>输入—>附件依赖项。即指定静态链接库的文件名
	*/
	add(1, 2);	//静态链接库
	/////////////////////////////////////////////////////////////////////////////////////
	//2、创建一个动态链接库，使用两种方式进行导出(_declspec(dllexport)与.def文件).
	/*
	* 1st:reverse项目—>配置—>C++常规—>附件包含目录，即指定.h文件的路径
	* 2ed:reverse项目—>配置—>链接器—>常规—>附件库目录。即指定.lib文件的路径
	* 3rd:reverse项目—>配置—>链接器—>输入—>附件依赖项。即指定动态链接库的文件名
	*/
	//隐式链接
	// __declspec(dllexport) 或 __declspec(dllimport)
	mydiv(2, 1);
	////////////////////////////////////////////////////////////////////////////////////////
	//3、分别使用隐式链接和显示链接使用一个DLL文件.
	//显示链接
	//不需要加载include头文件
	typedef int (*funcdim)(int, int);
	//1、加载DLL
	HMODULE hDLL = LoadLibraryA("defDLL.dll");
	if (hDLL) {
		funcdim  dim = (funcdim)GetProcAddress(hDLL, "dim");

		dim(3, 1);
	}

	return 0;
}