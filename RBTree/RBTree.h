#pragma once
using namespace std;

enum Color
{
	RED,
	BLACK
};

template <typename K, typename V>
struct RBTreeNode
{
	RBTreeNode(const K& key, const V& value, Color color = RED)
	: _key(key)
	, _value(value)
	, _color(color)
	, _left(NULL)
	, _right(NULL)
	, _parent(NULL)
	{}

	K _key;
	V _value;
	Color _color;
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
};

template <typename K, typename V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}

	pair<Node*, bool> Insert(const K& key, const V& value)
	{
		//1. 如果是空树直接创建一个黑色节点
		if (_root == NULL)
		{
			_root = new Node(key, value, BLACK);
			//return true;
			return pair<Node*, bool>(_root, true);
		}

		Node* parent = NULL;
		Node* cur = _root;
		//2. 找到插入的位置
		while (cur)
		{
			if (key > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else //已存在则不插入
			{
				//return false;
				return pair<Node*, bool>(cur, false);
			}
		}
		//3. 根据搜索树规则插入新节点
		cur = new Node(key, value);
		if (key > parent->_key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		//4. 根据颜色调整结构及颜色，满足红黑树规则
		Node* newNode = cur;
		while (cur != _root && parent->_color == RED) //如果没调整到根节点或父节点是红色就继续调整
		{
			Node* grandfather = parent->_parent;

			if (parent == grandfather->_left) //如果父结点是祖父的左孩子
			{
				Node* uncle = grandfather->_right;
				//情况1：叔叔节点存在且为红色
				if (uncle && uncle->_color == RED)
				{
					//p，u改为黑色，g改为红色
					parent->_color = BLACK;
					uncle->_color = BLACK;
					grandfather->_color = RED;
					//以grandfather为当前节点向上继续调整
					cur = grandfather;
					parent = cur->_parent;
				}
				else //情况2：叔叔节点不存在或叔叔节点为黑
				{
					//情况3：cur为p的右孩子，则以p为轴，进行左单旋转，转换为情况2
					if (cur == parent->_right)
					{
						_RotateL(parent);
						swap(cur, parent);//旋转以后cur升上去了，所以需要交换cur和parent
					}

					//情况2：以grandfather右单旋，p变黑，g变红
					parent->_color = BLACK;
					grandfather->_color = RED;
					_RotateR(grandfather);
				}
			}
			else //如果父结点是祖父的右孩子(和上面情况一样，只是旋转方向变了)
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_color == RED)
				{
					parent->_color = BLACK;
					uncle->_color = BLACK;
					grandfather->_color = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)
					{
						_RotateR(parent);
						swap(cur, parent);
					}

					parent->_color = BLACK;
					grandfather->_color = RED;
					_RotateL(grandfather);
				}
			}
		}
		_root->_color = BLACK;//根节点是黑色
		//return true;
		return pair<Node*, bool>(newNode, true);
	}

	V& operator[](const K& key)
	{
		pair<Node*, bool> ret = Insert(key, V());
		return ret.first->_value;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	bool IsBalace()
	{
		//空树返回true
		if (_root == NULL)
			return true;
		//根节点为红返回false
		if (_root->_color == RED)
		{
			return false;
		}
		//计算其中一条路径上的黑色节点数
		int k = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_color == BLACK)
				k++;

			cur = cur->_left;
		}
		//
		int count = 0;
		_IsBalance(_root, k, count);
	}

private:
	//左单旋
	void _RotateL(Node* parent)
	{
		// 提升右孩子
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		// 1.右孩子的左子树交给父节点的右
		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		// 2.父节点变成右孩子的左
		subR->_left = parent;
		subR->_parent = parent->_parent;

		Node* ppNode = parent->_parent;
		parent->_parent = subR;

		if (ppNode == NULL)
		{
			_root = subR;
			subR->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
				ppNode->_left = subR;
			else
				ppNode->_right = subR;

			subR->_parent = ppNode;
		}
	}
	//右单旋
	void _RotateR(Node* parent)
	{
		// 提升左孩子
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		subL->_right = parent;
		subL->_parent = parent->_parent;

		Node* ppNode = parent->_parent;
		parent->_parent = subL;

		parent = subL;

		if (ppNode == NULL)
		{
			_root = subL;
			subL->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
				ppNode->_left = subL;
			else
				ppNode->_right = subL;

			subL->_parent = ppNode;
		}
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

	bool _IsBalance(Node* root, const int k, int count)
	{
		if (root == NULL)
			return true;

		//检查是否存在连续红节点
		if (root->_color == RED && root->_parent->_color == RED)
			return false;
		//统计黑节点数
		if (root->_color == BLACK)
			++count;

		//到叶子节点时检测黑色节点数是否和k相等
		if (root->_left == NULL && root->_right == NULL && k != count)
			return false;

		//递归左子树和右子树
		return _IsBalance(root->_left, k, count) && _IsBalance(root->_right, k, count);
	}
private:
	Node* _root;
};