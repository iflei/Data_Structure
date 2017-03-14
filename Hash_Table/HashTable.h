#pragma once
#include <vector>
using namespace std;

//��ʶĳ��λ���Ƿ����Ԫ��
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

template <typename K, typename V, typename HashFunc = HashFunc<K>>
class HashTable
{
	typedef HashNode<K, V> Node;
public:
	//���캯��
	HashTable(size_t size)
		:_size(0)
	{
		_table.resize(_GetNewSize(size));
	}

	pair<Node*, bool> Insert(const K& key, const V& value)
	{
		//�������
		_CheckCapacity();
		//Ѱ�ҿ�λ��
		size_t index = _HashFunc(key);

		while (_table[index]._status != EMPTY)
		{
			if (_table[index]._key == key)
			{
				//�Ѵ���
				if (_table[index]._status == EXIST)//�����ٲ���
					return make_pair(&_table[index], false);
				else//��ɾ��
				{
					_table[index]._status = EXIST;
					++_size;
					return make_pair(&_table[index], true);
				}
			}

			++index;
			if (index == _table.size()) //index������ĩβ��
				index = 0;
		}
		//�ҵ�emptyλ����
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
				if (_table[index]._status == EXIST)//����
					return &_table[index];
				else//��ɾ��
					return NULL;
			}
			++index;
			if (index == _table.size())
				index = 0;
		}
		//�ҵ��սڵ��ˣ�˵��key������
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
		return hash % _table.size();//����������
	}

	void _CheckCapacity()
	{
		if (_size * 10 / _table.size() >= 7) //��֤����Ч��
		{
			HashTable<K, V, HashFunc> tmp(_table.size());//��ʱ�����´�С
			for (size_t i = 0; i < _table.size(); ++i)
			{
				if (_table[i]._status == EXIST)//���д��ڵĽڵ�
					tmp.Insert(_table[i]._key, _table[i]._value);

			}
			_Swap(tmp);//�ִ�д��
		}

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

	void _Swap(HashTable<K, V, HashFunc>& ht)
	{
		swap(_size, ht._size);
		_table.swap(ht._table);//vector��swap
	}

private:
	vector<Node> _table; 
	size_t _size; //��ǰԪ����
};
