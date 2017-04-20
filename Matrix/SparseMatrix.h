#pragma once
#include <vector>

//三元组
template <typename T>
struct Triple
{
	T _value;
	size_t _row;
	size_t _col;

	Triple()
	{}

	Triple(size_t row, size_t col, const T& value)
		:_row(row)
		, _col(col)
		, _value(value)
	{}
};

//稀疏矩阵压缩存储
template <typename T>
class SparseMatrix
{
public:
	SparseMatrix()
	{}

	SparseMatrix(T* a, size_t rowSize, size_t colSize, const T& invalid = T() )
		:_rowSize(rowSize)
		, _colSize(colSize)
		, _invalid(invalid)
	{
		for (size_t i = 0; i < rowSize; ++i)
		{
			for (size_t j = 0; j < colSize; ++j)
			{
				if (a[i*colSize + j] != invalid)
				{
					_a.push_back(Triple<T>(i, j, a[i*colSize + j]));
				}
			}
		}
	}

	void Display()
	{
		size_t index = 0;
		for (size_t i = 0; i < _rowSize; ++i)
		{
			for (size_t j = 0; j < _colSize; ++j)
			{
				//这里要防止越界index < _a.size()
				if (index < _a.size() && i == _a[index]._row && j == _a[index]._col)
				{
					cout << _a[index]._value <<" ";
					++index;
				}
				else
				{
					cout << _invalid << " ";
				}
			}
			cout << endl;
		}
	}

	//普通转置
	SparseMatrix<T> Transport()
	{
		SparseMatrix<T> tmp;
		//行列数
		tmp._rowSize = _colSize;
		tmp._colSize = _rowSize;
		tmp._invalid = _invalid;
		//先reserve容量
		tmp._a.reserve(_a.size());
		//列优先
		for (size_t j = 0; j < _colSize; ++j)
		{
			//寻找每列的有效元素，都要遍历一遍vector
			size_t index = 0;
			while (index < _a.size())
			{
				//三元组的列等于j
				if (_a[index]._col == j)
				{
					//交换行列，然后push_back
					tmp._a.push_back(Triple<T>(_a[index]._col, _a[index]._row, _a[index]._value));
				}
				++index;
			}
		}
		//调用默认拷贝构造函数
		return tmp;
	}

	//快速转置
	SparseMatrix<T> FastTransport()
	{
		SparseMatrix<T> tmp;
		tmp._rowSize = _colSize;
		tmp._colSize = _rowSize;
		tmp._invalid = _invalid;
		//这里必须resize()，让tmp._a有足够空间，因为下面需要用下标访问vector
		//另外Triple得有默认构造函数
		tmp._a.resize(_a.size(), Triple<T>());
		//统计转置后矩阵每一行的数据个数
		int* rowCounts = new int[tmp._rowSize];
		memset(rowCounts, 0, sizeof(int) * tmp._rowSize);
		
		size_t index = 0;
		while (index < _a.size())
		{
			//_a[index]._col 做下标，对应转置后的行号
			rowCounts[_a[index]._col]++;
			++index;
		}
		//统计转置后的矩阵每行在压缩矩阵中的存储的开始位置
		//rowStart的下标是转置后的行号
		int* rowStart = new int[tmp._rowSize];
		rowStart[0] = 0;
		for (size_t i = 1; i < tmp._rowSize; ++i)
		{
			rowStart[i] = rowStart[i - 1] + rowCounts[i - 1];
		}

		//开始快速转置
		//遍历原vector元素，定位新位置
		index = 0;
		while (index < _a.size())
		{
			//原来三元组的列是转置后的行，也是rowStart的下标
			size_t row = _a[index]._col;
			//直接定位原压缩稀疏矩阵的元素的转置后压缩矩阵位置
			tmp._a[rowStart[row]] = Triple<T>(_a[index]._col, _a[index]._row, _a[index]._value);
			//第row行已经存入新元素，让这一行的rowStart++
			rowStart[row]++;
			++index;
		}
		delete[] rowCounts;
		delete[] rowStart;
		return tmp;
	}

protected:
	//vector存储压缩矩阵
	vector< Triple<T> > _a;
	size_t _rowSize; //行数
	size_t _colSize; //列数
	T _invalid;      //非法值
};