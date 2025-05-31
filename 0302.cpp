#include <cstdio>
#include <string.h>

/***************0302*******************/

//char s[3];
//返回值是字符串s的长度。不包括结束符'/0'。
int strlen(char* str) {
	int ret = 0;

	for (int i = 0;*(str + i) != 0;i++) {
		//printf("%s", str[i]);
		ret++;
	}

	return ret;
}
//注意：从内存中看，全局变量中多个字符串变量的存放是没有\0做结尾的。


//char dest[1] = { 'a' };
//char src[3] = { 'c','d','e' };
//复制字符串src到dest中。返回指针为dest的值。
char* strcpy(char* dest, char* src) {
	char* ret;
	ret = dest;
	while (*dest = *src) {
		dest++;
		src++;
	}

	return ret;
}
//	printf("%s", strcpy(dest, src));

//char src[3] = {'a','c','0'};
//char dest[4] = {'t','d','s','0'};

char src[2] = { 'a','c' };
char dest[3] = { 't','d','s' };
char src1[2] = { 'a','c' };

//将字符串src添加到dest尾部。返回指针为dest的值。
char* strcat(char* dest, char* src) {
	char* ret;
	ret = dest;
	dest = dest + strlen(dest);

	for (int i = 0;i < strlen(src);i++) {
		*(dest + i) = *(src + i);
	}

	return ret;
}

//一样返回0  不一样返回1
int strcmp(char* s1, char* s2) {
	while (*s1++ == *s2++) {
		if (*s1++ == *s2++) { return 1; }
	}
	return 0;
}

//这一堆数据中存储了角色的名字信息(WOW)，请列出角色名的内存地址.
char array[100] = {
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x07,0x09,
	0x00,0x20,0x10,0x03,0x03,0x0C,0x00,0x00,0x44,0x00,
	0x00,0x33,0x00,0x47,0x0C,0x0E,0x00,0x0D,0x00,0x11,
	0x00,0x00,0x00,0x02,0x64,0x00,0x00,0x00,0xAA,0x00,
	0x00,0x00,0x64,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x00,0x74,0x0F,0x41,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0A,0x00,
	0x00,0x02,0x57,0x4F,0x57,0x00,0x06,0x08,0x00,0x00,
	0x00,0x00,0x00,0x64,0x00,0x0F,0x00,0x00,0x0D,0x00,
	0x00,0x00,0x23,0x00,0x00,0x64,0x00,0x00,0x64,0x00
};

//1、编写函数，返回角色名字信息的地址，如果没有返回0
//const char* rolename = "WOW";

char rolename[3] = { 'W','O','W' };

char* FindRoleName(char* pData, char* pRoleName) {

	for (int i = 0;i < 100;i++) {
		//printf("%d\n", i);

		if (*pRoleName == *(pData + i) && *(pRoleName + 1) == *(pData + 1 + i) && *pRoleName == *(pData + 2 + i))
		{
			return pData + i;
		}
	}
	printf("end\n");
	return 0;
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

	FindRoleName(array, rolename);

	return 0;
}