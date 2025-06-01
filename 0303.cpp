#include <cstdio>
#include <string.h>


//1、创建一个int* arr[5] 数组，并为数组赋值(使用&).

int* arr[5];

void function3_3_1() {
	arr[1] = (int*)2;

	int** x = &arr[0];
	*x = (int*)11;

}

//2、创建一个字符指针数组，存储所有的C的关键词(查资料找)，并全部打印出来.

char* key[32];

void function3_3_2() {
	/*
	数据类型关键字（12个）
	void：声明函数无返回值或无参数，声明无类型指针23。
	char：声明字符型变量或函数34。
	int：声明整型变量或函数34。
	float：声明单精度浮点型变量或函数3。
	double：声明双精度浮点型变量或函数34。
	enum：声明枚举类型4。
	short：声明短整型变量或函数34。
	long：声明长整型变量或函数34。
	signed：声明有符号类型变量或函数34。
	struct：声明结构体变量或函数34。
	union：声明共用体数据类型34。
	unsigned：声明无符号类型变量或函数34。
	*/
	key[0] = (char*)"void";
	key[1] = (char*)"char";
	key[2] = (char*)"int";
	key[3] = (char*)"float";
	key[4] = (char*)"double";
	key[5] = (char*)"enum";
	key[6] = (char*)"short";
	key[7] = (char*)"long";
	key[8] = (char*)"signed";
	key[9] = (char*)"struct";
	key[10] = (char*)"union";
	key[11] = (char*)"unsigned";
	/*
	控制语句关键字（12个）
	if：条件语句。
	else：条件语句的否定分支，与 if 连用。
	switch：开关语句。
	case：开关语句的分支标签。
	default：开关语句中的“其他”分支。
	for：循环语句。
	do：循环语句的循环体4。
	while：循环语句的循环条件。
	break：跳出当前循环或 switch 结构。
	continue：结束当前循环的剩余部分，开始下一轮循环。
	goto：无条件跳转语句。
	return：子程序返回语句。
	*/
	key[12] = (char*)"if";
	key[13] = (char*)"else";
	key[14] = (char*)"switch";
	key[15] = (char*)"case";
	key[16] = (char*)"default";
	key[17] = (char*)"for";
	key[18] = (char*)"do";
	key[19] = (char*)"while";
	key[20] = (char*)"break";
	key[21] = (char*)"continue";
	key[22] = (char*)"goto";
	key[23] = (char*)"return";
	/*
	存储类型关键字（4个）
	auto：声明自动变量。
	extern：声明变量是在其他文件正声明。
	register：声明寄存器变量。
	static：声明静态变量。
	*/
	key[24] = (char*)"auto";
	key[25] = (char*)"extern";
	key[26] = (char*)"register";
	key[27] = (char*)"static";
	/*
	其它关键字（4个）
	const：声明只读变量。
	sizeof：计算数据类型或变量在内存中所占的字节数。
	typedef：用以给数据类型取别名。
	volatile：说明变量在程序执行中可被隐含地改变。
	*/
	key[28] = (char*)"const";
	key[29] = (char*)"sizeof";
	key[30] = (char*)"typedef";
	key[31] = (char*)"volatile";

	for (int x = 0;x < 32;x++) {
		for (int i = 0;*(key[x] + i) != 0;i++) {
			printf("%c", *(key[x] + i));
		}
		printf("\n");
	}
	
	//printf("%c\n", *(key[2]+2));
}


//3、查找这些数据中，有几个id=1 level=8的结构体信息。
char array[100] = {
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x07,0x09,
	0x00,0x20,0x10,0x03,0x03,0x0C,0x00,0x00,0x44,0x00,
	0x00,0x33,0x00,0x47,0x0C,0x0E,0x00,0x0D,0x00,0x11,
	0x00,0x00,0x00,0x02,0x64,0x00,0x00,0x00,0xAA,0x00,
	0x00,0x00,0x64,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x00,0x74,0x0F,0x41,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0A,0x00,
	0x00,0x02,0x57,0x4F,0x57,0x00,0x06,0x08,0x00,0x00,
	0x00,0x00,0x00,0x64,0x00,0x0F,0x00,0x00,0x0D,0x00,
	0x00,0x00,0x23,0x00,0x00,0x64,0x00,0x00,0x64,0x00
};

typedef struct TagPlayer {
	int id;
	int level;
}Player;

int function3_3_3() {
	Player* p;
	//p = (Player*)array;

	for (int i = 0;i < 100;i++) {
		p = (Player*)(array + i);
		if (p->id == 1 && p->level == 8) {
			printf("%d\n", i);
		}
		printf("%x\n", p->id);

	}

	return 0;
}

int main() {

	function3_3_3();

}