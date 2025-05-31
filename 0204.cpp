#include <cstdio>
#include <string.h>
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
void function2_4_2(info& Person) {

	Person.n = 100;
	Person.m = 60;
	Person.p.x = 5;
	Person.p.y = 5;

}

//3、定义一个函数，用来显示这个结构体变量的所有成员信息.
void function2_4_3(info& Person) {
	printf("blood--%d\nlevel--%d\nlocation(%d,%d)", Person.n, Person.m, Person.p.x, Person.p.y);
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