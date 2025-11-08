#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

#define SUCCESS           1 // 执行成功
#define ERROR            -1 // 执行失败
#define INDEX_IS_ERROR   -2 // 错误的索引号
#define BUFFER_IS_EMPTY  -3 // 缓冲区已空

template <class T_ELE>
class LinkedList
{

public:
	LinkedList();
	~LinkedList();

public:
	BOOL  IsEmpty();										//判断链表是否为空 空返回1 非空返回0				
	void  Clear();											//清空链表				
	DWORD GetElement(IN DWORD dwIndex, OUT T_ELE& Element);	//根据索引获取元素				
	DWORD GetElementIndex(IN T_ELE& Element);				//根据元素获取链表中的索引				
	DWORD Insert(IN T_ELE Element);							//新增元素				
	DWORD Insert(IN DWORD dwIndex, IN T_ELE Element);		//根据索引新增元素				
	DWORD Delete(IN DWORD dwIndex);							//根据索引删除元素				
	DWORD GetSize();										//获取链表中元素的数量	

private:
	typedef struct _NODE
	{
		T_ELE  Data;
		_NODE* pNext;
	}NODE, * PNODE;
	PNODE GetIndexCurrentNode(DWORD dwIndex);				//获取索引为dwIndex的指针				
	PNODE GetIndexPreviousNode(DWORD dwIndex);				//获取索引为dwIndex的前一个节点指针				
	PNODE GetIndexNextNode(DWORD dwIndex);					//获取索引为dwIndex的后一个节点指针

private:
	PNODE m_pList;											//链表头指针，指向第一个节点				
	DWORD m_dwLength;										//元素的数量				
};




template <class T_ELE>
LinkedList<T_ELE>::LinkedList() {

	this->m_pList = NULL;

	this->m_dwLength = 0;

}

template <class T_ELE>
LinkedList<T_ELE>::~LinkedList() {
	Clear();
}

template <class T_ELE>
BOOL LinkedList<T_ELE>::IsEmpty() {
	if (this->m_pList == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

template <class T_ELE>
VOID  LinkedList<T_ELE>::Clear() {
	NODE* temp = this->m_pList;

	// 0,1,2,3,4,5
	for (int i = 0; i < this->m_dwLength - 1; i++) {
		delete temp;
		temp = GetIndexNextNode(i);
	}

}

template <class T_ELE>
DWORD LinkedList<T_ELE>::Insert(IN T_ELE Element) {

	_NODE* pNode = new _NODE;

	pNode->Data = Element;

	pNode->pNext = NULL;

	if (IsEmpty()) {

		this->m_pList = pNode;

		this->m_dwLength++;

		return SUCCESS;
	}
	else {
		GetIndexCurrentNode(this->m_dwLength - 1)->pNext = pNode;

		this->m_dwLength++;

		return SUCCESS;
	}
	/*
	NODE* temp = m_pList;
	// 0,1,2,3,4,5    6个
	for (int i = 0; i < this->m_dwLength; i++) {
		temp = temp->pNext;
	}
	temp->pNext = pNode;
	this->m_dwLength++;
	*/
}

template <class T_ELE>//此函数有问题
DWORD LinkedList<T_ELE>::Insert(IN DWORD dwIndex, IN T_ELE Element) {
	_NODE* pNode = new _NODE;

	pNode->Data = Element;

	//if (dwIndex > this->m_dwLength - 1 or this->m_pList == NULL) {
	if (dwIndex > this->m_dwLength - 1 or this->m_pList == NULL) {
		return INDEX_IS_ERROR;
	}

	// 0,1,2,3,4,5   6个

	if (dwIndex == 0) {
		//当节点在数组的开头时
		pNode->pNext = m_pList;

		m_pList = pNode;
	}
	else if (dwIndex == this->m_dwLength - 1) {
		//当节点在数组的结尾时。
		pNode->pNext = GetIndexCurrentNode(dwIndex);

		GetIndexCurrentNode(dwIndex - 1)->pNext = pNode;

	}
	else {
		//当节点在数组的中间时
		//先将后面的节点，接上新增的节点
		pNode->pNext = GetIndexNextNode(dwIndex);

		//再将前面的节点接上新增节点
		GetIndexCurrentNode(dwIndex)->pNext = pNode;
	}

	this->m_dwLength++;

	return SUCCESS;
}

template <class T_ELE>
DWORD LinkedList<T_ELE>::GetElement(IN DWORD dwIndex, OUT T_ELE& Element) {

	if (dwIndex > this->m_dwLength - 1 or this->m_pList == NULL) return INDEX_IS_ERROR;

	// 0,1,2,3,4,5

	Element = GetIndexCurrentNode(dwIndex)->Data;

	return Element;
}

template <class T_ELE>
DWORD LinkedList<T_ELE>::GetElementIndex(IN T_ELE& Element) {

	NODE* temp = this->m_pList;

	for (int i = 0; i < this->m_dwLength; i++) {
		if (temp->Data == Element) {
			//根据元素值，返回索引号
			return i;
		}
		else {
			temp = temp->pNext;
		}
	}
	//无法找到元素值
	return ERROR;
}

template <class T_ELE>
DWORD LinkedList<T_ELE>::Delete(IN DWORD dwIndex) {

	if (dwIndex > this->m_dwLength - 1 or this->m_pList == NULL) {
		return INDEX_IS_ERROR;
	}

	NODE* pNode = GetIndexCurrentNode(dwIndex);

	if (dwIndex == this->m_dwLength - 1) {

		delete GetIndexCurrentNode(dwIndex);

		GetIndexCurrentNode(dwIndex - 1)->pNext = NULL;

	}else if (dwIndex == 0) {

		pNode = GetIndexNextNode(dwIndex);

		delete GetIndexCurrentNode(dwIndex);

		this->m_pList = pNode;
	}
	else {
		pNode = GetIndexNextNode(dwIndex);

		delete GetIndexCurrentNode(dwIndex);

		GetIndexCurrentNode(dwIndex-1)->pNext = pNode;
	}

	this->m_dwLength--;

	return SUCCESS;
	/*
	NODE* temp = *m_pList;

	if (dwIndex > this->m_dwLength - 1 or this->m_pList == NULL) {
		return INDEX_IS_ERROR;
	}

	// 0,1,2,3,4,5

	if (dwIndex == 0) {
		m_pList = temp->pNext;

		delete temp;
	}

	for (int i = 0; i < dwIndex; i++) {
		temp = temp->pNext;
	}

	temp->pNext = temp->pNext->pNext;

	delete temp->pNext;

	*/

}

template <class T_ELE>
DWORD LinkedList<T_ELE>::GetSize() {
	return this->m_dwLength;
}

template <class T_ELE>
typename LinkedList<T_ELE>::PNODE LinkedList<T_ELE>::GetIndexCurrentNode(DWORD dwIndex) {
	if (dwIndex >= this->m_dwLength || this->m_pList == NULL) {
		return NULL;
	}

	NODE* temp = this->m_pList;
	for (int i = 0; i < dwIndex; i++) {
		temp = temp->pNext;
	}
	return temp;
}

template <class T_ELE>
typename LinkedList<T_ELE>::PNODE LinkedList<T_ELE>::GetIndexPreviousNode(DWORD dwIndex) {
	if (dwIndex >= this->m_dwLength || this->m_pList == NULL) {
		return NULL;
	}

	NODE* temp = this->m_pList;
	for (int i = 0; i < dwIndex + 1; i++) {
		temp = temp->pNext;
	}
	return temp;
}

template <class T_ELE>
typename LinkedList<T_ELE>::PNODE LinkedList<T_ELE>::GetIndexNextNode(DWORD dwIndex) {
	if (dwIndex + 1 >= this->m_dwLength || this->m_pList == NULL) {
		return NULL;
	}

	NODE* temp = this->m_pList;
	for (int i = 0; i < dwIndex + 1; i++) {
		temp = temp->pNext;
	}

	return temp;
}



int main() {
	int a = 1;
	int b = 2;

	a = b;

	printf("%d\n", a);

	LinkedList<int> list;

	list.IsEmpty();

	list.Insert(1);

	list.IsEmpty();

	list.Insert(2);

	list.Insert(1,3);

	list.Insert(0, 0);

	list.GetSize();

	list.GetElement(0,a);

	list.GetElementIndex(b);

	list.Delete(0);
	list.Delete(1);
	list.Delete(1);

}

/*
int类型也是一种类，也有默认构造函数int()，和有参构造函数int(X)。
int* pint = new int();

printf("%d\n", *pint);
*/