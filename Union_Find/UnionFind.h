#pragma once
#include <vector>
using namespace std;

class UnionFind
{
public:
	//ÿ��Ԫ�ض���һ�����ϣ����ϴ�С�ø�����ʾ��������ʾ����
	UnionFind(const size_t n)
	{
		_set.resize(n, -1);
	}

	//����Ԫ�صĸ�
	int Root(size_t x)
	{
		while (_set[x] >= 0)
			x = _set[x];

		return x;
	}

	////�����������ǲ���һ������
	bool Find(const int x1, const int x2)
	{
		return Root(x1) == Root(x2);
	}

	//�ϲ����鼯
	void Union(size_t x1, size_t x2)
	{
		//���ȵõ�����Ԫ�صĸ�
		int root1 = Root(x1);
		int root2 = Root(x2);

		if (root1 != root2)
		{
			_set[root1] += _set[root2];//�Ѽ���2��Ԫ�ظ����ӵ�����1��
			_set[root2] = root1;//root2�ĸ�����root1
		}
	}

	//���ж��ٸ�����
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