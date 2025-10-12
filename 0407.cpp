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

	//������ָ���ַ��Ҳ���麯�����ָ���ַ���˴�subΪ�����psΪ��ָ��
	ps = &sub;

	//ȡ���麯����ĵ�ַ

	unsigned long long vaddrsheet;

	vaddrsheet = *(unsigned long long*)ps;

	//�����麯����ĵ�ַ��ȡ���麯����ĺ�����ַ��

	unsigned long long* pvaddrsheet;

	pvaddrsheet = (unsigned long long*)vaddrsheet;

	//����һ��ָ�뺯��,
	void (*pfn)();

	for (int i = 0;i< 6; i++) {

		pfn = (void (*)()) * (pvaddrsheet + i);

		pfn();
		
	}


	/*
//д��һ��������һ�֡�����ֵΪvoid �޲����ĺ������� ����pfunc�����ĺ�������(pfunc�������һ�ֺ�������)
typedef void pfunc();

pfunc* pf;

pf = (pfunc*)*(unsigned long long*)(*(unsigned long long*)ps);

pf();

//д������������һ����ָ�롱����������ֵΪvoid��������ָ������Ϊpfn��
void (*pfn)();

pfn = (void (*)())*(unsigned long long*)(*(unsigned long long*)ps);

pfn();
*/
}

void test2() {
	Sub1 sub;

	Sub1* ps;

	//������ָ���ַ��Ҳ���麯�����ָ���ַ���˴�subΪ�����psΪ��ָ��
	ps = &sub;

	//ȡ���麯����ĵ�ַ

	unsigned long long vaddrsheet;

	vaddrsheet = *(unsigned long long*)ps;

	//�����麯����ĵ�ַ��ȡ���麯����ĺ�����ַ��

	unsigned long long* pvaddrsheet;

	pvaddrsheet = (unsigned long long*)vaddrsheet;

	//����һ��ָ�뺯��,
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