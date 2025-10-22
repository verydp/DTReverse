#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

class Base {
public:
	int x;

	int y;

	Base() {
		this->x = 100;
		this->y = 200;
	}

	void fn1() {
		printf("base:fn1()~\n");
	}
	virtual void fn2() {
		printf("base:virtual fn2()~\n");
	}

	virtual void print() {
		printf("base: %x %x\n", this->x, this->y);
	}
};

class Sub :public Base {
public:
	int A;

	Sub() {
		this->A = 300;
	}

	void fn1() {
		printf("sub:fn1()~\n");
	}

	void fn2() {
		printf("sub:virtual fn2~\n");
	}

	virtual void print() {
		printf("sub1:%x %x %x\n", this->x, this->y, this->A);
	}
};

class Sub1 :public Base {
public:
	int A;

	Sub1() {
		this->A = 300;
	}

	void fn1() {
		printf("sub:fn1()~\n");
	}

	void fn2() {
		printf("sub:virtual fn2~\n");
	}

	virtual void print() {
		printf("sub1:%x %x %x\n", this->x, this->y,this->A);
	}
};

class Sub2 :public Base {
public:
	int B;

	Sub2() {
		this->B = 400;
	}

	void fn1() {
		printf("sub:fn1()~\n");
	}

	void fn2() {
		printf("sub:virtual fn2~\n");
	}

	virtual void print() {
		printf("sub1:%x %x %x\n", this->x, this->y, this->B);
	}
};

void TestBound(Base* base) {
	int n = base->x;

	printf("%x\n", n);

	base->fn1();

	base->fn2();
}

//虚函数依靠虚表实现多态，多态的本质就是实现父类指针可以调用子类的方法
void TestBound1() {

	Base base;

	Sub1 sub1;

	Sub2 sub2;

	Base* pb[3] = { &base, &sub1, &sub2 };

	for (int i = 0; i < 3; i++) {
		pb[i]->print();
	}
}



int main() {
	Base b;

	Sub s;

	TestBound(&s);

	printf("================\n");

	TestBound1();

}