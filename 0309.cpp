#include <cstdio>
#include <string.h>

//(ʹ��λ����ָ����磺& | ! ^ << >>��)


unsigned char x = 4;

unsigned char x3 = 4;
unsigned char x5 = 16;
unsigned char x7 = 64;

//1������һ��unsiged char ���ͣ�ͨ������Ϊ��3��5��7λ��ֵ,��ֵʱ����Ӱ�쵽����λԭ����ֵ.
unsigned char function3_9_1(unsigned char x,int k,int p) {
	unsigned char ret;
	//��������p������ȷ����Ҫ��1������0���Ӷ����ɶ�Ӧ��һ��unsigned charֵn�������ͱ���x��������
	if (p == 0) {
		//�����Ϊ�û���Ҫ�ڵ�kλ��ֵ��0��������һ����kλΪ0������7λȫ��Ϊ1��unsigned char����x���������㡣
		printf("%x\n",x & ~(1 << (k-1)));
		ret = x & ~(1 << (k - 1));

	}
	else {
		//�����Ϊ�û���Ҫ�ڵ�kλ��ֵ��1.������һ����kλΪ1������7λȫ��λ0��unsigned char����x���л����㡣
		printf("%x\n", x | 1 << (k - 1));
		ret = x | 1 << (k - 1);
	}

	return ret;
}

//2���ж�ĳ��λ��ֵ�Ƿ�Ϊ1.
void function3_9_2(unsigned char x,int p) {
	if (x >> p & 1) {
		printf("%d in position %d is 1", x,p);
	}
	else {
		printf("%d in position %d is 0", x, p);
	}
}

//3����ȡ��7��6��5λ��ֵ����ʮ������ʾ(unsigned).
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