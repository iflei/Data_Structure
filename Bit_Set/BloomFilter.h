#pragma once
#include <string>
#include "BitMap.h"
#include "HashFunc.h"
using namespace std;

template<typename K = string,
typename HashFunc1 = __HashFunc1<K>,
typename HashFunc2 = __HashFunc2<K>,
typename HashFunc3 = __HashFunc3<K>,
typename HashFunc4 = __HashFunc4<K>,
typename HashFunc5 = __HashFunc5<K>>
class BloomFilter //布隆过滤器属于哈希，底层用位图节省内存
{
public:
	BloomFilter(size_t n = 1024)
		:_size(_GetNextPrime(n)) //_size设为素数减少hash冲突
		, _bm(_size)
	{}

	void Set(const K& key)
	{
		//用不同的哈希函数把key转化为不同的哈希值
		size_t hash1 = HashFunc1()(key);
		size_t hash2 = HashFunc2()(key);
		size_t hash3 = HashFunc3()(key);
		size_t hash4 = HashFunc4()(key);
		size_t hash5 = HashFunc5()(key);
		//把不同哈希值都映射到位图里表示一个key是否存在
		_bm.Set(hash1%_size);
		_bm.Set(hash2%_size);
		_bm.Set(hash3%_size);
		_bm.Set(hash4%_size);
		_bm.Set(hash5%_size);
	}

	//用同样的哈希函数转化key，只有所有哈希值都在位图里存在，这个key才可能存在。
	bool Test(const K& key)
	{
		size_t hash1 = HashFunc1()(key);
		if (!_bm.Test(hash1%_size))
			return false;

		size_t hash2 = HashFunc2()(key);
		if (!_bm.Test(hash2%_size))
			return false;

		size_t hash3 = HashFunc3()(key);
		if (!_bm.Test(hash3%_size))
			return false;

		size_t hash4 = HashFunc4()(key);
		if (!_bm.Test(hash4%_size))
			return false;

		size_t hash5 = HashFunc5()(key);
		if (!_bm.Test(hash5%_size))
			return false;

		return true;
	}
	
	//从素数表获取素数
	size_t _GetNextPrime(size_t size)
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};

		for (size_t i = 0; i < _PrimeSize; ++i)
		{
			if (_PrimeList[i] > size)
			{
				return _PrimeList[i];
			}
		}

		return _PrimeList[_PrimeSize - 1];
	}
protected:
	BitMap _bm;
	size_t _size;
};