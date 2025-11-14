#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

class Monster
{
public:
	int ID;
	int Level;
	char Name[20];
public:
	Monster() 
	{
		this->ID = 0;
		this->Level = 0;
		memset(this->Name, 0, 20);
	}
	Monster(int ID, int Level, const char* Name)
	{
		this->ID = ID;
		this->Level = Level;
		memcpy(this->Name, Name, strlen(Name) + 1);
	}
};

template<class T>
class TreeNode {
public:
	T element;										//当前节点存储的数据		
	TreeNode<T>* pLeft;								//指向左子节点的指针		
	TreeNode<T>* pRight;							//指向右子节点的指针		

	TreeNode(T& ele) {
		//初始化Node节点
		memset(&element, 0, sizeof(TreeNode));
		//为元素赋值
		memcpy(&element, &ele, sizeof(T));
		pLeft = pRight = NULL;
	}
};

template<class T>
class BSortTree {
public:
	BSortTree();										//构造函数
	~BSortTree();										//析构函数
public:
	VOID InOrderTraverse(TreeNode<T>* pNode);			//中序遍历(左 根 右)
	VOID PreOrderTraverse(TreeNode<T>* pNode);			//前序遍历(根 左 右)
	VOID PostOrderTraverse(TreeNode<T>* pNode);			//后序遍历(左 右 根)
	TreeNode<T>* GetRoot();								//返回根节点
	int GetDepth(TreeNode<T>* pNode);					//返回某个节点的高度/深度
	VOID Clear(TreeNode<T>* pNode);						//删除某个节点；
private:
	VOID Init();
private:
	TreeNode<T>* m_pRoot;								//根结点指针
	int size;											//树中元素总个数
};

//构造函数
template<class T>
BSortTree<T>::BSortTree() {
	/*
	TreeNode<T>* pNode;
	pNode.element = T();
	pNode.pLeft = NULL;
	pNode.pRight = NULL;

	m_pRoot = pNode;
	this->size++;
	*/
	
	this->Init();
}

//析构函数
template<class T>
BSortTree<T>::~BSortTree() {
	TreeNode<T>* pnode;

	pnode = this->m_pRoot;

	Clear(pnode);

	this->m_pRoot = NULL;

}

//中序遍历(左 根 右)
template<class T>
VOID BSortTree<T>::InOrderTraverse(TreeNode<T>* pNode) {
	if (pNode != NULL) 
	{
		InOrderTraverse(pNode->pLeft);
		printf("%d\n", pNode->element.ID);
		InOrderTraverse(pNode->pRight);
	}
}

//前序遍历(根 左 右)
template<class T>
void BSortTree<T>::PreOrderTraverse(TreeNode<T>* pNode) {
	if (pNode != NULL)
	{
		printf("%d\n", pNode->element.ID);
		PreOrderTraverse(pNode->pLeft);
		PreOrderTraverse(pNode->pRight);
	}
}

//后序遍历(左 右 根)
template<class T>
void BSortTree<T>::PostOrderTraverse(TreeNode<T>* pNode) {
	if (pNode != NULL)
	{
		PostOrderTraverse(pNode->pLeft);
		PostOrderTraverse(pNode->pRight);
		printf("%d\n", pNode->element.ID);
	}
}

//返回根节点
template<class T>
TreeNode<T>* BSortTree<T>::GetRoot() {
	return this->m_pRoot;
}

//返回某个节点的高度/深度
template<class T>
int BSortTree<T>::GetDepth(TreeNode<T>* pNode) {
	if (pNode == NULL)
	{
		return 0;
	}
	else
	{
		int m = GetDepth(pNode->pLeft);
		int n = GetDepth(pNode->pRight);
		return (m > n) ? (m + 1) : (n + 1);
	}
}

template<class T>
VOID BSortTree<T>::Clear(TreeNode<T>* pNode)
{
	TreeNode<T>* pnode;
	pnode = pNode;

	if (pnode != NULL)
	{
		Clear(pnode->pLeft);
		Clear(pnode->pRight);
		delete pnode;
		this->size--;
	}
}

template<class T>
VOID BSortTree<T>::Init()
{
	Monster m1(1, 1, "刺猬");
	Monster m2(2, 2, "野狼");
	Monster m3(3, 3, "野猪");
	Monster m4(4, 4, "士兵");
	Monster m5(5, 5, "火龙");
	Monster m6(6, 6, "独角兽");
	Monster m7(7, 7, "江湖大盗");

	TreeNode<Monster>* n1 = new TreeNode<Monster>(m1);
	TreeNode<Monster>* n2 = new TreeNode<Monster>(m2);
	TreeNode<Monster>* n3 = new TreeNode<Monster>(m3);
	TreeNode<Monster>* n4 = new TreeNode<Monster>(m4);
	TreeNode<Monster>* n5 = new TreeNode<Monster>(m5);
	TreeNode<Monster>* n6 = new TreeNode<Monster>(m6);
	TreeNode<Monster>* n7 = new TreeNode<Monster>(m7);

	m_pRoot = n5;
	n5->pLeft = n4;
	n5->pRight = n6;
	n4->pLeft = n1;
	n1->pRight = n2;
	n6->pLeft = n3;
	n3->pRight = n7;
	size = 7;
}

int main() {
	BSortTree<Monster> test;

	TreeNode<Monster>* pnode;

	pnode = test.GetRoot();

	test.InOrderTraverse(pnode);

	//printf("%d\n", test.GetDepth(pnode));


}