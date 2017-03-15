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

//默认建大堆
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

		//建堆,从最后一个非叶子结点开始向下调整
		//父结点下标等于子结点下标减1除2
		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			//必须保证左子树和右子树都满足堆
			_AdjustDown(i);
		}
	}

	void Push(const T& x)
	{
		//先push_back到最后
		_a.push_back(x);
		//然后从最后一个结点向上调整
		_AdjustUp(_a.size() - 1);
	}

	//Pop堆顶结点
	void Pop()
	{
		//先交换堆顶结点和最后一个结点
		swap(_a[0], _a[_a.size() - 1]);
		//pop_back最后一个结点
		_a.pop_back();
		//从堆顶开始向下调整一次即可
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
		//左孩子节点下标等于父节点下标乘2加1
		int child = parent * 2 + 1;

		Compare compare; //仿函数用于比较优先级
		while (child < _a.size()) //左孩子存在
		{
			//child指向大孩子
			//如果右孩子存在，并且右孩子优先级大于左孩子
			if (child + 1 < _a.size() && compare(_a[child + 1], _a[child]))
				++child;

			//如果孩子优先级大于父亲，交换孩子和父亲节点的值；然后让parent指向孩子结点，继续上面的操作
			if (compare(_a[child], _a[parent]))
			{
				//这里用swap只能交换内置数据类型
				//swap(_a[child], _a[parent]);
				T tmp = _a[child];
				_a[child] = _a[parent];
				_a[parent] = tmp;

				parent = child;
				child = parent * 2 + 1;
			}
			//如果孩子优先级小于父亲，则不需要继续向下调整
			else
				break;
		}
	}

	void _AdjustUp(int child)
	{
		int parent = (child - 1) / 2;
		Compare compare; //仿函数用于比较优先级
		//如果还没调整到根节点
		while (child > 0)
		{
			if (compare(_a[child], _a[parent]))
			{
				//交换
				T tmp = _a[child];
				_a[child] = _a[parent];
				_a[parent] = tmp;
				//向上调整，把父亲调到孩子处，下面仍然满足需要的堆
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
