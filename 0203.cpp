#include <cstdio>
#include <string.h>

void data() {
	int x = 1;
	int y = 2;
	int r;
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };

	r = arr[1];
	r = arr[x];
	r = arr[y];
	r = arr[x * 2 + y];

}
//Ͱ����
void function1() {

	int arr[10] = { 6,3,1,8,7,0,1,2,4,9 };

	int retarr[10] = { 0 };

	int finnalarr[10] = { 0 };

	for (int x = 0; x < 10; x++) {

		retarr[arr[x]]++;

	}

	for (int y = 0;y < 10;y++) {
		//printf("%d\n",retarr[y]);

		if (retarr[y] != 0) {
			for (retarr[y];retarr[y] > 0;retarr[y]--) {
				printf("%d\n", y);
			}
		}

	}
}


//ð������
void function2() {
	int x = 0;
	int y = 0;
	int k = 0;
	int arr[10] = { 6,3,1,8,7,0,1,2,4,9 };

	while (y < 10) {

		x = 0;
		while (x < 9 - y) {

			if (arr[x] > arr[x + 1]) {

				arr[x] = arr[x] + arr[x + 1];
				arr[x + 1] = arr[x] - arr[x + 1];
				arr[x] = arr[x] - arr[x + 1];
			}

			x++;
		}

		k = 0;
		while (k < 10) {
			printf("%d\n", arr[k]);
			k++;
		}

		printf("-------------------\n");

		y++;
	}

	/*
	x = 0;

	while (x < 9) {

		if (arr[x] > arr[x + 1]) {

			arr[x] = arr[x] + arr[x + 1];
			arr[x + 1] = arr[x] - arr[x + 1];
			arr[x] = arr[x] - arr[x + 1];
		}

		x++;
	}
	k = 0;
	while (k < 10) {
		printf("%d\n", arr[k]);
		k++;
	}
	printf("-------------------\n");

	*/
	/*
	while (y < 10) {
	while (k < 10) {
		printf("%d\n", arr[k]);
		k++;
	}

	printf("-------------------");
	y++;
	*/

}


void function3() {
	char arr1[3] = { 1,2,3 };
	//char arr2[4] = { 1,2,3,4 };
	//char arr3[5] = { 1,2,3,4,5 };
}

void function2_3() {

	//����������5���࣬ÿ����10���ˣ����һ����ά����洢��Щ�˵�����.
	int arr[5][10] = {
		{1,2,3,4,5,6,7,8,9,10},
		{11,12,13,14,15,16,17,28,19,20},
		{21,22,23,24,25,26,27,28,29,30},
		{31,32,33,34,35,36,37,38,39,40},
		{41,42,43,44,45,46,47,48,49,50},
	};

	//�����֪���ڶ�����ĵ�6���˵����䣬Ӧ����λ�ȡ������������λ�ȡ��
	printf("%d\n", arr[1][5]);

	printf("---------��ӡ���а༶������ѧ��������(ÿ���༶��ӡһ��).---------\n");

	//��ӡ���а༶������ѧ��������(ÿ���༶��ӡһ��).
	for (int i = 0;i < 5;i++) {
		for (int x = 0;x < 10;x++) {
			printf("%d ", arr[i][x]);
		}
		printf("\n");
	}

	printf("---------���ڶ����༶�ĳ���20���ѧ���������޸�Ϊ21��.---------\n");

	//���ڶ����༶�ĳ���20���ѧ���������޸�Ϊ21��.
	for (int i = 0;i < 10;i++) {
		if (arr[1][i] > 20) { arr[1][i] = 21; }
	}

	for (int i = 0;i < 5;i++) {
		for (int x = 0;x < 10;x++) {
			printf("%d ", arr[i][x]);
		}
		printf("\n");
	}

	printf("----------//��ӡ��ÿ���༶ѧ��������ĺ�.--------\n");

	//��ӡ��ÿ���༶ѧ��������ĺ�.

	for (int i = 0;i < 5;i++) {
		int k = 0;
		for (int x = 0;x < 10;x++) {
			k += arr[i][x];
		}
		printf("%d------%d\n", i, k);
	}

}

//�鲢����
void function2_3_6() {
	/*
	* ǰ�᣺�����������Ѿ����Դ�С����������˵ġ�
	����һ��[3,5,7,9,12,25,34,55]
	�������[4,7,9,11,13,16]
	�������������������ݽ��д�С��������򣬴洢����һ��������.
	˼·��ÿ�δӸ�������ժ����С��ֵ�����뵽�µ������С�����С���������һ��ֵ��
	i=0-->3
	k=0-->4
	4>3
	{3}
	i++-->5
	k=0-->4
	5>4
	*/

	int arr1[8] = { 3,5,7,9,12,25,34,55 };
	int arr2[6] = { 4,7,9,11,13,16 };
	int arr3[14] = {};

	int x = 0;
	int y = 0;

	while (x + y < 14) {
		if (arr1[x] > arr2[y] && y < 6) {
			arr3[x + y] = arr2[y];
			y++;
		}
		else
		{
			arr3[x + y] = arr1[x];
			x++;
		}
	}


	for (int i = 0;i < 14;i++) {
		printf("%d ", arr3[i]);
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