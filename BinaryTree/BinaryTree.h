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
	//����������
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

	//�ݹ����
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

	//�ǵݹ����
	void PrevOrderNonR()
	{
		cout << "PrevOrder: ";
		stack<Node*> s;
		if (_root)
			s.push(_root);

		while (!s.empty())
		{
			//����ջ��Ԫ��
			Node* top = s.top();
			cout << top->_data << " ";
			s.pop();

			//�ȷ�����ڵ㣬�����Ȱ��ҽڵ���ջ
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
			//����ȫ��ջ
			while (cur)
			{
				s.push(cur);
				cur = cur->_left;
			}

			if (!s.empty())
			{
				Node* top = s.top();
				cout << top->_data << " ";
				cur = top->_right; //��ǰ�ڵ��Ѿ����ʣ���cur��������
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
			//���ջ�����������Ϊ�գ����������Ѿ����ʹ�
			if (top->_right == NULL || top->_right == prev)
			{
				cout << top->_data << " ";
				prev = top; //prev��Ǵ˽ڵ��Ѿ����ʹ�
				s.pop();
			}
			else //������δ���ʹ�
				cur = top->_right;
		}
		cout << endl;
	}

	void LevelOrder()//����ʵ�ֲ�α���,�ǵݹ�
	{
		queue<Node*> q;//�������д��Note*����ֵ
		if (_root != NULL)
		{
			q.push(_root);
		}
		cout << "LevelOrder: ";
		while (!q.empty())
		{
			Node* front = q.front();
			cout << front->_data << " ";//���ʶ�ͷ
			q.pop();//��ͷ������
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

	//��k�������
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
		//������Խ�磬����ǿսڵ�ֱ�ӷ���NULL
		if (index < size && a[index] != invalid)
		{
			//ǰ��
			root = new Node(a[index]);
			//�ݹ齨��������
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