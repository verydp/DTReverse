#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

class Base {
public:
	virtual void fn1() {
		printf("Base:fn1~\n");
	}

	virtual void fn2() {
		printf("Base:fn2~\n");
	}	

	virtual void fn3() {
		printf("Base:fn3~\n");
	}

};

class Sub:public Base {
public:
	virtual void fn4() {
		printf("Base:fn4~\n");
	}

	virtual void fn5() {
		printf("Base:fn5~\n");
	}

	virtual void fn6() {
		printf("Base:fn6~\n");
	}

};

class Base1 {
public:
	virtual void fn1() {
		printf("Base1:fn1~\n");
	}

	virtual void fn2() {
		printf("Base1:fn2~\n");
	}

	virtual void fn3() {
		printf("Base1:fn3~\n");
	}
};

class Sub1:public Base1{
public:
	virtual void fn1() {
		printf("Sub1:fn1~\n");
	}

	virtual void fn2() {
		printf("Sub1:fn2~\n");
	}

	virtual void fn6() {
		printf("Sub1:fn6~\n");
	}
};

void test1() {
	Sub sub;

	Sub* ps;

	//类对象的指针地址，也即虚函数表的指针地址。此处sub为类对象，ps为类指针
	ps = &sub;

	//取出虚函数表的地址

	unsigned long long vaddrsheet;

	vaddrsheet = *(unsigned long long*)ps;

	//根据虚函数表的地址，取出虚函数表的函数地址。

	unsigned long long* pvaddrsheet;

	pvaddrsheet = (unsigned long long*)vaddrsheet;

	//定义一个指针函数,
	void (*pfn)();

	for (int i = 0;i< 6; i++) {

		pfn = (void (*)()) * (pvaddrsheet + i);

		pfn();
		
	}


	/*
//写法一：声明了一种“返回值为void 无参数的函数类型 名叫pfunc。”的函数类型(pfunc代表的是一种函数类型)
typedef void pfunc();

pfunc* pf;

pf = (pfunc*)*(unsigned long long*)(*(unsigned long long*)ps);

pf();

//写法二：声明了一个“指针”函数，返回值为void，函数“指针名”为pfn。
void (*pfn)();

pfn = (void (*)())*(unsigned long long*)(*(unsigned long long*)ps);

pfn();
*/
}

void test2() {
	Sub1 sub;

	Sub1* ps;

	//类对象的指针地址，也即虚函数表的指针地址。此处sub为类对象，ps为类指针
	ps = &sub;

	//取出虚函数表的地址

	unsigned long long vaddrsheet;

	vaddrsheet = *(unsigned long long*)ps;

	//根据虚函数表的地址，取出虚函数表的函数地址。

	unsigned long long* pvaddrsheet;

	pvaddrsheet = (unsigned long long*)vaddrsheet;

	//定义一个指针函数,
	void (*pfn)();

	for (int i = 0; *(pvaddrsheet + i) != 0; i++) {

		pfn = (void (*)()) * (pvaddrsheet + i);

		pfn();

	}
}

int main() {
	//test1();

	test2();
}