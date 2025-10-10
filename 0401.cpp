#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

//1、设计一个结构体，有两个Int类型的成员X，Y 在结构体内部定义4个函数
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
	//2、观察这些函数调用的时候，与其他的函数调用有哪些不同？
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