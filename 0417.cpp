#include "defDLL.h"
#include <stdio.h>
#include <Windows.h>

#define SUCCESS           			  1 // 执行成功						
#define ERROR			 -1 // 执行失败			         			

template<class T>
class TreeNode {
public:
	T element;					//当前节点存储的数据			
	TreeNode<T>* pLeft;					//指向左子节点的指针			
	TreeNode<T>* pRight;					//指向右子节点的指针			
	TreeNode<T>* pParent;					//指向父结点的指针			


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
	BSortTree();						//构造函数		
	~BSortTree();						//析构函数		
public:							//判断树是否为空		
	bool IsEmpty();						//新增节点		
	DWORD Insert(T element);						//删除节点		
	void Delete(T element);
	TreeNode<T>* Search(T element);						//查找节点		
	void InOrderTraverse(TreeNode<T>* pNode);						//中序遍历		
	void PreOrderTraverse(TreeNode<T>* pNode);						//前序遍历		
	void PostOrderTraverse(TreeNode<T>* pNode);						//后序遍历		
private:
	TreeNode<T>* GetMaxNode(TreeNode<T>* pNode);						//获取以pNode为根的最大节点		
	TreeNode<T>* GetMinNode(TreeNode<T>* pNode);						//获取以pNode为根的最小节点		
	TreeNode<T>* SearchNode(TreeNode<T>* pNode, T element);						//获取以pNode为根的最小节点		
	DWORD InsertNode(T element, TreeNode<T>* pNode);						//新增节点		
	TreeNode<T>* DeleteNode(T element, TreeNode<T>* pNode);						//删除节点		
	void Clear(TreeNode<T>* pNode);						//清空所有节点		
private:
	TreeNode<T>* m_pRoot;						//根结点指针		
	int size;						//树中元素总个数		
};
