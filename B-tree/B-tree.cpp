#include <iostream>
using namespace std;

template <typename K, size_t M>
struct BTreeNode
{
	K _keys[M]; //key集合，多留一个关键字位置，简化分裂
	BTreeNode<K, M>* _subs[M+1]; //孩子集合
	BTreeNode<K, M>* _parent;
	size_t _size; //有效关键字个数
	
	BTreeNode():_size(0), _parent(NULL) {
		for(int i = 0; i < M+1; ++i)
			_subs[i] = NULL;
	}
};

template <typename K, size_t M>
class BTree
{
	typedef BTreeNode<K, M> Node;
public:
	BTree() :_root(NULL) {
	}

	~BTree() {
		_Destroy(_root);
	}

	//查找值为key的结点
	pair<Node*, int> Find(const K& key) {
		Node* cur = _root;
		Node* parent = NULL;
		while(cur) 
		{
			size_t i = 0;
			//遍历当前节点的key
			while(i < cur->_size) 
			{
				//要查找的key小于当前节点key
				if(key < cur->_keys[i])
				  break;
				else if(key > cur->_keys[i])
				  ++i; //和当前节点下一个key比较
				else
				  return pair<Node*, int>(cur, i); //找到了
			}

			parent = cur;
			cur = cur->_subs[i]; //跳到合适的孩子节点
		}

		return pair<Node*, int>(parent, -1); //没找到返回最后查找的叶子结点和-1
	}

	//插入
	bool Insert(const K& key) {
		//空树
		if(_root == NULL) {
			_root = new Node;
			_root->_keys[0] = key;
			_root->_size = 1;
			_root->_parent = NULL;
			return true;
		}

		//寻找key插入的节点
		pair<Node*, int> pos = Find(key);

		//key已经存在直接退出
		if(pos.second != -1) {
			return false;
		}

		//插入
		Node* cur = pos.first;
		Node* sub = NULL;

		K k = key;
		while(1) {
			_InsertKey(cur, k, sub); //插入key和右孩子
			if(cur->_size < M)
			  return true;
			
			//分裂节点 cur->_size == M
			size_t mid = cur->_size / 2; //中间靠右
			Node* newNode = new Node;
			
			size_t j = 0;
			size_t i = mid+1;
			//mid以右key移到新节点
			for (; i < cur->_size; ++i, ++j) {
				newNode->_keys[j] = cur->_keys[i]; //把mid右边的key移到新节点
				newNode->_subs[j] = cur->_subs[i]; //复制左孩子
				if(newNode->_subs[j])
				  newNode->_subs[j]->_parent = newNode; //指向新节点

				newNode->_size++;
			}
			//复制最后一个右孩子
			newNode->_subs[j] = cur->_subs[i];
			if(newNode->_subs[j])
			  newNode->_subs[j]->_parent = newNode;

			cur->_size = cur->_size - newNode->_size - 1; //更新分裂后的size

			//如果分裂的节点为根节点
			if(cur == _root) {
				_root = new Node;
				_root->_keys[0] = cur->_keys[mid]; //把mid提升到新_root节点
				_root->_size = 1;
				_root->_subs[0] = cur; //左孩子是cur
				cur->_parent = _root;
				_root->_subs[1] = newNode; //右孩子是newNode
				newNode->_parent = _root;
				return true;
			}
			else { //将mid插入到父节点
				k = cur->_keys[mid];
				cur = cur->_parent;
				sub = newNode;
			}
		}
	}

	//中序遍历
	void InOrder() {
		cout << "InOrder: ";
		_InOrder(_root);
		cout << endl;
	}
private:
	void _InsertKey(Node* cur, const K& key, Node* sub) {
		int end = cur->_size - 1;
		while(end >= 0) {
			if(key < cur->_keys[end]) {
				cur->_keys[end+1] = cur->_keys[end]; //key 向后移动
				cur->_subs[end+2] = cur->_subs[end+1]; //key的右孩子也向后移动
			}
			else //key > cur->_keys[end]
			  break;

			--end;
		}

		cur->_keys[end+1] = key; //插入key
		cur->_subs[end+2] = sub; //插入右孩子
		cur->_size++;

		if(sub) {
			sub->_parent = cur;
		}
	}

	//中序遍历
	void _InOrder(Node* root) {
		if(root) {
			for(size_t i = 0; i < root->_size; ++i) {
				_InOrder(root->_subs[i]); //递归左
				cout << root->_keys[i] << " "; //打印key
			}
			_InOrder(root->_subs[root->_size]); //递归最右边的孩子
		}
	}

	void _Destroy(Node* root) {
		if(root == NULL)
		  return;

		//0 -- root->_size 销毁所有孩子节点
		for(size_t i = 0; i <= root->_size; ++i)
		  _Destroy(root->_subs[i]);

		//删除当前节点
		delete root;
	}
private:
	Node* _root;
};


int main()
{
	BTree<int, 3> _bt;
	_bt.Insert(10);
	_bt.Insert(30);
	_bt.Insert(20);
	_bt.Insert(40);
	_bt.Insert(50);
	_bt.Insert(38);
	_bt.Insert(35);
	_bt.InOrder();
	return 0;
}
