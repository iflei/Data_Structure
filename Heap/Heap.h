#pragma once
#include <vector>
using namespace std;

template <typename T>
struct Greater
{
	bool operator()(const T& left, const T& right)
	{
		return left > right;
	}
};

template <typename T>
struct Less
{
	bool operator()(const T& left, const T& right)
	{
		return left < right;
	}
};

//Ĭ�Ͻ����
template <typename T, typename Compare = Greater<T> >
class Heap
{
public:
	Heap()
	{}

	Heap(T* a, size_t n)
		:_a(a, a + n)
	{
		/*
		_a.reserve(n);
		for(size_t i = 0; i < n; ++i)
		{
		_a.push_back(a[i]);
		}
		*/

		//����,�����һ����Ҷ�ӽ�㿪ʼ���µ���
		//������±�����ӽ���±��1��2
		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			//���뱣֤���������������������
			_AdjustDown(i);
		}
	}

	void Push(const T& x)
	{
		//��push_back�����
		_a.push_back(x);
		//Ȼ������һ��������ϵ���
		_AdjustUp(_a.size() - 1);
	}

	//Pop�Ѷ����
	void Pop()
	{
		//�Ƚ����Ѷ��������һ�����
		swap(_a[0], _a[_a.size() - 1]);
		//pop_back���һ�����
		_a.pop_back();
		//�ӶѶ���ʼ���µ���һ�μ���
		_AdjustDown(0);
	}

	T Top()
	{
		return _a[0];
	}
	size_t Size()
	{
		return _a.size();
	}

	bool Empty()
	{
		return _a.empty();
	}

protected:
	void _AdjustDown(int root)
	{
		int parent = root;
		//���ӽڵ��±���ڸ��ڵ��±��2��1
		int child = parent * 2 + 1;

		Compare compare; //�º������ڱȽ����ȼ�
		while (child < _a.size()) //���Ӵ���
		{
			//childָ�����
			//����Һ��Ӵ��ڣ������Һ������ȼ���������
			if (child + 1 < _a.size() && compare(_a[child + 1], _a[child]))
				++child;

			//����������ȼ����ڸ��ף��������Ӻ͸��׽ڵ��ֵ��Ȼ����parentָ���ӽ�㣬��������Ĳ���
			if (compare(_a[child], _a[parent]))
			{
				//������swapֻ�ܽ���������������
				//swap(_a[child], _a[parent]);
				T tmp = _a[child];
				_a[child] = _a[parent];
				_a[parent] = tmp;

				parent = child;
				child = parent * 2 + 1;
			}
			//����������ȼ�С�ڸ��ף�����Ҫ�������µ���
			else
				break;
		}
	}

	void _AdjustUp(int child)
	{
		int parent = (child - 1) / 2;
		Compare compare; //�º������ڱȽ����ȼ�
		//�����û���������ڵ�
		while (child > 0)
		{
			if (compare(_a[child], _a[parent]))
			{
				//����
				T tmp = _a[child];
				_a[child] = _a[parent];
				_a[parent] = tmp;
				//���ϵ������Ѹ��׵������Ӵ���������Ȼ������Ҫ�Ķ�
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}
	}

protected:
	vector<T> _a;
};
