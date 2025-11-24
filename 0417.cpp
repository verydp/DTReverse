#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

#define SUCCESS           1			// 执行成功						
#define ERROR			 -1			// 执行失败			         			

template<class T>
class TreeNode {
public:
	T element;								//当前节点存储的数据			
	TreeNode<T>* pLeft;						//指向左子节点的指针			
	TreeNode<T>* pRight;					//指向右子节点的指针			
	TreeNode<T>* pParent;					//指向父结点的指针		


	TreeNode() {
		//初始化Node节点							
		memset(&element, 0, sizeof(TreeNode));
		//为元素赋值
		this->element = T();
		pLeft = pRight = pParent = NULL;
	}


	TreeNode(T& ele) {
		//初始化Node节点							
		memset(&element, 0, sizeof(TreeNode));
		//为元素赋值							
		memcpy(&element, &ele, sizeof(T));
		pLeft = pRight = pParent = NULL;
	}
		//重载== 比较两结点是否相等
	bool operator==(TreeNode<T>* node) {
		return node->element == element ? true : false;
	}
};

template<class T>
class BSortTree {
public:
	BSortTree();											//构造函数		
	~BSortTree();											//析构函数		
public:																
	bool IsEmpty();											//判断树是否为空		
	DWORD Insert(T element);								//新增节点		
	TreeNode<T>* Search(T element);							//查找节点	
	VOID Delete(T element);									//删除节点
	VOID InOrderTraverse(TreeNode<T>* pNode);				//中序遍历	
	VOID InOrderTraverse();
	VOID PreOrderTraverse(TreeNode<T>* pNode);				//前序遍历		
	VOID PostOrderTraverse(TreeNode<T>* pNode);				//后序遍历		
private:
	TreeNode<T>* GetMaxNode(TreeNode<T>* pNode);			//获取以pNode为根的最大节点		
	TreeNode<T>* GetMinNode(TreeNode<T>* pNode);			//获取以pNode为根的最小节点
	TreeNode<T>* SearchNode(TreeNode<T>* pNode, T element);	//获取以pNode为根的最小节点
	DWORD InsertNode(T element, TreeNode<T>* pNode);		//新增节点
	TreeNode<T>* DeleteNode(T element, TreeNode<T>* pNode);	//删除节点
	VOID Clear(TreeNode<T>* pNode);							//清空所有节点
private:
	TreeNode<T>* m_pRoot;									//根结点指针
	int size;												//树中元素总个数
};

template <class T>
BSortTree<T>::BSortTree() {

	this->m_pRoot = NULL;
	this->size = 0;
}

template <class T>
BSortTree<T>::~BSortTree() {
	Clear(this->m_pRoot);
}

template <class T>
bool BSortTree<T>::IsEmpty() {
	if (this->m_pRoot != NULL) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

template <class T>
DWORD BSortTree<T>::Insert(T element) {

	TreeNode<T>* ptrnode = new TreeNode<T>(element);

	if (this->m_pRoot == NULL) 
	{
		this->m_pRoot = ptrnode;
		this->size++;

		return SUCCESS;
	}

	return InsertNode(element, this->m_pRoot);
}

template <class T>//中序遍历
void BSortTree<T>::InOrderTraverse(TreeNode<T>* pNode) {
	if (pNode != NULL) {
		InOrderTraverse(pNode->pLeft);
		printf("%d\n", pNode->element);
		InOrderTraverse(pNode->pRight);
	}
}

template <class T>//中序遍历
void BSortTree<T>::InOrderTraverse() {
	TreeNode<T>* pNode = this->m_pRoot;
	if (pNode != NULL) {
		InOrderTraverse(pNode->pLeft);
		printf("%d\n", pNode->element);
		InOrderTraverse(pNode->pRight);
	}
}

template <class T>//前序遍历
void BSortTree<T>::PreOrderTraverse(TreeNode<T>* pNode) {
	if (pNode != NULL) {
		printf("%d\n", pNode->element);
		PreOrderTraverse(pNode->pLeft);
		PreOrderTraverse(pNode->pRight);
	}
}

template <class T>//后序遍历
void BSortTree<T>::PostOrderTraverse(TreeNode<T>* pNode) {
	if (pNode != NULL) {
		PostOrderTraverse(pNode->pLeft);
		PostOrderTraverse(pNode->pRight);
		printf("%d\n", pNode->element);
	}
}

template <class T>
TreeNode<T>* BSortTree<T>::GetMaxNode(TreeNode<T>* pNode) {
	if (pNode->pRight == NULL) {
		return pNode;
	}
	else {
		GetMaxNode(pNode->pRight);
	}
}

template <class T>
TreeNode<T>* BSortTree<T>::GetMinNode(TreeNode<T>* pNode) {
	if (pNode->pLeft == NULL) {
		return pNode;
	}
	else {
		GetMinNode(pNode->pLeft);
	}
}

template <class T>
VOID BSortTree<T>::Clear(TreeNode<T>* pNode) {
	if (pNode != NULL) {
		Clear(pNode->pLeft);
		Clear(pNode->pRight);
		//printf("%d\n", pNode->element);
		delete pNode;
		this->size--;
		pNode = NULL;
	}
}

template <class T>
DWORD BSortTree<T>::InsertNode(T element, TreeNode<T>* pNode) {

	TreeNode<T>* ptrNode = pNode;

	if (ptrNode->element == element)
	{
		return SUCCESS;
	}
	//父节点的值 < 新增的值。
	if (ptrNode->element < element) 
	{
		//需要将新增值向右边放置。
		if (ptrNode->pRight != NULL) 
		{
			InsertNode(element, ptrNode->pRight);
		}
		else 
		{
			TreeNode<T>* eleNode = new TreeNode<T>(element);
			ptrNode->pRight = eleNode;
			eleNode->pParent = ptrNode;
			this->size++;

			return SUCCESS;
		}
	}
	//父节点的值 > 新增的值
	else 
	{
		//需要将新增值向左边放置
		if (ptrNode->pLeft != NULL) 
		{
			InsertNode(element, ptrNode->pLeft);
		}
		else
		{
			TreeNode<T>* eleNode = new TreeNode<T>(element);
			ptrNode->pLeft = eleNode;
			eleNode->pParent = ptrNode;
			this->size++;
			return SUCCESS;
		}
	}
}

template <class T>
TreeNode<T>* BSortTree<T>::Search(T element) {
	return SearchNode(this->m_pRoot, element);
}

template <class T>
TreeNode<T>* BSortTree<T>::SearchNode(TreeNode<T>* pNode, T element) {
	if (pNode->element == element)
	{
		return pNode;
	}

	if (pNode->element < element)
	{
		if (pNode->pRight != NULL)
		{
			SearchNode(pNode->pRight, element);
		}
		else 
		{
			return NULL;
		}
	}
	else 
	{
		if (pNode->pLeft != NULL)
		{
			SearchNode(pNode->pLeft, element);
		}
		else
		{
			return NULL;
		}
	}
}

template <class T>
VOID BSortTree<T>::Delete(T element) {

	if (Search(element) != NULL)
	{
		TreeNode<T>* pNode = Search(element);
		DeleteNode(element, pNode);
	}
}

template <class T>
TreeNode<T>* BSortTree<T>::DeleteNode(T element, TreeNode<T>* pNode) {
	//没有子节点
	if (pNode->pLeft == NULL && pNode->pRight == NULL)
	{
		TreeNode<T>* parentNode = pNode->pParent;
		if (parentNode->pLeft == pNode)
		{
			delete pNode;
			this->size--;
			parentNode->pLeft = NULL;
			return NULL;
		}
		else
		{
			delete pNode;
			this->size--;
			parentNode->pRight = NULL;
			return NULL;
		}
	}

	/*
	情况3：左右子树都有		
		1、用右子树最小的节点取代源节点		
		2、再递归删除最小节点		
	*/
	if (pNode->pLeft != NULL && pNode->pRight != NULL)
	{
		TreeNode<T>* minNode = GetMinNode(pNode->pRight);

		pNode->element = minNode->element;

		return DeleteNode(minNode->element,minNode);
	}

/* 情况2：只有一个子树
		1、删除该节点	
		2、将父节点(左或者右)指针指向子树
******************************************************************
		10
	8		12				1：需要删除8，将9接入10的左节点
NULL	9

==================================================================
		10
	8		12				2：需要删除12，将14接入10的右节点
		NULL	14
******************************************************************/
	TreeNode<T>* BelowNode, *UpNode;

	pNode->pLeft == NULL ? BelowNode = pNode->pRight : BelowNode = pNode->pLeft;

	UpNode = pNode->pParent;

	UpNode->pLeft == pNode ? UpNode->pLeft = BelowNode : UpNode->pRight = BelowNode;

	BelowNode->pParent = UpNode;

	delete pNode;

	this->size--;

	return NULL;

}


int main() {
	BSortTree<int> Btree;

	Btree.Insert(10);										
	//													10
	Btree.Insert(8);
	//												8			14
	Btree.Insert(14);
	//											6		9	13		15
	Btree.Insert(6);
	//																	18
	Btree.Insert(9);

	Btree.Insert(15);

	Btree.Insert(18);

	Btree.Insert(13);

	Btree.InOrderTraverse();

	printf("Search(14) = %d\n", Btree.Search(14)->element);

	Btree.Delete(14);

	Btree.InOrderTraverse();

}