#include <cstdio>
#include <string.h>

/*************************0206***********************
1��дһ��switch��䣬���������Ҳ������С��������Ӧ�ķ����.
2��дһ��switch��䣬ֻ���ɴ��,������Ӧ�ķ����.
3��дһ��switch��䣬���ɴ���С��,������Ӧ�ķ����.
4��Ϊdo..while������ɵķ������дע��.
5��Ϊwhile������ɵķ������дע��.
6��Ϊfor������ɵķ������дע��.
*/

//1��дһ��switch��䣬���������Ҳ������С��������Ӧ�ķ����.
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

//2��дһ��switch��䣬ֻ���ɴ��,������Ӧ�ķ����.
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

//3��дһ��switch��䣬���ɴ���С��,������Ӧ�ķ����.
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

//4��Ϊdo..while������ɵķ������дע��.
void function2_6_4(int x, int y) {
	do
	{
		x++;
	} while (x > y);
}

//5��Ϊwhile������ɵķ������дע��.
void function2_6_5(int x, int y) {
	while (x < y) {
		x++;
	}
}

//6��Ϊfor������ɵķ������дע��.
void function2_6_6(int y) {
	int i = 0;
	for (i;i < y;i++) {
		printf("okok");
	}
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