#pragma once
#include <assert.h>
#include <iostream>
#include <stack>
using namespace std;

template <typename T>
struct BinaryTreeNode
{
	BinaryTreeNode(const T& x = T(0))
		:_left(NULL)
		, _right(NULL)
		, _data(x)
	{}

	BinaryTreeNode<T>* _left;
	BinaryTreeNode<T>* _right;
	T _data;
};

template <typename T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
public:
	//二叉树创建
	BinaryTree(const T* a, size_t size, const T& invalid = T())
	{
		assert(a);
		size_t index = 0;
		_root = _CreatTree(a, size, index, invalid);
	}

	~BinaryTree()
	{
		_Destory(_root);
	}

	//递归遍历
	void PrevOrder()
	{
		cout << "PrevOrder: ";
		_PrevOrder(_root);
		cout << endl;
	}

	void InOrder()
	{
		cout << "InOrder: ";
		_InOrder(_root);
		cout << endl;
	}

	void PostOrder()
	{
		cout << "PostOrder: ";
		_PostOrder(_root);
		cout << endl;
	}

	//非递归遍历
	void PrevOrderNonR()
	{
		cout << "PrevOrder: ";
		stack<Node*> s;
		if (_root)
			s.push(_root);

		while (!s.empty())
		{
			//访问栈顶元素
			Node* top = s.top();
			cout << top->_data << " ";
			s.pop();

			//先访问左节点，所以先把右节点入栈
			if (top->_right)
				s.push(top->_right);
			if (top->_left)
				s.push(top->_left);
		}
		cout << endl;
	}

	void InOrderNonR()
	{
		cout << "InOrder: ";
		stack<Node*> s;

		Node* cur = _root;
		while (cur || !s.empty())
		{
			//把左全入栈
			while (cur)
			{
				s.push(cur);
				cur = cur->_left;
			}

			if (!s.empty())
			{
				Node* top = s.top();
				cout << top->_data << " ";
				cur = top->_right; //当前节点已经访问，让cur跳到右树
				s.pop();
			}
		}
		cout << endl;
	}

	void PostOrderNonR()
	{
		cout << "PostOrder: ";
		stack<Node*> s;
		Node* cur = _root;
		Node* prev = NULL;

		while (cur || !s.empty())
		{
			while (cur)
			{
				s.push(cur);
				cur = cur->_left;
			}

			Node* top = s.top();
			//如果栈顶结点右子树为空，或右子树已经访问过
			if (top->_right == NULL || top->_right == prev)
			{
				cout << top->_data << " ";
				prev = top; //prev标记此节点已经访问过
				s.pop();
			}
			else //右子树未访问过
				cur = top->_right;
		}
		cout << endl;
	}

	void LevelOrder()//队列实现层次遍历,非递归
	{
		queue<Node*> q;//建立队列存放Note*类型值
		if (_root != NULL)
		{
			q.push(_root);
		}
		cout << "LevelOrder: ";
		while (!q.empty())
		{
			Node* front = q.front();
			cout << front->_data << " ";//访问队头
			q.pop();//队头出队列
			if (front->_left != NULL)
			{
				q.push(front->_left);
			}
			if (front->_right != NULL)
			{
				q.push(front->_right);
			}
		}
		cout << endl;
	}

	size_t GetKLevel(int k)
	{
		assert(k > 0);
		return _GetKLevel(k, _root);
	}

protected:

	void _PrevOrder(Node* root)
	{
		if (root == NULL)
			return;

		cout << root->_data << " ";
		_PrevOrder(root->_left);
		_PrevOrder(root->_right);
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_data << " ";
		_InOrder(root->_right);
	}

	void _PostOrder(Node* root)
	{
		if (root == NULL)
			return;

		_PostOrder(root->_left);
		_PostOrder(root->_right);
		cout << root->_data << " ";
	}

	//第k层结点个数
	size_t _GetKLevel(int k, Node* root)
	{
		if (root == NULL)
		{
			return 0;
		}
		if (k == 1)
		{
			return 1;
		}
		size_t leftSize = _GetKLevel(k - 1, root->_left);
		size_t rightSize = _GetKLevel(k - 1, root->_right);
		return leftSize + rightSize;
	}

	Node* _CreatTree(const T* a, size_t size, size_t& index, const T& invalid)
	{
		Node* root = NULL;
		//防数组越界，如果是空节点直接返回NULL
		if (index < size && a[index] != invalid)
		{
			//前序
			root = new Node(a[index]);
			//递归建立左子树
			root->_left = _CreatTree(a, size, ++index, invalid);
			root->_right = _CreatTree(a, size, ++index, invalid);
		}
		return root;
	}

	void _Destory(Node* root)
	{
		if (root == NULL)
			return;

		_Destory(root->_left);
		_Destory(root->_right);
		delete root;
		root = NULL;
	}
protected:
	Node* _root;
};