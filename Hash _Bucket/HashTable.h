#include <vector>
using namespace std;

//内置类型
template <typename K>
struct HashFunc
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

//特化string
template <>
struct HashFunc<string>
{
	static size_t BKDRHash(const char* str)
	{
		unsigned int seed = 131;
		unsigned int hash = 0;
		while (*str)
		{
			hash = hash * seed + (*str++);
		}
		return(hash & 0x7FFFFFFF);
	}

	size_t operator()(const string& str)
	{
		return BKDRHash(str.c_str());
	}
};

template <typename K, typename V>
struct HashNode
{
	HashNode(const pair<K, V>& kv)
		:_kv(kv)
		, _next(NULL)
		, _prev(NULL)
	{}

	pair<K, V> _kv;
	HashNode<K, V>* _next;
	HashNode<K, V>* _prev; //迭代器--操作
};

//声明
template <typename K, typename V, typename HashFunc = HashFunc<K>> class HashTable;

template <typename K, typename V, typename Ref, typename Ptr>
struct __HashTableIterator
{
	typedef HashNode<K, V> Node;
	typedef __HashTableIterator<K, V, Ref, Ptr> Self;

	__HashTableIterator(Node* node, HashTable<K, V>* ht)
		:_node(node)
		, _ht(ht)
	{}

	//Ref是pair&
	Ref operator*()
	{
		return _node->_kv;
	}
	//Ptr是&pair
	Ptr operator->()
	{
		return &_node->_kv;
	}

	//Self是迭代器类本身，Self&是当前对象的引用
	Self& operator++() //前置++
	{
		_node = _Next(_node);
		return *this;
	}

	Self& operator--() //前置--
	{
		_node = _Prev(_node);
		return *this;
	}

	bool operator!=(__HashTableIterator<K, V, Ref, Ptr>& it)
	{
		return _node != it._node;
	}
	
protected:
	Node* _Next(Node* node)
	{
		Node* next = _node->_next;
		if (next) //如果next存在直接返回
		{
			return next;
		}
		else //如果next不存在，从下一个_table[index]开始找非空节点
		{
			size_t index = _ht->_HashFunc(node->_kv.first) + 1;

			for (; index < _ht->_table.size(); ++index)
			{
				next = _ht->_table[index];
				if (next)
				{
					return next;
				}
			}
		}
		//到这里next为空，而且index==_ht->_table.size()没进入for循环
		return NULL;
	}

	Node* _Prev(Node* node)
	{
		Node* prev = _node->_prev;
		if (prev) //如果prev存在直接返回
		{
			return prev;
		}
		else //如果prev不存在，从上一个_table[index]开始找非空节点
		{
			int index = _ht->_HashFunc(node->_kv.first) - 1;
			for (; index >= 0; --index)
			{
				prev = _ht->_table[index];
				if (prev)
				{
					return prev;
				}
			}
		}
		//到这里prev为空，而且index < 0 没进入for循环
		return NULL;
	}
private:
	Node* _node;//指向Node
	HashTable<K, V>* _ht; //通过this指针传，需要访问HashTable对象的_size, _table和_HashFunc()
};

template <typename K, typename V, typename HashFunc = HashFunc<K>>
class HashTable
{
	typedef HashNode<K, V> Node;
public:
	typedef __HashTableIterator<K, V, pair<K, V>&, pair<K, V>*> Iterator;
	typedef __HashTableIterator<K, V, const pair<K, V>&, const pair<K, V>*> ConstIterator;
	//__HashTableIterator类中需要访问_HashFunc保护成员，所以设为友元
	friend struct Iterator;
	friend struct ConstIterator;
public:
	HashTable(size_t size)
		:_size(0)
	{
		_table.resize(_GetNewSize(size));
	}

	~HashTable()
	{
		//清理哈希桶
		for (size_t i = 0; i < _table.size(); ++i)
		{
			Node* cur = _table[i];
			while (cur)
			{
				Node* del = cur;
				cur = cur->_next;
				delete del;
			}
			_table[i] = NULL;
		}
	}

	pair<Iterator, bool> Insert(const pair<K, V>& kv)
	{
		//检查容量
		_CheckCapacity();

		size_t index = _HashFunc(kv.first);
		//遍历链表检查是否已存在
		Node* cur = _table[index];
		while (cur)
		{
			if (cur->_kv.first == kv.first)
				return make_pair(Iterator(cur, this), false);

			cur = cur->_next;
		}

		//不存在进行头插
		Node* tmp = new Node(kv);
		tmp->_next = _table[index];
		_table[index] = tmp;

		++_size;
		return make_pair(Iterator(tmp, this), true);
	}

	Iterator Find(const K& key)
	{
		size_t index = _HashFunc(key);
		Node* cur = _table[index];
		while (cur)
		{
			if (cur->_kv.first == key)
				return Iterator(cur, this);

			cur = cur->_next;
		}
		return Iterator(NULL, this);
	}

	bool Erase(const K& key)
	{
		size_t index = _HashFunc(key);
		Node* prev = NULL;
		Node* cur = _table[index];

		while (cur)
		{
			//找到了
			if (cur->_kv.first == key)
			{
				//删除头节点情况
				if (prev == NULL)
				{
					delete cur;
					_table[index] = NULL;
				}
				else
				{
					prev->_next = cur->_next;
					delete cur;
				}
				return true;
			}

			prev = cur;
			cur = cur->_next;
		}
		//没找到
		return false;
	}

	Iterator Begin()
	{
		for (size_t i = 0; i < _table.size(); ++i)
		{
			if (_table[i]) //_table里第一个非空指针
			{
				return Iterator(_table[i], this);
			}
		}
		return Iterator(NULL, this);
	}

	Iterator End()
	{
		return Iterator(NULL, this);
	}

	V& operator[](const K& key)
	{
		pair<Iterator, bool> ret = Insert(make_pair(key, V()));
		return ret.first->second;
		//ret.first是Iterator，Iterator里重载了->
	}
protected:
	void _CheckCapacity()
	{
		//负载因子等于1需要扩容，平均每个节点下挂一个
		if (_size == _table.size())
		{
			//计算新size由构造函数完成
			HashTable<K, V, HashFunc> tmp(_table.size());
			for (size_t i = 0; i < _table.size(); ++i)
			{
				if (_table[i]) //下面挂有节点
				{
					Node* cur = _table[i];
					while (cur)
					{
						//深拷贝的工作在Insert中做的
						tmp.Insert(cur->_kv);
						cur = cur->_next;
					}
				}
			}
			_Swap(tmp);//现代写法
		}
	}

	void _Swap(HashTable<K, V, HashFunc>& ht)
	{
		swap(_size, ht._size); //_size其实是一样的
		_table.swap(ht._table);//vector的swap
	}

	size_t _HashFunc(const K& key)
	{
		HashFunc hf;
		size_t hash = hf(key);
		return hash % _table.size();
	}

	size_t _GetNewSize(size_t num) //素数做哈希表大小能减少哈希冲突
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] = {
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul,
			25165843ul, 50331653ul, 100663319ul, 201326611ul, 402653189ul,
			805306457ul, 1610612741ul, 3221225473ul, 4294967291ul
		};
		for (int i = 0; i < _PrimeSize; ++i)
		{
			if (_PrimeList[i] > num)
				return _PrimeList[i];
		}
		return _PrimeList[_PrimeSize - 1];
	}
private:
	vector<Node*> _table;
	size_t _size;
};
