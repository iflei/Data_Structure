#include <iostream>
#include <assert.h>
using namespace std;

//静态栈
template <typename T, size_t N>
class Stack_S
{
public:
	Stack_S()
		:_top(0)
	{}

	void Push(const T& e)
	{
		assert(_top < N);
		_a[_top++] = e;
	}

	void Pop()
	{
		assert(_top);
		--_top;
	}

	T& Top()
	{
		assert(_top);
		return _a[_top - 1];
	}

	size_t Size()
	{
		return _top;
	}

	bool Empty()
	{
		return _top == 0;
	}
private:
	T _a[N];
	size_t _top;
};

void test_Stack_S()
{
	Stack_S<int, 3> s;
	s.Push(1);
	s.Push(2);
	s.Push(3);
	//s.Push(4);
	cout <<"Empty:"<< s.Empty() << endl;
	s.Pop();
	s.Pop();
	s.Pop();
	//s.Pop();
	cout << "Empty:" << s.Empty() << endl;
}

//动态栈
template <typename T>
class Stack
{
public:
	Stack()
		:_a(NULL)
		, _top(0)
		, _capacity(0)
	{}

	~Stack()
	{
		delete[] _a;
	}

	void Push(const T& e)
	{
		_checkCapacity();
		_a[_top++] = e;
	}

	void Pop()
	{
		assert(_top);
		--_top;
	}

	T& Top()
	{
		assert(_top);
		return _a[_top - 1];
	}

	size_t Size()
	{
		return _top;
	}

	bool Empty()
	{
		return _top == 0;
	}
protected:
	void _checkCapacity()
	{
		if (_top == _capacity)
		{
			//开新空间
			T* tmp = new T[2 * _capacity + 3];
			//复制栈数据
			for (int i = 0; i < _top; ++i)
			{
				tmp[i] = _a[i];
			}
			//释放旧栈
			delete[] _a;
			_a = tmp;
			_capacity = 2 * _capacity + 3;
		}
	}
private:
	T* _a;
	size_t _top;
	size_t _capacity;
};


void testStack()
{
	Stack<int> s;
	s.Push(1);
	s.Push(2);
	s.Push(3);
	s.Push(4);
	s.Pop();
	s.Pop();
	s.Pop();
	s.Pop();
	s.Pop();
}