#include "Heap.h"

template <typename T, typename Compare = Greater<T> >
class PriorityQueue
{
public:
	PriorityQueue(T* a, size_t n)
		:_hp(a, n)
	{}

	void Push(const T& x)
	{
		_hp.Push(x);
	}

	void Pop()
	{
		_hp.Pop();
	}

	const T& Top()
	{
		return _hp.Top();
	}

	size_t Size()
	{
		return _hp.Size();
	}

	bool Empty()
	{
		_hp.Empty();
	}
protected:
	Heap<T, Compare> _hp;
};