#include <cstdio>
#include <string.h>
/*********************0205************************
1������һ���ṹ��Monster���ܹ��洢�ֵĸ�����Ϣ(������һ����Ա�ǽṹ������)��
2������һ��Monster���͵����飬����Ϊ10.
3����дһ��������Ϊ�ڶ����е����鸳ֵ.
4����дһ���������ܹ�ͨ������ID����ӡ��ǰ��������������Ϣ.
*/

//1������һ���ṹ��Monster���ܹ��洢�ֵĸ�����Ϣ(������һ����Ա�ǽṹ������)��

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

//2������һ��Monster���͵����飬����Ϊ10.

Mon arr[10];


//3����дһ��������Ϊ�ڶ����е����鸳ֵ.
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

//4����дһ���������ܹ�ͨ������ID����ӡ��ǰ��������������Ϣ.
void function2_5_4(Mon array[], int i) {
	printf("id-->%d\nlevel-->%d\nsex-->%d\n|-blood-->%d\n|-home-->%s\n",
		array[i].id, array[i].level, array[i].sex, array[i].con.blood, array[i].con.home);
}

//5����������ṹ����ڴ���䣺
struct S1
{
	char c;//1
	double i;//8
};//�ṹ����16

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

//�ݹ麯���������õݹ麯����һ��ʮ���Ƶ���ת���ɶ����Ƶ�����

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