#include <cstdio>
#include <string.h>

void data() {
	int x = 1;
	int y = 2;
	int r;
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };

	r = arr[1];
	r = arr[x];
	r = arr[y];
	r = arr[x * 2 + y];

}
//桶排序
void function1() {

	int arr[10] = { 6,3,1,8,7,0,1,2,4,9 };

	int retarr[10] = { 0 };

	int finnalarr[10] = { 0 };

	for (int x = 0 ; x < 10 ; x++) {

		retarr[arr[x]]++;

	}

	for (int y = 0;y < 10;y++) {
		//printf("%d\n",retarr[y]);

		if (retarr[y] != 0) {
			for (retarr[y];retarr[y] > 0;retarr[y]--) {
				printf("%d\n", y);
			}
		}

	}
}


//冒泡排序
void function2() {
	int x = 0;
	int y = 0;
	int k = 0;
	int arr[10] = { 6,3,1,8,7,0,1,2,4,9 };

	while (y < 10) {

		x = 0;
		while (x < 9-y) {

			if (arr[x] > arr[x + 1]) {

				arr[x] = arr[x] + arr[x + 1];
				arr[x + 1] = arr[x] - arr[x + 1];
				arr[x] = arr[x] - arr[x + 1];
			}

			x++;
		}

		k = 0;
		while (k < 10) {
			printf("%d\n", arr[k]);
			k++;
		}

		printf("-------------------\n");

		y++;
	}
		
		/*
		x = 0;

		while (x < 9) {

			if (arr[x] > arr[x + 1]) {

				arr[x] = arr[x] + arr[x + 1];
				arr[x + 1] = arr[x] - arr[x + 1];
				arr[x] = arr[x] - arr[x + 1];
			}

			x++;
		}
		k = 0;
		while (k < 10) {
			printf("%d\n", arr[k]);
			k++;
		}
		printf("-------------------\n");

		*/
		/*
		while (y < 10) {
		while (k < 10) {
			printf("%d\n", arr[k]);
			k++;
		}

		printf("-------------------");
		y++;
		*/

	}


void function3() {
	char arr1[3] = { 1,2,3 };
	//char arr2[4] = { 1,2,3,4 };
	//char arr3[5] = { 1,2,3,4,5 };
}

void function2_3() {

	//假设现在有5个班，每个班10个人，设计一个二维数组存储这些人的年龄.
	int arr[5][10] = {
		{1,2,3,4,5,6,7,8,9,10},
		{11,12,13,14,15,16,17,28,19,20},
		{21,22,23,24,25,26,27,28,29,30},
		{31,32,33,34,35,36,37,38,39,40},
		{41,42,43,44,45,46,47,48,49,50},
	};

	//如果想知道第二个班的第6个人的年龄，应该如何获取？编译器该如何获取？
	printf("%d\n", arr[1][5]);

	printf("---------打印所有班级，所有学生的年龄(每个班级打印一行).---------\n");

	//打印所有班级，所有学生的年龄(每个班级打印一行).
	for (int i = 0;i < 5;i++) {
		for (int x = 0;x < 10;x++) {
			printf("%d ", arr[i][x]);
		}
		printf("\n");
	}

	printf("---------将第二个班级的超过20岁的学生的年龄修改为21岁.---------\n");

	//将第二个班级的超过20岁的学生的年龄修改为21岁.
	for (int i = 0;i < 10;i++) {
		if (arr[1][i] > 20) { arr[1][i] = 21; }
	}

	for (int i = 0;i < 5;i++) {
		for (int x = 0;x < 10;x++) {
			printf("%d ", arr[i][x]);
		}
		printf("\n");
	}

	printf("----------//打印出每个班级学生的年龄的和.--------\n");

	//打印出每个班级学生的年龄的和.

	for (int i = 0;i < 5;i++) {
		int k = 0;
		for (int x = 0;x < 10;x++) {
			k += arr[i][x];
		}
		printf("%d------%d\n", i, k);
	}

}

//归并排序
void function2_3_6() {
	/*
	* 前提：两个数组是已经各自从小到大排序好了的。
	数组一：[3,5,7,9,12,25,34,55]
	数组二：[4,7,9,11,13,16]
	将两个数组中所有数据进行从小到大的排序，存储到另一个数组中.
	思路：每次从各自数组摘出最小的值，放入到新的数组中。让数小的数组的下一个值和
	i=0-->3
	k=0-->4
	4>3
	{3}
	i++-->5
	k=0-->4
	5>4
	*/

	int arr1[8] = { 3,5,7,9,12,25,34,55 };
	int arr2[6] = { 4,7,9,11,13,16 };
	int arr3[14] = {};

	int x = 0;
	int y = 0;

	while ( x+y<14 ) {
		if (arr1[x] > arr2[y] && y<6) {
			arr3[x + y]=arr2[y];
			y++;
		}
		else
		{
			arr3[x + y] = arr1[x];
			x++;
		}
	}


	for (int i = 0;i < 14;i++) {
		printf("%d ", arr3[i]);
	}


}

/*
************************20250204************************
1、定义一个结构体Gamer用来存储一个游戏中的角色的信息，包括血值、等级、坐标等信息
(1) 具体包含哪些信息自由设计
(2)但这些包含的类型中，必须要有一个成员是结构体类型

2、定义一个函数，用来给这个结构体变量赋值.

3、定义一个函数，用来显示这个结构体变量的所有成员信息.
*/

//1、定义一个结构体Gamer用来存储一个游戏中的角色的信息，包括血值、等级、坐标等信息

struct Point
{
	int x;
	int y;
};

struct info
{
	int n;
	int m;
	Point p;
};

info person;

//2、定义一个函数，用来给这个结构体变量赋值.
void function2_4_2(info &Person) {

	Person.n = 100;
	Person.m = 60;
	Person.p.x = 5;
	Person.p.y = 5;

}

//3、定义一个函数，用来显示这个结构体变量的所有成员信息.
void function2_4_3(info &Person) {
	printf("blood--%d\nlevel--%d\nlocation(%d,%d)", Person.n, Person.m, Person.p.x, Person.p.y);
}


/*********************0205************************
1、定义一个结构体Monster，能够存储怪的各种信息(至少有一个成员是结构体类型)。
2、声明一个Monster类型的数组，长度为10.
3、编写一个函数，为第二题中的数组赋值.
4、编写一个函数，能够通过怪物ID，打印当前这个怪物的所有信息.
*/

//1、定义一个结构体Monster，能够存储怪的各种信息(至少有一个成员是结构体类型)。

struct condition 
{
	int blood;
	char home[10];
};

typedef struct Monster {
	char sex;
	int level;
	int id;
	condition con;
}Mon;

//2、声明一个Monster类型的数组，长度为10.

Mon arr[10];


//3、编写一个函数，为第二题中的数组赋值.
void function2_5_3(Mon array[]) {
	for (int i = 0;i < 10;i++) {

		array[i].id = i;
		array[i].level = i;
		array[i].sex = 1;
		array[i].con.blood = 100;
		strcpy_s(array[i].con.home, "china\0");
	}

	printf("okok\n");
}

//4、编写一个函数，能够通过怪物ID，打印当前这个怪物的所有信息.
void function2_5_4(Mon array[], int i) {
	printf("id-->%d\nlevel-->%d\nsex-->%d\n|-blood-->%d\n|-home-->%s\n",
		array[i].id, array[i].level, array[i].sex, array[i].con.blood, array[i].con.home);
}

//5、分析下面结构体的内存分配：
struct S1
{
	char c;//1
	double i;//8
};//结构对齐16

S1 s1;

struct S2
{
	int c1;//4
	char c2[10];//10
};//16
S2 s2;

struct S3
{
	char c1;//1
	S1 s1;//8
	char c2;//1
	char c3;//1
};//32
S3 s3;
struct S4
{
	char c1;//1
	S1 s1;//8
	char c2;//1
	double d1;//8
};//40
S4 s4;

/*
	printf("%d\n", sizeof(S1));
	printf("%d\n", sizeof(S2));
	printf("%d\n", sizeof(S3));
	printf("%d\n", sizeof(S4));
	printf("-------------------------\n");
	s1.c = 1;
	s1.i = 1.0f;

	s2.c1 = 1;
	s2.c2[0] = 0;
	s2.c2[1] = 1;
	s2.c2[2] = 2;
	s2.c2[3] = 3;
	s2.c2[4] = 4;
	s2.c2[5] = 5;
	s2.c2[6] = 6;
	s2.c2[7] = 7;
	s2.c2[8] = 8;
	s2.c2[9] = 9;


	s3.c1 = 1;
	s3.c2 = 2;
	s3.c3 = 3;
	s3.s1.c = 1;
	s3.s1.i = 1.0f;

	s4.c1 = 1;
	s4.c2 = 1;
	s4.d1 = 1.0f;
	s4.s1.c = 1;
	s4.s1.i = 2.0f;
*/

//递归函数（尝试用递归函数将一个十进制的数转换成二进制的数）

int function2_5_5(int x) {
	if (x == 1) {
		return 0;
	}
	else
	{
		x--;
		function2_5_5(x);
	} 

}

/*************************0206***********************
1、写一个switch语句，不生产大表也不生产小表，贴出对应的反汇编.
2、写一个switch语句，只生成大表,贴出对应的反汇编.
3、写一个switch语句，生成大表和小表,贴出对应的反汇编.
4、为do..while语句生成的反汇编填写注释.
5、为while语句生成的反汇编填写注释.
6、为for语句生成的反汇编填写注释.
*/

//1、写一个switch语句，不生产大表也不生产小表，贴出对应的反汇编.
void function2_6_1(int x) {
	switch (x)
	{
	case 1:
		printf("a");
		break;

	case 2:
		printf("b");
		break;

	case 3:
		printf("c");
		break;
	default:
		printf("error");
		break;
	}
}

//2、写一个switch语句，只生成大表,贴出对应的反汇编.
void function2_6_2(int x) {
	switch (x)
	{
	case 1:
		printf("a");
		break;

	case 2:
		printf("b");
		break;

	case 3:
		printf("c");
		break;
	case 4:
		printf("d");
		break;
	case 5:
		printf("e");
		break;
	case 6:
		printf("f");
		break;
	case 7:
		printf("g");
		break;
	case 8:
		printf("h");
		break;
	default:
		break;
	}
}

//3、写一个switch语句，生成大表和小表,贴出对应的反汇编.
void function2_6_3(int x) {
	switch (x)
	{
		case 1:
			printf("a");
			break;

		case 2:
			printf("b");
			break;


		case 4:
			printf("d");
			break;
		case 5:
			printf("e");
			break;
		case 6:
			printf("f");
			break;
		case 7:
			printf("g");
			break;
		case 8:
			printf("h");
			break;
	default:
		break;
	}
}

//4、为do..while语句生成的反汇编填写注释.
void function2_6_4(int x,int y) {
	do
	{
		x++;
	} while (x>y);
}

//5、为while语句生成的反汇编填写注释.
void function2_6_5(int x, int y) {
	while (x < y) {
		x++;
	}
}

//6、为for语句生成的反汇编填写注释.
void function2_6_6(int y) {
	int i = 0;
	for (i;i < y;i++) {
		printf("okok");
	}
}

/************************0226*************************
* 1、char类型占几字节？char*类型占几字节？int*****占几字节？
* 2、char** arr[10] 占多少个字节？
* 
*/

//1、char类型占几字节？char*类型占几字节？int*****占几字节？.
void function2_26_1() {
	char x;
	char* y;
	int***** z;

	x = 1;//1byte
	y = (char*)2;//qword=8byte--64bit
	z = (int*****)3;//qword=8byte--64bit

}

//2、char** arr[10] 占多少个字节？
void function2_26_2() {
	char** arr1[10];

	char arr2[10] = {0};//1byte*10=10byte

	char* arr3[10];//8byte*10=80byte

	arr3[0] = (char*)0;//strart + 0*8
	arr3[9] = (char*)9;//strart + 9*8


	arr1[0] = (char**)1;//strart + 0*8
	arr1[9] = (char**)9;//strart + 9*8
}

struct Student {
	int x;
	int y;
};

//3、自定义结构体
void function2_26_3() {
	/*
	Student**** s;
	s = (Student****)100;
	s++;//108
	s = s + 2;//124
	s = s - 3;//100
	*/
	int x;
	Student**** s1;
	Student**** s2;
	s1 = (Student****)100;
	s2 = (Student****)200;
	x = s2 - s1;

}

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
	function2_27_1();
	function2_27_2();
	function2_27_3();

	return 0;
}