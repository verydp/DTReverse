#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

class array {

private:
	int x;
	int y;

public:
	array() {
		this->x = 1;
		this->y = 1;
	}

	array(int x, int y) {
		this->x = x;
		this->y = y;
	}

	array operator++(int) {
		this->x = this->x + 1;
		this->y = this->y + 1;

		return *this;
	}

	array operator+(const array& p) {
		this->x = this->x + p.x;
		this->y = this->y + p.y;
	}

	array operator-(const array& p) {
		this->x = this->x - p.x;
		this->y = this->y - p.y;
	}

	array operator*(const array& p) {
		this->x = this->x * p.x;
		this->y = this->y * p.y;
	}

	array operator/(const array& p) {
		this->x = this->x / p.x;
		this->y = this->y / p.y;
	}

	bool operator>(const array& p) {
		if (this->x + this->y > p.x + p.y) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	void print() {
		printf("%d %d\n", this->x, this->y);
	}

};

//二分查找
int half(int arr[], int length, int aim) {

	int begin, end, half;

	begin = 0;
	end = length - 1;

	//[1 2 3 4 5 6 7 8 9]
	//[1 2 3 4 5 6 7 8 9 10]

	while ( begin <= end ) {

		half = begin + (end - begin) / 2;

		if (arr[half] == aim) {
			return half;
		}
		else if (arr[half] > aim) {
			end = half;
		}
		else {
			begin = half;
		}


	}
}

int binary_search(int arr[], int size, int key) {
	int left = 0;
	int right = size - 1;

	while (left <= right) {
		int mid = left + (right - left) / 2; // 避免(left + right)溢出

		if (arr[mid] == key) {
			return mid; // 找到目标
		}
		else if (arr[mid] < key) {
			left = mid + 1; // 去右半边找
		}
		else {
			right = mid - 1; // 去左半边找
		}
	}
	return -1; // 没找到
}



int main() {

	array p1,p2(2,2);

	p2++;

	p2.print();

	int arr[] = { 1,2,3,4,5,6,7,8,9 };

	printf("%d\n", half(arr, 9, 9));

	//printf("%d\n", binary_search(arr, 9, 1));

}