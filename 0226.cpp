#include <cstdio>
#include <string.h>

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

	char arr2[10] = { 0 };//1byte*10=10byte

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