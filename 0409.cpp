#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

template<class T>
void swap(T &x, T &y) {
	//printf("before: x = %d y = %d||||||||||", x, y);
	T temp;

	temp = x;
	x = y;
	y = temp;

	//printf("after: x = %d y = %d\n", x, y);
}

//√∞≈›≈≈–Ú{5,2,6,9,2,3,4,7}
template<class Q>
void sort(Q arr[], int length) {
	int temp;

	for (int x=0;x<length;x++) {

		for (int i = 0; i < length - 1 - x ; i++) {

			if (arr[i] > arr[i + 1]) {
				swap(arr[i], arr[i + 1]);
			}
		}
	}

}

int main() {
	short x, y;
	x = 3;
	y = 4;
	swap(x, y);

	printf("================================\n");

	short arr[] = { 5, 2, 6, 9, 2, 3, 4, 7 };

	sort(arr, 8);

	int arr1[] =  {2,6,1,5,4};

	char arr2[] = { 2,6,1,5,4 };

	sort(arr1, 5);

	sort(arr2, 5);

}