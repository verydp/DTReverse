#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

#define SUCCESS 1
#define ERROR -1
#define MALLOC_ERROR -2
#define INDEX_ERROR -3


//typedef unsigned __int64 QWORD;



template <class T_ELE>
class vector {
public:
	vector();
	vector(DWORD dsize);
	~vector();
public:
	DWORD at(DWORD m_dwIndex);						//根据给定的索引得到元素
	DWORD push_back(T_ELE element);				//将元素存储到容器最后一个位置
	DWORD pop_back();								//删除最后一个元素
	DWORD insert(DWORD m_dwIndex, T_ELE element);	//向指定位置新增一个元素
	DWORD capacity();								//返回在不增容的情况下，还能存储多少元素
	VOID  clear();									//清空所有元素
	BOOL  empty();									//判断Vector是否为空 返回true时为空
	VOID  erase(DWORD m_dwIndex);					//删除指定元素	
	DWORD size();									//返回Vector元素数量的大小
	T_ELE operator[](DWORD m_dwIndex);				//根据给定的索引得到元素
	T_ELE& operator=(T_ELE element);

private:
	BOOL expand();
private:
	DWORD m_dwIndex;								//下一个可用索引	
	DWORD m_dwIncrement;							//每次增容的大小	
	DWORD m_dwLen;									//当前容器的长度
	DWORD m_dwInitSize;								//默认初始化大小	
	T_ELE* m_pvector;								//容器指针	
};


template <class T_ELE>
vector<T_ELE>::vector():m_dwInitSize(10), m_dwIncrement(5) {

	//1.创建长度为m_dwInitSize个T_ELE对象
	this->m_pvector = new T_ELE[10];

	//2.将新创建的空间初始化
	memset(this->m_pvector, 0, 10 * sizeof(T_ELE));

	//3.设置其他值
	this->m_dwIndex = 0;

	//this->m_dwIncrement = 5;

	this->m_dwLen = 10;

	//this->m_dwInitSize = 100;


}

template <class T_ELE>
vector<T_ELE>::vector(DWORD dsize):m_dwIndex(0), m_dwIncrement(5), m_dwInitSize(0) {
	//1.创建长度为m_dwInitSize个T_ELE对象

	this->m_pvector = new T_ELE[dsize];

	//2.将新创建的空间初始化

	memset(this->m_pvector, 0, dsize * sizeof(T_ELE));

	//3.设置其他值

	this->m_dwLen = dsize;
}

template <class T_ELE>
vector<T_ELE>::~vector() {
	delete[] this->m_pvector;

	printf("this vector has been deleted~\n");
}

template <class T_ELE>
BOOL vector<T_ELE>::expand() {
	// 1. 计算增加后的长度

	// 2. 申请空间
	T_ELE* pvector = new int[this->m_dwLen + 5];

	// 3. 将数据复制到新的空间
	for (int i = 0; i < this->m_dwLen; i++) {
		pvector[i] = this->m_pvector[i];
	}

	// 4. 释放原来空间
	delete[] this->m_pvector;

	// 5. 为各种属性赋值

	this->m_dwIncrement = 5;

	this->m_dwInitSize = 10;

	this->m_dwLen += 5;

	this->m_pvector = pvector;

	if (this->m_pvector != NULL) {
		return SUCCESS;
	}
	else {
		return ERROR;
	}

}

template <class T_ELE>
DWORD vector<T_ELE>::at(DWORD m_dwIndex) {

	return this->m_pvector[m_dwIndex];
}



template <class T_ELE>
DWORD vector<T_ELE>::push_back(T_ELE element) {
	if (this->m_dwIndex >= this->m_dwLen) {
		this->expand();
	}
	this->m_pvector[this->m_dwIndex] = element;

	this->m_dwIndex++;

	return SUCCESS;
}

template <class T_ELE>
DWORD vector<T_ELE>::pop_back() {
	this->m_dwIndex--;

	return SUCCESS;
}

template <class T_ELE>
DWORD vector<T_ELE>::insert(DWORD m_dwIndex, T_ELE element) {

	int size = this->m_dwIndex;

	if (m_dwIndex > this->m_dwIndex) {
		return ERROR;
	}

	if (this->m_dwIndex == m_dwIndex) {
		this->expand();
	}

	for (int i = 0; i < this->m_dwIndex - m_dwIndex; i++) {
		this->m_pvector[size - i] = this->m_pvector[size - i -1];
	}
	this->m_pvector[m_dwIndex] = element;

	this->m_dwIndex++;

	return SUCCESS;

}

template <class T_ELE>
DWORD vector<T_ELE>::capacity() {
	return this->m_dwLen - this->m_dwIndex;
}

template <class T_ELE>
VOID  vector<T_ELE>::clear() {

	this->m_dwIndex = 0;

}

template <class T_ELE>
BOOL  vector<T_ELE>::empty() {
	if (this->m_dwIndex == 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

template <class T_ELE>
VOID  vector<T_ELE>::erase(DWORD m_dwIndex) {
	
	int size = this->m_dwIndex;

	for (int i = 0; i < size - m_dwIndex; i++) {
		this->m_pvector[m_dwIndex + i] = this->m_pvector[m_dwIndex + i + 1];
	}

	this->m_dwIndex--;
	
	//return SUCCESS;
}

template <class T_ELE>
DWORD vector<T_ELE>::size() {
	return this->m_dwIndex;
}

template <class T_ELE>
T_ELE vector<T_ELE>::operator[](DWORD m_dwIndex) {
	return this->m_pvector[m_dwIndex];
}

template <class T_ELE>
T_ELE& vector<T_ELE>::operator=(T_ELE element) {
	return element;
}

class info
{
private:
	int len;
	int weight;
public:
	info() {
		this->len = 1;
		this->weight = 1;
	}

	info(int len, int weight) {
		this->len = len;
		this->weight = weight;
	}
};

int main() {



	vector<int> v1,v2(3);

	vector<info> v3;

	v1.push_back(1);

	v1.push_back(2);

	v1.push_back(3);

	v1.pop_back();

	v1.insert(1,22);



	printf("v1[0] =  %d\t capacity = %d \n", v1[0],v1.capacity());

}
