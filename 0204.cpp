#include <cstdio>
#include <string.h>
/*
************************20250204************************
1������һ���ṹ��Gamer�����洢һ����Ϸ�еĽ�ɫ����Ϣ������Ѫֵ���ȼ����������Ϣ
(1) ���������Щ��Ϣ�������
(2)����Щ�����������У�����Ҫ��һ����Ա�ǽṹ������

2������һ������������������ṹ�������ֵ.

3������һ��������������ʾ����ṹ����������г�Ա��Ϣ.
*/

//1������һ���ṹ��Gamer�����洢һ����Ϸ�еĽ�ɫ����Ϣ������Ѫֵ���ȼ����������Ϣ

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

//2������һ������������������ṹ�������ֵ.
void function2_4_2(info& Person) {

	Person.n = 100;
	Person.m = 60;
	Person.p.x = 5;
	Person.p.y = 5;

}

//3������һ��������������ʾ����ṹ����������г�Ա��Ϣ.
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