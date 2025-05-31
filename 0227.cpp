#include <cstdio>
#include <string.h>

/*********************0227********************************/

//1、列出每一行的反汇编代码：
void function2_27_1() {
	char a = 10;
	short b = 20;
	int c = 30;

	char* pa = &a;
	short* pb = &b;
	int* pc = &c;

	char** ppa = &pa;
	short** ppb = &pb;
	int** ppc = &pc;

	printf("%d\n", *(pa));

	printf("---------1---------\n");
}

//2、列出每一行的反汇编代码：
void function2_27_2() {
	int p = 10;

	int******* p7;
	int****** p6;
	int***** p5;
	int**** p4;
	int*** p3;
	int** p2;
	int* p1;

	p1 = &p;
	p2 = &p1;
	p3 = &p2;
	p4 = &p3;
	p5 = &p4;
	p6 = &p5;
	p7 = &p6;

	printf("---------2---------\n");
}

//4、完成代码，实现数组值的互换
void function2_27_3() {
	int arr[5] = { 1,2,3,4,5 };


	for (int i = 4;i >= 0;i--) {
		printf("%d\n", *(arr + i));
	};
}


int main() {
	//data();
	//function2();6
	//function3();
	//function2_3_6();
	//function2_4_2(person);
	//function2_4_3(person);
	//function2_5_3(arr);
	//function2_5_4(arr,2);
	//function2_5_5(5);
	//function2_27_1();
	//function2_27_2();
	//function2_27_3();
	//function2_28();

	return 0;
}