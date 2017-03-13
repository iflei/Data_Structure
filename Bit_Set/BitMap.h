#pragma once
#include <vector>
using namespace std;

class BitMap
{
public:
	BitMap(size_t num = 1024)
	{
		//需要多少位就可以表示多少个数的状态
		_array.resize((num >> 5) + 1); //bit / 32 + 1
	}

	//设置某个数存在
	void Set(size_t value)
	{//0 - 31  32 - 63
		size_t index = value >> 5; //这个数映射到数组里那个元素里
		size_t num = value % 32; //映射某个元素的哪位上

		_array[index] |= (1 << num);
	}
	//设置某个数不存在
	void ReSet(size_t value)
	{
		size_t index = value >> 5;
		size_t num = value % 32;

		_array[index] &= ~(1 << num);
	}
	//检测某个数是否存在
	bool Test(size_t value)
	{
		size_t index = value >> 5;
		size_t num = value % 32;

		return _array[index] & (1 << num);
	}

protected:
	vector<size_t> _array;
};