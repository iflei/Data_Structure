#pragma once
#include <vector>
using namespace std;

//标识某个位置是否存在元素
enum Status
{
	EXIST,
	DELETE,
	EMPTY
};

template <typename K, typename V>
struct HashNode
{
	HashNode(const K& key = K(), const V& value = V())
		: _key(key)
		, _value(value)
		, _status(EMPTY)
	{}

	K _key;
	V _value;
	Status _status;
};

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

template <typename K, typename V, typename HashFunc = HashFunc<K>>
class HashTable
{
	typedef HashNode<K, V> Node;
public:
	//构造函数
	HashTable(size_t size)
		:_size(0)
	{
		_table.resize(_GetNewSize(size));
	}

	pair<Node*, bool> Insert(const K& key, const V& value)
	{
		//检测容量
		_CheckCapacity();
		//寻找空位置
		size_t index = _HashFunc(key);

		while (_table[index]._status != EMPTY)
		{
			if (_table[index]._key == key)
			{
				//已存在
				if (_table[index]._status == EXIST)//不能再插入
					return make_pair(&_table[index], false);
				else//已删除
				{
					_table[index]._status = EXIST;
					++_size;
					return make_pair(&_table[index], true);
				}
			}

			++index;
			if (index == _table.size()) //index到数组末尾了
				index = 0;
		}
		//找到empty位置了
		_table[index]._key = key;
		_table[index]._value = value;
		_table[index]._status = EXIST;
		++_size;
		return make_pair(&_table[index], true);
	}

	Node* Find(const K& key)
	{
		size_t index = _HashFunc(key);
		while (_table[index]._status != EMPTY)
		{
			if (_table[index]._key == key)
			{
				if (_table[index]._status == EXIST)//存在
					return &_table[index];
				else//已删除
					return NULL;
			}
			++index;
			if (index == _table.size())
				index = 0;
		}
		//找到空节点了，说明key不存在
		return NULL;
	}

	bool Erase(const K& key)
	{
		Node* ret = Find(key);
		if (ret)
		{
			ret->_status = DELETE;
			return true;
		}
		return false;
	}

private:
	size_t _HashFunc(const K& key)
	{
		HashFunc hf;
		size_t hash = hf(key);
		return hash % _table.size();//除留余数法
	}

	void _CheckCapacity()
	{
		if (_size * 10 / _table.size() >= 7) //保证查找效率
		{
			HashTable<K, V, HashFunc> tmp(_table.size());//临时对象开新大小
			for (size_t i = 0; i < _table.size(); ++i)
			{
				if (_table[i]._status == EXIST)//表中存在的节点
					tmp.Insert(_table[i]._key, _table[i]._value);

			}
			_Swap(tmp);//现代写法
		}

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

	void _Swap(HashTable<K, V, HashFunc>& ht)
	{
		swap(_size, ht._size);
		_table.swap(ht._table);//vector的swap
	}

private:
	vector<Node> _table; 
	size_t _size; //当前元素数
};
