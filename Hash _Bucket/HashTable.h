#include <vector>
using namespace std;

//��������
template <typename K>
struct HashFunc
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

//�ػ�string
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
	HashNode<K, V>* _prev; //������--����
};

//����
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

	//Ref��pair&
	Ref operator*()
	{
		return _node->_kv;
	}
	//Ptr��&pair
	Ptr operator->()
	{
		return &_node->_kv;
	}

	//Self�ǵ������౾��Self&�ǵ�ǰ���������
	Self& operator++() //ǰ��++
	{
		_node = _Next(_node);
		return *this;
	}

	Self& operator--() //ǰ��--
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
		if (next) //���next����ֱ�ӷ���
		{
			return next;
		}
		else //���next�����ڣ�����һ��_table[index]��ʼ�ҷǿսڵ�
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
		//������nextΪ�գ�����index==_ht->_table.size()û����forѭ��
		return NULL;
	}

	Node* _Prev(Node* node)
	{
		Node* prev = _node->_prev;
		if (prev) //���prev����ֱ�ӷ���
		{
			return prev;
		}
		else //���prev�����ڣ�����һ��_table[index]��ʼ�ҷǿսڵ�
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
		//������prevΪ�գ�����index < 0 û����forѭ��
		return NULL;
	}
private:
	Node* _node;//ָ��Node
	HashTable<K, V>* _ht; //ͨ��thisָ�봫����Ҫ����HashTable�����_size, _table��_HashFunc()
};

template <typename K, typename V, typename HashFunc = HashFunc<K>>
class HashTable
{
	typedef HashNode<K, V> Node;
public:
	typedef __HashTableIterator<K, V, pair<K, V>&, pair<K, V>*> Iterator;
	typedef __HashTableIterator<K, V, const pair<K, V>&, const pair<K, V>*> ConstIterator;
	//__HashTableIterator������Ҫ����_HashFunc������Ա��������Ϊ��Ԫ
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
		//�����ϣͰ
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
		//�������
		_CheckCapacity();

		size_t index = _HashFunc(kv.first);
		//�����������Ƿ��Ѵ���
		Node* cur = _table[index];
		while (cur)
		{
			if (cur->_kv.first == kv.first)
				return make_pair(Iterator(cur, this), false);

			cur = cur->_next;
		}

		//�����ڽ���ͷ��
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
			//�ҵ���
			if (cur->_kv.first == key)
			{
				//ɾ��ͷ�ڵ����
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
		//û�ҵ�
		return false;
	}

	Iterator Begin()
	{
		for (size_t i = 0; i < _table.size(); ++i)
		{
			if (_table[i]) //_table���һ���ǿ�ָ��
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
		//ret.first��Iterator��Iterator��������->
	}
protected:
	void _CheckCapacity()
	{
		//�������ӵ���1��Ҫ���ݣ�ƽ��ÿ���ڵ��¹�һ��
		if (_size == _table.size())
		{
			//������size�ɹ��캯�����
			HashTable<K, V, HashFunc> tmp(_table.size());
			for (size_t i = 0; i < _table.size(); ++i)
			{
				if (_table[i]) //������нڵ�
				{
					Node* cur = _table[i];
					while (cur)
					{
						//����Ĺ�����Insert������
						tmp.Insert(cur->_kv);
						cur = cur->_next;
					}
				}
			}
			_Swap(tmp);//�ִ�д��
		}
	}

	void _Swap(HashTable<K, V, HashFunc>& ht)
	{
		swap(_size, ht._size); //_size��ʵ��һ����
		_table.swap(ht._table);//vector��swap
	}

	size_t _HashFunc(const K& key)
	{
		HashFunc hf;
		size_t hash = hf(key);
		return hash % _table.size();
	}

	size_t _GetNewSize(size_t num) //��������ϣ���С�ܼ��ٹ�ϣ��ͻ
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
