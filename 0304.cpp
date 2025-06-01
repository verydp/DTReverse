#include <cstdio>
#include <string.h>

//1、 *(  ) 与[  ]是可以互换的，也就是说：*(*(p+1)+2) 相当与 p[1][2]
//那*(p + 1)[2] 是否一定等于p[1][2]呢？ 通过反汇编进行论证。

int arr[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

void function3_4_1() {

	int (*px)[4];
	px = (int (*)[4])arr;

	printf("%d\n", *(*(px+1)+2));
	printf("%d\n", px[1][2]);
	printf("%d\n", *(px+1)[2]);

}

//使用数组指针遍历一个一维数组.

void function3_4_2() {
	int (*px)[2];
	px = (int (*)[2])arr;

	printf("%d\n", *(*px)    );
	/*
	for (int i = 0;i < 10;i++) {
		px = px + i;

		printf("%d\n%d\n", *((*px) + 0));
	}
	*/
	
}

int main() {
	function3_4_2();
}