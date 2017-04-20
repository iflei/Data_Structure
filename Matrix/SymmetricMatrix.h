#pragma once
#include <iostream>
using namespace std;

//∂‘≥∆æÿ’Û—πÀı¥Ê¥¢
template <typename T>
class SymmetricMatrix
{
public:
	SymmetricMatrix(T* a, size_t n)
		:_a(new T[n*(n + 1) / 2])
		, _n(n)
	{
		size_t index = 0;
		for (size_t i = 0; i < n; ++i)
		{
			for (size_t j = 0; j < n; ++j)
			{
				if (i >= j)
					_a[index++] = a[i*n + j];
				else
					break;
			}
		}
	}

	T& Access(size_t i, size_t j)
	{
		if (i < j)
			swap(i, j);
		return _a[i*(i + 1) / 2 + j];
	}

	void Display()
	{
		for (size_t i = 0; i < _n; ++i)
		{
			for (size_t j = 0; j < _n; ++j)
			{
				cout << Access(i, j) << " ";
			}
			cout << endl;
		}
	}

protected:
	T* _a;
	size_t _n;
};
