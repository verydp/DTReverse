#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>
#include "0403.h"

/*
本节练习：

1 设计一个结构 DateInfo, 要求其满足下述要求。

(1) 有三个成员： int year; int month; int day;
(2) 要求有个带参数的构造函数，其参数分别为对应年、月、日。
(3) 有一个无参数的构造函数，其初始的年、月、日分别为：2015、4、2。
(4) 要求有一个成员函数实现日期的设置：SetDay(int day)
(5) 要求有一个成员函数实现日期的获取: GetDay()
(6) 要求有一个成员函数实现年份的设置: SetYear(int year)
(7) 要求有一个成员函数实现年份的获取: GetYear()
(8) 要求有一个成员函数实现月份的设置: SetMonth(int month)
(9) 要求有一个成员函数实现月份的获取: GetMonth()
*/

DateInfo::DateInfo() {
	this->year = 2015;
	this->month = 4;
	this->day = 2;
}

DateInfo::DateInfo(int year, int month, int day) {
	this->year = year;
	this->month = month;
	this->day = day;
}

void DateInfo::SetDay(int day) {
	this->day = day;
}

int DateInfo::GetDay() {
	printf("%d\n", this->day);
	return this->day;
}

void DateInfo::SetYear(int year) {
	this->year = year;
}

int DateInfo::GetYear() {
	printf("%d\n", this->year);
	return this->year;
}

void DateInfo::SetMonth(int month) {
	this->month = month;
}

int DateInfo::GetMonth() {
	printf("%d\n", this->month);
	return this->month;
}


/*
2 设计一个结构 TimeInfo, 要求其满足下述要求。
(1) 该结构中包含表示时间的时、分、秒。
(2) 设置该结构中时、分、秒的函数。
(3) 获取该结构中时、分、秒的三个函数：GetHour(), GetMinute()和GetSecond()。
*/

TimeInfo::TimeInfo() {
	this->hour = 21;
	this->minute = 44;
	this->second = 11;
}

TimeInfo::TimeInfo(int hour, int minute, int second) {
	this->hour = hour;
	this->minute = minute;
	this->second = second;
}

void TimeInfo::SetHour(int hour) {
	this->hour = hour;
}

int TimeInfo::GetHour() {
	//printf("%d\n", this->hour);
	return this->hour;
}

void TimeInfo::SetMinute(int minute) {
	this->minute = minute;
}

int TimeInfo::GetMinute() {
	//printf("%d\n", this->minute);
	return this->minute;
}

void TimeInfo::SetSecond(int second) {
	this->second = second;
}

int TimeInfo::GetSecond() {
	//printf("%d\n", this->second);
	return this->second;
}



/*
3 让TimeInfo继承DateInfo 分别使用DataInfo和TimeInfo的指针访问TimeInfo
对象的成员.
*/

void testDate() {
	TimeInfo t;

	TimeInfo* pt;



	//printf("%d %d\n", t.GetHour(), );
}




/*
4 设计一个结构叫做MyString，要求该结构能够完成以下功能：
(1) 构造函数能够根据实际传入的参数分配实际存储空间；
(2) 提供一个无参的构造函数，默认分配大小为1024个字节；
(3) 析构函数释放该空间；
(4) 编写成员函数SetString，可以将一个字符串赋值给该结构；
(5) 编写成员函数PrintString，可以将该结构的内容打印到屏幕上；
(6) 编写成员函数AppendString，用于向已有的数据后面添加数据；
(7) 编写成员函数Size，用于得到当前数据的真实长度。
编写测试程序，测试这个结构。
*/


MyString::MyString(int length) {

	this->str = (char*)malloc(length);
	memset(this->str, 0, length);
}
MyString::MyString() {
	this->str = (char*)malloc(1024);
	memset(this->str, 0, 1024);
}
MyString::~MyString() {
	printf("this struct is destoryed~");
}

void MyString::SetString(char* string) {
	char* tmp;
	tmp = this->str;
	for (; *string != 0; string++) {
		memcpy(tmp, string, 1);
		tmp++;
	}
}

void MyString::PrintString() {
	char* tmp;
	tmp = this->str;

	for (; *tmp != 0; tmp++) {
		printf("%c", *tmp);
	}
}

void MyString::AppendString(char* string) {
	//获取原本字符串的的长度i
	int i = 0;

	char* tmp;
	tmp = this->str;

	while (*tmp != 0) {
		i++;
		tmp++;
	}

	for (; *string != 0; string++) {
		memcpy(tmp, string, 1);
		tmp++;
		i++;
	}
}
int MyString::size() {
	int i = 0;

	char* tmp;
	tmp = this->str;

	while (*tmp != 0) {
		i++;
		tmp++;
	}
	return i;
}



void testString() {

	MyString str;

	printf("%d\n", str.size());

	char string[] = "aaaa";

	str.SetString(string);

	printf("%d\n", str.size());

	char string1[] = "bbb";

	str.AppendString(string1);

	printf("%d\n", str.size());

	str.PrintString();



}

int main() {
	//testDate();

	testString();
}
