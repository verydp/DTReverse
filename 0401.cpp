#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

//1�����һ���ṹ�壬������Int���͵ĳ�ԱX��Y �ڽṹ���ڲ�����4������
struct Arg2 {
	int x;
	int y;

	int plus(int x, int y) {
		return this->x + this->y;
	}

	int sub(int x, int y) {
		return this->x - this->y;
	}

	int mul(int x, int y) {
		return this->x * this->y;
	}

	int div(int x, int y) {
		return this->x / this->y;
	}
};

int plus1(int x, int y) {
	return x + y;
}

struct Person {
	void fn1() {
		printf("fn1~\n");
	}

	void fn2() {
		printf("fn2~\n");
	}
};

struct Person1 {
	int x;

	void fn1() {
		printf("fn1~\n");
	}

	void fn2() {
		printf("fn2~\n");
	}
};

int main() {
	//2���۲���Щ�������õ�ʱ���������ĺ�����������Щ��ͬ��
	Arg2 arg;

	arg.x = 3;
	arg.y = 1;

	arg.plus(arg.x, arg.y);

	plus1(3, 1);

	Arg2 argg;

	printf("%d\n", sizeof(argg));


	Person* p = NULL;
	p->fn1();
	p->fn2();

	Person1* p1 = NULL;
	p->fn1();
	p->fn2();
}