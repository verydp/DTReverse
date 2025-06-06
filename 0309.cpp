#include <cstdio>
#include <string.h>

//(使用位操作指令、比如：& | ! ^ << >>等)


unsigned char x = 4;

unsigned char x3 = 4;
unsigned char x5 = 16;
unsigned char x7 = 64;

//1、定义一个unsiged char 类型，通过程序为第3、5、7位赋值,赋值时不能影响到其它位原来的值.
unsigned char function3_9_1(unsigned char x,int k,int p) {
	unsigned char ret;
	//首先依据p参数来确认是要置1还是置0，从而生成对应的一个unsigned char值n，用来和变量x进行运算
	if (p == 0) {
		//该情况为用户想要在第k位将值置0。即生成一个第k位为0，其余7位全部为1的unsigned char数和x进行与运算。
		printf("%x\n",x & ~(1 << (k-1)));
		ret = x & ~(1 << (k - 1));

	}
	else {
		//该情况为用户想要在第k位将值置1.即生成一个第k位为1，其余7位全部位0的unsigned char数和x进行或运算。
		printf("%x\n", x | 1 << (k - 1));
		ret = x | 1 << (k - 1);
	}

	return ret;
}

//2、判断某个位的值是否为1.
void function3_9_2(unsigned char x,int p) {
	if (x >> p & 1) {
		printf("%d in position %d is 1", x,p);
	}
	else {
		printf("%d in position %d is 0", x, p);
	}
}

//3、读取第7、6、5位的值，以十进制显示(unsigned).
void function3_9_3(unsigned char x) {
	unsigned char ret;
	ret = x >> 4;
	ret = ret & 7;
	printf("%d\n", ret);
}

int main() {
	function3_9_1(0, 4, 1);

	//function3_9_2(8, 3);

	//function3_9_3(104);

}