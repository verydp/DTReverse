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

//¶þ·Ö²éÕÒ
void half(int arr[], int length, int aim) {
	int begin, end;

	begin = 0;
	end = length;

	if (aim > arr[begin] && aim < arr[length]) {

	}
}



int main() {
	array p1,p2(2,2);

	p2++;

	p2.print();
}