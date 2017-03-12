#pragma once
#include <vector>
using namespace std;

class UnionFind
{
public:
	//每个元素都是一个集合，集合大小用负数表示（负数表示根）
	UnionFind(const size_t n)
	{
		_set.resize(n, -1);
	}

	//查找元素的根
	int Root(size_t x)
	{
		while (_set[x] >= 0)
			x = _set[x];

		return x;
	}

	////查找两个数是不是一个集合
	bool Find(const int x1, const int x2)
	{
		return Root(x1) == Root(x2);
	}

	//合并并查集
	void Union(size_t x1, size_t x2)
	{
		//首先得到两个元素的根
		int root1 = Root(x1);
		int root2 = Root(x2);

		if (root1 != root2)
		{
			_set[root1] += _set[root2];//把集合2的元素个数加到集合1上
			_set[root2] = root1;//root2的父亲是root1
		}
	}

	//求有多少个集合
	int CountRoot()
	{
		int count = 0;
		for (int i = 0; i < _set.size(); ++i)
		{
			if (_set[i] < 0)
			{
				++count;
			}
				
		}

		return count;
	}
private:
	vector<int> _set;
};